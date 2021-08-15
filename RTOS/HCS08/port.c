#include "derivative.h"		// para uso de periféricos
#include "port.h"

word_t *stk_sel;
volatile long long cpu_time;

void initSysTimer(void){
	TPM1SC = 0x00;			// reg. de estado do timer 1
	TPM1MOD = 19999;		// módulo para um tick=1ms sob f=20MHz
	TPM1SC = 0x48;			// inicia a contagem	
}

void prepareStack(void *task, word_t **p_stk, int stk_size) {
	// ajusta o ponteiro para a base (final) da pilha
	stk_sel = *p_stk + stk_size;
	// preenche o contexto na pilha
	*(--stk_sel) = (word_t)((int) task & 0xFF); // PC low
	*(--stk_sel) = (word_t)((int) task >> 8); // PC high
	*(--stk_sel) = 0x00; // (default) index register low
	*(--stk_sel) = 0x00; // (default) accumulator
	*(--stk_sel) = 0x60; // (default) condition code register
	*(--stk_sel) = 0x00; // (default) index register high
	// atualiza o ponteiro da pilha
	*p_stk = stk_sel;
}

// IVT[1] (Vswi) :: interrupção de software
interrupt void switchContext(void){
	SAVE_CTX();				// salva o reg. SP em stk_sel
	scheduler();			// chama o escalonador do SO
	LOAD_CTX();				// carrega stk_sel no reg. SP
}

// IVT[7] (Vtpm1ovf) :: timer 1 overflow
interrupt void tickTimer(void){
	TPM1SC_TOF = 0x00;		// limpa a flag de "timer overflow"
	if(updateTCBs()){		// troca o contexto
		SAVE_CTX();
		scheduler();
		LOAD_CTX();
	}
}
