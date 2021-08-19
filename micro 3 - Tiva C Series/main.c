#include "os.h"
#include "init.h"

/**
 * main.c
 */

word_t stk1[128];
word_t stk2[128];
word_t stk3[128];

void task1(void) {
    int i=0;
    for(;i>=0;) {
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
    for(;i>=0;) {
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
    for(;i>=0;) {
        i++;
        #if (TEST_CASE == 1)
        yield();
        #elif (TEST_CASE > 1)
        delay(3);
        #endif
    }
}

int main(void)
{
    // setup do master clock
    mclkSetup120MHz();

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

    for(;;) {}
	return 0;
}
