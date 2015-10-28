#include <stdint.h>
#include "Wrappers/MPU_wrapper.h"
#include "Timers/TimerInit.h"
#include "Timers/ClkSetting.h"
#include "generalDefs.h"
#include "TaskMgr.h"

volatile States state = IDLE;
volatile States last_state = REPORT;

static void init_mpu(MPU_handle mpu)
{
	MPU_init_str mpu_init;

	mpu_init.acc_on = TRUE;
	mpu_init.gyro_on = TRUE;
	mpu_init.sample_rate_divider = 0x20;

	MPU_init(mpu, &mpu_init);
}

TaskMgrHandles* global_init(TaskMgrHandles* handles)
{
	init_mpu(handles->mpu->mpu_h);

    setClks();

    timerInit(BYTE_OVERFLOW);

    return handles;
}



void cycle(TaskMgrHandles* handles)
{
	for(;;)
	{
		switch(state)
		{
			case MEASURE:
			{
				if(handles->mpu->AccOn == TRUE)		MPU_GetAccData(handles->mpu->mpu_h);
				if(handles->mpu->GyroOn == TRUE)	MPU_GetGyroData(handles->mpu->mpu_h);
				last_state = state;
				state = IDLE;
				break;
			}

			case REPORT:
			{
				if(handles->mpu->AccOn == TRUE)
				{
					//FilterData(MPU6050.AccData, KalmanForAcc);
					//UART_print(MPU6050.AccData);
				}
				if(handles->mpu->GyroOn == TRUE)
				{
					//FilterData(MPU6050.GyroData, KalmanForGyro);
					//UART_print(MPU6050.GyroData);
				}
				
				//ProcessToExecution(	MPU6050.GyroData, MPU6050.AccData, NULL	);
				
				last_state = state;
				state = IDLE;
				break;
			}
			
			case IDLE:
				break;
		}
	}
}

/*void UART_print(MPUData* sensorData)
{
	if(sensorData == MPU6050.AccData)	printf("A");
	
	else if(sensorData == MPU6050.GyroData) 	printf("G");
	
	printf("%d %d %d\n", sensorData->x, sensorData->y, sensorData->z);
}
*/
