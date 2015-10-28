typedef struct i2c_str
{

	I2C_ADDRESS_t 					own_address;
	I2C_ADDRESS_t 					dest_address;
	I2C_DATA_CTR_t					n_bytes_to_be_sent;
	I2C_DATA_CTR_t					n_bytes_to_be_rcvd;
	volatile I2C_DATA_CTR_t			data_ctr;
	I2C_BOOL_t						generate_stop_at_the_end;	//determines if STOP condition is generated, when 0 == data_ctr
	volatile I2C_BOOL_t				transmission_on;
}i2c_str;


I2C_STATUS i2c_recv_pvt(
		i2c_str* i2c_h,
		int8_t* buf,
		uint8_t len,
		uint8_t stop);

static I2C_STATUS i2c_send_pvt(
		i2c_str* i2c_h,
		int8_t* buf,
		uint8_t len,
		uint8_t stop);
