#ifndef OS_H
#define OS_H

#define MAX_TASKS 5

#define RR_COOP 0				        // Round-Robin cooperativo
#define RR_PRIO 1				        // Round-Robin com prioridade

#define TASK_READY 1                    // estado "pronto"
#define TASK_WAITING 0                  // estado "em espera"

#include "port.h"
#include "config.h"

// checa a falta de configurações
#ifndef SCHDLR_MODE
#define SCHDLR_MODE RR_PRIO
#endif

//definições e cabeçalhos
typedef void (*task_t)(void); 	        // ponteiro de tarefa

typedef struct {
	word_t *stk;
	word_t prio;
	word_t state;
	long long timeout;
} tcb_t;
					
extern volatile tcb_t tcb[];	        // vetor de TCBs
extern volatile int ct;			        // tarefa corrente
extern volatile int it;			        // tarefas instaladas
extern volatile long long cpu_ticks;	// contagem do sistema

// macros para verificar o estado da tarefa
#define isTaskReady(i)		(tcb[i].state == TASK_READY)
#define isTaskWaiting(i)	(tcb[i].state == TASK_WAITING)

// instala uma tarefa no SO
void installTask(task_t task, word_t prio, word_t *stk, int stk_size);

// impõe uma espera em milisegundos (64 bits)
void delay(long long ms);

// escalonador de tarefas
void scheduler(void);

/// procura alguma tarefa pronta
word_t updateTCBs(void);

// inicia o RTOS
void startOS(void);

#endif
