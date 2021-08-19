#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"

void mclkSetup120MHz(){
    /*==========================================
         baseado nos exemplos do Tivaware
    ==========================================*/

    MAP_SysCtlMOSCConfigSet(SYSCTL_MOSC_HIGHFREQ);

   // Run from the PLL at 120 MHz.
   (void) MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
               SYSCTL_OSC_MAIN |
               SYSCTL_USE_PLL |
               SYSCTL_CFG_VCO_480), 120000000);
}
