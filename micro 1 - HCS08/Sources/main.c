#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "os.h"

#ifdef __cplusplus
 extern "C"
#endif
void MCU_init(void); /* Device initialization function declaration */

// definição das tarefas
word_t stk1[128];
word_t stk2[128];
word_t stk3[128];

void task1(void) {
	int i=0;	
	for(;;) {
		i++;
        #if (TEST_CASE == 1)
        yield();
        #elif (TEST_CASE > 1) 
        delay(1);
        #endif
	}
}

void task2(void) {
	int i=0;	
	for(;;) {
		i++;
        #if (TEST_CASE == 1)
        yield();
        #elif (TEST_CASE > 1) 
        delay(2);
        #endif
	}
}

void task3(void) {
    int i=0;	
    for(;;) {
        i++;
        #if (TEST_CASE == 1)
        yield();
        #elif (TEST_CASE > 1) 
        delay(3);
        #endif
    }
}

void main(void) {
    MCU_init(); /* call Device Initialization */

    /* include your code here */
  
    // casos de teste definidos em "config.h"
    
    //======================================
    #if (TEST_CASE == 1)
    
    installTask(task1, 1, stk1, 128);
    stk_sel = tcb[0].stk;
    dispatcher();
    
    //======================================
    #elif (TEST_CASE > 1)
        
    installTask(task1, 1, stk1, 128);
    installTask(task2, 2, stk2, 128);
    installTask(task3, 3, stk3, 128);
    startOS();
    
    //======================================
    #endif


    for(;;) {
    /* __RESET_WATCHDOG(); by default, COP is disabled with device init. When enabling, also reset the watchdog. */
    } /* loop forever */
    /* please make sure that you never leave main */
}
