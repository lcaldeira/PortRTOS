#ifndef PORT_H
#define PORT_H

#include <stdint.h>

// registradores do controlador de interrupção aninhada
#define NVIC_INT_CTRL     (*((volatile unsigned long *) 0xE000ED04))
#define NVIC_SYSPRIO3     (*((volatile unsigned long *) 0xE000ED20))
#define NVIC_SYSTICK_CTRL (*((volatile unsigned long *) 0xE000E010))
#define NVIC_SYSTICK_LOAD (*((volatile unsigned long *) 0xE000E014))

// constantes do temporizador do sistema
#define CPU_CLOCK_HZ        ((uint32_t) 120000000)
#define TICK_RATE_HZ        ((uint32_t) 1000)
#define NVIC_SYSTICK_CLK    0x00000004
#define NVIC_SYSTICK_INT    0x00000002
#define NVIC_SYSTICK_ENABLE 0x00000001

// prioridades de interrupção de kernel
#define PRIO_BITS           4   // 16 níveis de prioridade
#define LOWEST_INT_PRIO     0xF
#define KERNEL_INT_PRIO     (LOWEST_INT_PRIO << (8 - PRIO_BITS))
#define NVIC_PENDSV_PRIO    ((unsigned long) (KERNEL_INT_PRIO << 16))
#define NVIC_SYSTICK_PRIO   ((unsigned long) (KERNEL_INT_PRIO << 24))

// máscaras para definir e limpar um determinado bit
#define REG_BIT_SET(reg,bit) (reg |= (word_t)  (1 << bit))
#define REG_BIT_CLR(reg,bit) (reg &= (word_t) ~(1 << bit))

// mudança de estado de interrupções
#define PENDSV_SET(void)            \
    REG_BIT_CLR(NVIC_INT_CTRL, 27); \
    REG_BIT_SET(NVIC_INT_CTRL, 28)

#define PENDSV_CLR(void)            \
    REG_BIT_CLR(NVIC_INT_CTRL, 28); \
    REG_BIT_SET(NVIC_INT_CTRL, 27)

#define PENDST_SET(void)            \
    REG_BIT_CLR(NVIC_INT_CTRL, 25); \
    REG_BIT_SET(NVIC_INT_CTRL, 26)

#define PENDST_CLR(void)            \
    REG_BIT_CLR(NVIC_INT_CTRL, 26); \
    REG_BIT_SET(NVIC_INT_CTRL, 25)

// tipos e cabeçalhos do port e do SO
typedef unsigned long word_t; 			// tamanho da palavra (32 bits)
extern word_t *stk_sel;		            // pilha da tarefa selecionada
extern void scheduler(void);
extern word_t updateTCBs(void);

// configura e inicia o timer do sistema
void initSysTimer(void);

// prepara a pilha da tarefa conforme a direção de push/pull da CPU
void prepareStack(void *task, word_t **p_stk, int stk_size);

// entrada de região crítica:
//  * desabilita interrupções
#define enterCritical() __asm("CPSID I")

// saída de região crítica:
//  * habilita interrupções
#define exitCritical() __asm("CPSIE I")

// dispacha a primeira tarefa
#define dispatcher()                    \
        exitCritical();                 \
        __asm("SVC 0")

// lança uma chamada de interrupção p/ troca de contexto
#define yield()                         \
    PENDSV_SET()

// armazena o ponteiro da pilha:
#define SAVE_SP()                       \
        __asm("LDR R1, =stk_sel");      \
        __asm("STR R0, [R1]")

// armazena o contexto complementar na pilha:
//  * "store multiple, decrement before"
//  * "move [generic reg.] <- [special reg.]
#define SAVE_CTX()                       \
        __asm("MRS R0, PSP");            \
        __asm("STMDB R0!, {R4-R11, R14}")

// armazena o contexto de ponto flutuante na pilha
#define SAVE_FPU()

// salva o estado completo do processador
#define SAVE_FULL()                     \
    SAVE_FPU();                         \
    SAVE_CTX();                         \
    SAVE_SP()

// carrega o ponteiro da pilha:
#define LOAD_SP()                       \
    __asm("LDR R1, =stk_sel");          \
    __asm("LDR R0, [R1]")
        
// carrega o contexto complementar da pilha:
//  * "load multiple, increment after"
//  * "move [special reg.] <- [generic reg.]
//  * "logical OR"
#define LOAD_CTX()                      \
    __asm("LDMIA R0!, {R4-R11, R14}");  \
    __asm("MSR PSP, R0");               \
    __asm("ORR LR, LR, #0x04")

// carrega o contexto de ponto flutuante da pilha
#define LOAD_FPU()

// restaura o estado completo do processador
#define LOAD_FULL()                    \
    LOAD_SP();                         \
    LOAD_CTX();                        \
    LOAD_FPU()

// sinaliza a entrada na interrupção
#define INT_ENTER(void)             \
    enterCritical()

// saída da interrupção
#define INT_EXIT()                  \
    exitCritical();                 \
    __asm("BX LR")

#endif
