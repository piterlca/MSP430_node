#ifndef MPU_6050_H_
#define MPU_6050_H_

#include <stdint.h>

#include "../SensorsData.h"

#define ACC_X_OFFSET	0
#define ACC_Y_OFFSET	0
#define ACC_Z_OFFSET	0

#define GYRO_X_OFFSET	0
#define GYRO_Y_OFFSET	0
#define GYRO_Z_OFFSET	0

struct MPUSensor;
typedef struct MPUSensor* MPU_handle;

typedef struct MPU_init_str{
	uint8_t acc_on : 1;
	uint8_t gyro_on : 1;
	uint8_t sample_rate_divider;
}MPU_init_str;

void MPU_init(MPU_handle mpu, MPU_init_str* init);

void MPU_GetAccData(MPU_handle mpu);

void MPU_GetGyroData(MPU_handle mpu);

void ConvertToStructure(int8_t* RawData, TriDimData* mpudata);



#endif /* MPU_6050_H_ */


// You're gonna need:
// I2C
// UART
