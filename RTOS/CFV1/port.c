#include "derivative.h"		// para uso de periféricos
#include "port.h"

word_t *stk_sel;

void initSysTimer(void){
	TPM1SC = 0x00;			// reg. de estado do timer 1
	TPM1MOD = 19999;		// módulo para um tick=1ms sob f=20MHz
	TPM1SC = 0x48;			// inicia a contagem	
}

void prepareStack(void *task, word_t **p_stk, int stk_size) {
	// ajusta o ponteiro para a base (final) da pilha
	stk_sel = (word_t*) (*p_stk + stk_size);
	// preenche o contexto na pilha
	*(--stk_sel) = (word_t) task;	// PC da tarefa
	*(--stk_sel) = 0x40002000; 		// (default) reg. de status
	// (autom. stacked registers: D2-D0/A1-A0)
	*(--stk_sel) = 0xA1;
	*(--stk_sel) = 0xA0;
	*(--stk_sel) = 0xD2;
	*(--stk_sel) = 0xD1;
	*(--stk_sel) = 0xD0;
	// (manual stacked registers: D7-D3/A6-A2)
	*(--stk_sel) = 0xA6;
	*(--stk_sel) = 0xA5;
	*(--stk_sel) = 0xA4;
	*(--stk_sel) = 0xA3;
	*(--stk_sel) = 0xA2;
	*(--stk_sel) = 0xD7;
	*(--stk_sel) = 0xD6;
	*(--stk_sel) = 0xD5;
	*(--stk_sel) = 0xD4;
	*(--stk_sel) = 0xD3;
	// atualiza o ponteiro da pilha
	*p_stk = (word_t*) stk_sel;
}

// IVT[32] (Vtrap0) :: interrupção de software
interrupt void switchContext(void){
	SAVE_CTX();				// salva o contexto da CPU
	scheduler();			// chama o escalonador do SO
	LOAD_CTX();				// carrega o contexto da CPU
}

// IVT[69] (Vtpm1ovf) :: timer 1 overflow
interrupt void tickTimer(void){
	TPM1SC_TOF = 0x00;		// limpa a flag de "timer overflow"
	if(updateTCBs()){		// troca o contexto se houver tarefa pronta
		SAVE_CTX();
		scheduler();
		LOAD_CTX();
	}
}