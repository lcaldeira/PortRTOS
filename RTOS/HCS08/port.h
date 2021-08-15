#ifndef PORT_H
#define PORT_H

typedef unsigned char word_t; 			// tamanho da palavra (8 bits)
extern word_t *stk_sel;					// pilha da tarefa selecionada
extern void scheduler(void);            // escalonador de tarefas do SO
extern word_t updateTCBs(void);

// configura e inicia o timer do sistema
void initSysTimer(void);

// prepara a pilha da tarefa conforme a direção de push/pull da CPU
void prepareStack(void *task, word_t **p_stk, int stk_size);

// desempilha a tarefa selecionada conforme os seguintes passos:
//	* carrega o ponteiro de 16 bits da RAM para HX
//	* sobrescreve SP com HX (único reg. q faz isso)
//	* desempilha HX high manualmente (por software)
//	* desempilha os demais registradores (automaticamente)
#define dispatcher()				\
	asm("LDHX stk_sel");			\
	asm("TXS");						\
	asm("PULH");					\
	asm("RTI")

// armazena o registrador de ponteiro de pilha na memória:
//	* copia de SP para HX (low)
//	* copia de HX (low) para a memória
#define SAVE_CTX()					\
	asm("TSX");						\
    asm("STHX stk_sel")
        
// carrega o registrador de ponteiro de pilha da memória:
//	* copia da memória para HX (low)
//	* copia de HX (low) para SP
#define LOAD_CTX()					\
    asm("LDHX stk_sel");			\
    asm("TXS")

// cede o uso do processador disparando uma interrupção de software
#define yield() asm("SWI")

#endif
