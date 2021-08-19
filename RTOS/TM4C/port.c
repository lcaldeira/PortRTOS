//#include "derivative.h"		// para uso de perif�ricos
#include <mylib/port.h>

word_t *stk_sel;

void initSysTimer(void){
    uint32_t cmp_value = (CPU_CLOCK_HZ / TICK_RATE_HZ) - 1;
	NVIC_SYSTICK_CTRL = 0x00;	    // limpa o o reg. de status
	NVIC_SYSTICK_LOAD = cmp_value;  // valor de comparação (módulo)
	// inicia a contagem, habilitando o clock e interrupção
	NVIC_SYSTICK_CTRL = NVIC_SYSTICK_CLK | NVIC_SYSTICK_INT | NVIC_SYSTICK_ENABLE;
}

void prepareStack(void *task, word_t **p_stk, int stk_size) {
	// ajusta o ponteiro para a base (final) da pilha
	stk_sel = (word_t*) ((int) *p_stk + (stk_size * sizeof(word_t)));
	// preenche o contexto na pilha
	// (xPSR, PC, LR)
    *(--stk_sel) = (word_t) 0x01000000; // xPSR (program status register)
    *(--stk_sel) = (word_t) task;       // PC (program counter)
    *(--stk_sel) = 0x0014;              // LR (link register)
	// (autom. stacked registers: R12, R0-R3)
	*(--stk_sel) = 0x0012;
	*(--stk_sel) = 0x0003;
	*(--stk_sel) = 0x0002;
	*(--stk_sel) = 0x0001;
	*(--stk_sel) = 0x0000;
	// (manual stacked registers: R14, R11-R4)
    //*(--stk_sel) = 0x0013;             // PSP (process stack pointer)
    *(--stk_sel) = 0xFFFFFFFD;           // LR as EXC_RETURN
	*(--stk_sel) = 0x0011;
	*(--stk_sel) = 0x0010;
	*(--stk_sel) = 0x0009;
	*(--stk_sel) = 0x0008;
	*(--stk_sel) = 0x0007;
	*(--stk_sel) = 0x0006;
	*(--stk_sel) = 0x0005;
	*(--stk_sel) = 0x0004;
	// atualiza o ponteiro da pilha
	*p_stk = (word_t*) stk_sel;
}

// IVT[10] (SVCall) :: interrupção de software
__attribute__ ((naked))
void establishContext(void){
    //NVIC_SYSPRIO3 |= NVIC_PENDSV_PRIO;
    //NVIC_SYSPRIO3 |= NVIC_SYSTICK_PRIO;
    LOAD_FULL();            // carrega o contexto da CPU
    INT_EXIT();             // sai da interrupção
}

// IVT[13] (PendSV) :: interrupção de software pendente
__attribute__ ((naked))
void switchContext(void){
    INT_ENTER();            // entrada segura da interrupção
    PENDSV_CLR();           // limpa a flag de PendSV
    SAVE_FULL();			// salva o contexto da CPU
	scheduler();			// chama o escalonador do SO
	LOAD_FULL();			// carrega o contexto da CPU
	INT_EXIT();             // saída segura da interrupção
}

// IVT[14] (SysTick) :: system timer overflow
//__attribute__ ((naked))
void tickTimer(void){
    PENDST_CLR();           // limpa a flag de SysTick pendente
	if(updateTCBs()){
	    yield();
	}
}
