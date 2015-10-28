#include <stdint.h>
#include <stdlib.h>

#include "../generalDefs.h"
#include "../SensorsData.h"
#include "../MPU-6050/MPU-6050.h"
#include "../I2C/i2c_api.h"

#define NO_STOP FALSE
#define STOP TRUE

static const unsigned char MPUaddress = 0b1101000;

typedef struct MPUSensor{
	uint8_t ID;
	uint8_t AccOn;
	uint8_t GyroOn;
	TriDimData* AccData;
	TriDimData* GyroData;
	i2c_handle i2c_h;
}MPUSensor;

enum{
	INT_ENABLE = 0x38, 
	PWR_MGMT_1 = 0x6B, 
	PWR_MGMT_2 = 0x6C, 
	ACC_X_MSB = 0x3B, 
	GYRO_X_MSB = 0x43, 
	ACCEL_CONFIG = 0x1C,
	SMPRT_DIV = 0x19
};

static MPUSensor* init_mpu_struct(MPUSensor* mpu, MPU_init_str* init)
{
	mpu = (MPUSensor*)calloc(1, sizeof(MPUSensor));

	mpu->ID = MPUaddress;
	if(init->acc_on = mpu->AccOn)
	{
		mpu->AccData = (TriDimData*)calloc(	1, sizeof(TriDimData)	);
	}
	
	if(	init->gyro_on = mpu->GyroOn )
	{
		mpu->GyroData = (TriDimData*)calloc(	1, sizeof(TriDimData)	);
	}

	return mpu;
}

void MPU_init(MPUSensor* mpu, MPU_init_str* init)
{
	i2c_msg_str msg;
	int8_t buf[2] = {0};

	mpu = init_mpu_struct(mpu, init);
	
	mpu->i2c_h = i2c_init();

	i2c_enable(mpu->i2c_h);

	msg.destination_address = mpu->ID;
	msg.generate_stop_after_transmission = 1;
	msg.i2c_buffer_length = 2;
	buf[0] = PWR_MGMT_1;
	buf[1] = 0x00;	// Operating mode = NORMAL
	msg.i2c_buffer = (int8_t*)&buf;
	i2c_send(mpu->i2c_h, &msg);


	buf[0] = SMPRT_DIV;
	buf[1] = init->sample_rate_divider;
	i2c_send(mpu->i2c_h, &msg);


	msg.generate_stop_after_transmission = 1;
	buf[0] = INT_ENABLE;
	buf[1] = 0x01; //enable data_Rdy interrupt for testing
	i2c_send(mpu->i2c_h, &msg);

}

void MPU_GetAccData(MPUSensor* mpu)
{
	i2c_msg_str msg;
	uint8_t txbuf = 0;
	uint8_t rxbuf[6] = {0};
	i2c_handle i2c_h = NULL;

	i2c_h = get_i2c_h(i2c_h);

	msg.destination_address = mpu->ID;
	msg.generate_stop_after_transmission = 0;
	msg.i2c_buffer_length = 1;
	msg.i2c_buffer = (int8_t*)&txbuf;
	txbuf = ACC_X_MSB;
	i2c_send(i2c_h, &msg);
	
	msg.generate_stop_after_transmission = 1;
	msg.i2c_buffer_length = 6;
	msg.i2c_buffer = (int8_t*)&rxbuf;
	
	i2c_recv(i2c_h, &msg);
	
	//ConvertToStructure(I2C->InputBuffer, MPU6050.RawAcc);


}

void MPU_GetGyroData(MPUSensor* mpu)
{
	i2c_msg_str msg;
	uint8_t txbuf = 0;
	uint8_t rxbuf[6] = {0};
	i2c_handle i2c_h = NULL;

	i2c_h = get_i2c_h(i2c_h);

	msg.destination_address = mpu->ID;
	msg.generate_stop_after_transmission = 0;
	msg.i2c_buffer_length = 1;
	msg.i2c_buffer = (int8_t*)&txbuf;
	txbuf = GYRO_X_MSB;
	i2c_send(i2c_h, &msg);
	
	msg.generate_stop_after_transmission = 1;
	msg.i2c_buffer_length = 6;
	msg.i2c_buffer = (int8_t*)&rxbuf;
	
	i2c_recv(i2c_h, &msg);

}

void ConvertToStructure(int8_t* RawData, TriDimData* mpudata)
{
	mpudata->x = ( RawData[0] << 8 | RawData[1] ) + 300;
	mpudata->y = ( RawData[2] << 8 | RawData[3] ) + 100;
	mpudata->z = ( RawData[4] << 8 | RawData[5] ) + 20;
}
