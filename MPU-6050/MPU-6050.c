#include <stdint.h>
#include <stdlib.h>

#include "../generalDefs.h"
#include "../SensorsData.h"
#include "../MPU-6050/MPU-6050.h"
#include "../I2C/i2c_api.h"

#define NO_STOP FALSE
#define STOP TRUE

static const unsigned char MPUaddress = 0b1101000;

MPUSensor MPU6050;

static const enum{	
	INT_ENABLE = 0x38, 
	PWR_MGMT_1 = 0x6B, 
	PWR_MGMT_2 = 0x6C, 
	ACC_X_MSB = 0x3B, 
	GYRO_X_MSB = 0x43, 
	ACCEL_CONFIG = 0x1C,
	SMPRT_DIV = 0x19
} MPU_Registers;

void MPU_init(uint8_t AccOn, uint8_t GyroOn, uint8_t SampleRateDivider)
{
	I2C_STATUS status;
	i2c_msg_str msg;
	uint8_t buf[2] = {0};
	i2c_handle* i2c_h =	i2c_init();
	i2c_enable(i2c_h);

	MPU6050.ID = MPUaddress;
	if(	(MPU6050.AccOn = AccOn) == TRUE)
	{
		MPU6050.RawAcc = (TriDimData*)calloc(	1, sizeof(TriDimData)	);
	}
	
	if(	(MPU6050.GyroOn = GyroOn) == TRUE)	
	{
		MPU6050.RawGyro = (TriDimData*)calloc(	1, sizeof(TriDimData)	);
	}
	

	msg.destination_address = MPU6050.ID;
	msg.generate_stop_after_transmission = 1;
	msg.i2c_buffer_length = 2;
	buf[0] = PWR_MGMT_1;
	buf[1] = 0x00;	// Operating mode = NORMAL
	msg.i2c_buffer = &buf;
	status = i2c_send(i2c_h, &msg);


	buf[0] = SMPRT_DIV;
	buf[1] = SampleRateDivider;
	i2c_send(i2c_h, &msg);



	msg.generate_stop_after_transmission = 1;
	buf[0] = INT_ENABLE;
	buf[1] = 0x01; //enable data_Rdy interrupt for testing
	i2c_send(i2c_h, &msg);


}

void MPU_GetAccData()
{
	I2C_STATUS status;
	i2c_msg_str msg;
	uint8_t txbuf = 0;
	uint8_t rxbuf[6] = {0};
	i2c_handle* i2c_h = NULL;

	i2c_h = get_i2c_h(i2c_h);

	msg.destination_address = MPU6050.ID;
	msg.generate_stop_after_transmission = 0;
	msg.i2c_buffer_length = 1;
	msg.i2c_buffer = &txbuf;
	txbuf = ACC_X_MSB;
	i2c_send(i2c_h, &msg);
	
	msg.generate_stop_after_transmission = 1;
	msg.i2c_buffer_length = 6;
	msg.i2c_buffer = &rxbuf;
	
	i2c_recv(i2c_h, &msg);
	
	//ConvertToStructure(I2C->InputBuffer, MPU6050.RawAcc);
}

void MPU_GetGyroData()
{
	I2C_STATUS status;
	i2c_msg_str msg;
	uint8_t txbuf = 0;
	uint8_t rxbuf[6] = {0};
	i2c_handle* i2c_h = NULL;

	i2c_h = get_i2c_h(i2c_h);

	msg.destination_address = MPU6050.ID;
	msg.generate_stop_after_transmission = 0;
	msg.i2c_buffer_length = 1;
	msg.i2c_buffer = &txbuf;
	txbuf = GYRO_X_MSB;
	i2c_send(i2c_h, &msg);
	
	msg.generate_stop_after_transmission = 1;
	msg.i2c_buffer_length = 6;
	msg.i2c_buffer = &rxbuf;
	
	i2c_recv(i2c_h, &msg);
}

void ConvertToStructure(int8_t* RawData, TriDimData* mpudata)
{
	mpudata->x = ( RawData[0] << 8 | RawData[1] ) + 300;
	mpudata->y = ( RawData[2] << 8 | RawData[3] ) + 100;
	mpudata->z = ( RawData[4] << 8 | RawData[5] ) + 20;
}
