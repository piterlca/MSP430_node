
#ifndef I2C_I2C_API_H_
#define I2C_I2C_API_H_

typedef enum I2C_OPERATION_MODE{I2C_MASTER = 0, I2C_SLAVE = 1}I2C_OPERATION_MODE;

#define I2C_BOOL_t uint8_t
//#define I2C_STATUS uint8_t
#define I2C_ADDRESS_t uint16_t
#define I2C_DATA_t int8_t
#define I2C_DATA_CTR_t uint8_t

enum I2C_OPERATION_MODE;

enum{TRANSMIT = 0, RECEIVE = 1};

typedef struct i2c_handle
{

	I2C_ADDRESS_t 					own_address;
	I2C_ADDRESS_t 					dest_address;
	I2C_DATA_CTR_t					n_bytes_to_be_sent;
	I2C_DATA_CTR_t					n_bytes_to_be_rcvd;
	volatile I2C_DATA_CTR_t			data_ctr;
	I2C_BOOL_t						generate_stop_at_the_end;	//determines if STOP condition is generated, when 0 == data_ctr
	volatile I2C_BOOL_t				transmission_on;
}i2c_handle;

typedef enum I2C_STATUS{
	I2C_STATUS_OK = 0,
	I2C_STATUS_TX_DATA_OVERWRITTEN = 1,
	I2C_STATUS_BUS_BUSY = 2,
	I2C_STATUS_SLAVE_NOT_RESPONDING = 3
}I2C_STATUS;

typedef struct i2c_msg_str
{
	int8_t* i2c_buffer;
	uint8_t i2c_buffer_length;
	uint8_t	 generate_stop_after_transmission : 1;
	uint8_t  i2c_op_mode : 1;
	uint16_t destination_address;
}i2c_msg_str;

typedef struct i2c_init_str
{
	I2C_OPERATION_MODE 				op_mode;
	uint8_t							i2c_clock_val;
	uint8_t							tx_buffer_size;
	uint8_t							rx_buffer_size;
}i2c_init_str;

I2C_STATUS i2c_send(i2c_handle* i2c_h, i2c_msg_str* msg);
i2c_handle* get_i2c_h(i2c_handle* i2c_h);
i2c_handle* i2c_init(void);
void i2c_enable(i2c_handle* i2c_h);
void i2c_disable(i2c_handle* i2c_h);

#endif /* I2C_I2C_API_H_ */





