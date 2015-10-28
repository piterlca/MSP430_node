#ifndef WRAPPERS_MPU_WRAPPER_H_
#define WRAPPERS_MPU_WRAPPER_H_

#include "../MPU-6050/MPU-6050.h"

typedef struct MPU_wrapper
{
	MPU_handle mpu_h;
	uint8_t AccOn : 1;
	uint8_t GyroOn : 1;
}MPU_wrapper;



#endif /* WRAPPERS_MPU_WRAPPER_H_ */
