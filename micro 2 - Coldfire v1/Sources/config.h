// casos de teste:
//  1) tarefa �nica com desist�ncia do processador
//  2) m�ltiplas tarefas com espera em modo cooperativo
//  3) m�ltiplas tarefas com espera usando prioridades

#define TEST_CASE 3

#if (TEST_CASE > 2)
#define SCHDLR_MODE RR_PRIO
#else
#define SCHDLR_MODE RR_COOP
#endif