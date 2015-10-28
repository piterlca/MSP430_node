/*
 * MeasuringAndPresenting.h
 *
 *  Created on: Mar 21, 2015
 *      Author: Piotr
 */

#ifndef MEASURINGANDPRESENTING_H_
#define MEASURINGANDPRESENTING_H_

#include "Wrappers/MPU_wrapper.h"

typedef enum {MEASURE, REPORT, IDLE} States;
extern volatile States state;
extern volatile States last_state;

typedef struct TaskMgrHandles
{
	MPU_wrapper* mpu;
}TaskMgrHandles;

void cycle();

TaskMgrHandles* global_init(TaskMgrHandles* handles);

#endif /* MEASURINGANDPRESENTING_H_ */
