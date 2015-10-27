#include <msp430.h>
#include "generalDefs.h"
#include "MPU-6050/MPU-6050.h"
#include "TaskMgr.h"
#include "TimerInit.h"
#include "ClkSetting.h"

int main(void) {

	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode



    // to activate previously configured port settings
    //I2C_HiLevelInit(16, 16);

//    i2c_trial();
    setClks();

    //i2c_init();

    MPU_init(TRUE, FALSE, 0x20);

    timerInit(0xFF);

    //MPU_GetGyroData();


    //MPU_GetGyroData();



	//i2c_send_stop();

    //MPU_GetAccData();


    for(;;)
    {
    	cycle();
    }
}
