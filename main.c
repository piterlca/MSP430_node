#include <msp430.h>
#include "TaskMgr.h"

int main(void) {

	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
    TaskMgrHandles handles;

    (void)global_init(&handles);

    for(;;)
    {
    	cycle(&handles);
    }
}
