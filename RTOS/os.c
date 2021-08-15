#include "os.h"

volatile tcb_t tcb[MAX_TASKS+1];
volatile int ct = 0;
volatile int it = 0;
word_t iddleStk[128];
volatile long long cpu_ticks;

void installTask(task_t task, word_t prio, word_t *stk, int stk_size) {
	prepareStack(task, &stk, stk_size);
	tcb[it].stk = stk;
	tcb[it].prio = prio;
	tcb[it].state = TASK_READY;
	tcb[it].timeout = 0;
	it++;
}

void iddleTask(void){
	// deveria entrar em baixo consumo de energia
	for(;;){
	}
}

void delay(long long ms){
	tcb[ct].timeout = cpu_ticks + ms;
	tcb[ct].state = TASK_WAITING;
	yield();
}

word_t updateTCBs(void){
	int i;
	word_t any_task_ready = 0;
	cpu_ticks++;
	
	for(i = 0; i < it; i++){
		if(tcb[i].timeout == cpu_ticks){
			tcb[i].state = TASK_READY;
			any_task_ready = 1;
		}
	}
	return any_task_ready;
}

void scheduler(void){
	#if (SCHDLR_MODE == RR_PRIO)
	int higher_prio = 0;
	int higher_task = 0;
	int i;
	#endif
	// salva a pilha atual
	tcb[ct].stk = (word_t*) stk_sel;
	// escolhe a próxima tarefa
	#if (SCHDLR_MODE == RR_COOP)
	do{
		ct++;
		if(ct >= it){
			ct = 0;
		}
	}while(!isTaskReady(ct));
	#elif (SCHDLR_MODE == RR_PRIO)
	for(i=0; i<it; i++){
		if(isTaskReady(i) && tcb[i].prio >= higher_prio){
			higher_prio = tcb[i].prio;
			higher_task = i;
		}
	}
	ct = higher_task;
	#endif
	// redefine a próxima pilha
	stk_sel = tcb[ct].stk;
}

void startOS(void){
	installTask(iddleTask, 0, iddleStk, 128);
	initSysTimer();
	cpu_ticks = 0;
	stk_sel = tcb[ct].stk;
	#if (SCHDLR_MODE == RR_PRIO)
	scheduler();
	#endif
	dispatcher();
}