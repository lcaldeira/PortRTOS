#ifndef PORT_H
#define PORT_H

typedef unsigned long word_t; 			// tamanho da palavra (32 bits)
extern word_t *stk_sel;		            // pilha da tarefa selecionada
extern void scheduler(void);
extern word_t updateTCBs(void);

// configura e inicia o timer do sistema
void initSysTimer(void);

// prepara a pilha da tarefa conforme a direção de push/pull da CPU
void prepareStack(void *task, word_t **p_stk, int stk_size);

// desempilha a tarefa selecionada conforme os seguintes passos:
//	* carrega o ponteiro de 32 bits para SP (A7)
//	* desempilha o que é apontado por A7, salvo manualmente
//	* desloca o SP em 40 bytes (4 bytes X 10 reg.)
//	* desempilha o que é apontado por A7, salvo automaticamente
//	* desloca o SP em 20 bytes (4 bytes X 5 reg.)
//	* desempilha os demais registradores (automaticamente)
#define dispatcher()					\
	asm(MOVE stk_sel, A7);				\
	asm(MOVEM.L (A7), D3-D7/A2-A6);		\
	asm(LEA 40(A7), A7);				\
	asm(MOVEM.L (A7), D0-D2/A0-A1);		\
	asm(LEA 20(A7), A7);				\
	asm(RTE)

// armazena o contexto complementar na pilha:
//	* reserva 40 bytes na pilha
//	* descarrega os registradores na pilha
//	* armazena o SP na memória
#define SAVE_CTX() 						\
	asm(LEA -40(A7), A7);				\
	asm(MOVEM.L D3-D7/A2-A6, (A7));		\
	asm(MOVE A7, stk_sel)		
        
// carrega o contexto complementar da pilha:
//	* carrega o SP da memória
//	* carrega os registradores da pilha
//	* libera 40 bytes da pilha
#define LOAD_CTX()						\
	asm(MOVE stk_sel, A7);				\
	asm(MOVEM.L (A7), D3-D7/A2-A6);		\
	asm(LEA 40(A7), A7)

// cede o uso do processador disparando uma interrupção de software
#define yield() asm(TRAP %0)

#endif
