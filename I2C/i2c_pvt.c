/*
 * i2c.c

 *
 *  Created on: 8 paü 2015
 *      Author: Piotr
 */
#include <stdint.h>
#include <string.h>
#include <msp430fr4133.h>
#include "i2c_definitions.h"
#include "i2c_api.h"

static I2C_STATUS i2c_send_pvt(i2c_handle* i2c_h, int8_t* buf, uint8_t len, uint8_t stop);

i2c_handle* get_i2c_h(i2c_handle* i2c_h)
{
	static i2c_handle* i2c = NULL;

	if(NULL != i2c_h)
		i2c = i2c_h;

	else
		return i2c;
}

I2C_STATUS i2c_send(i2c_handle* i2c_h, i2c_msg_str* msg)
{
	I2C_STATUS status;
	UCB0I2CSA = msg->destination_address;

	status = i2c_send_pvt(
				i2c_h,
				msg->i2c_buffer,
				msg->i2c_buffer_length,
				msg->generate_stop_after_transmission);

	return status;
}

I2C_STATUS i2c_recv(i2c_handle* i2c_h, i2c_msg_str* msg)
{
	I2C_STATUS status;
	UCB0I2CSA = msg->destination_address;

	status = i2c_recv_pvt(
			i2c_h,
			msg->i2c_buffer,
			msg->i2c_buffer_length,
			msg->generate_stop_after_transmission);

	return status;

}

I2C_STATUS i2c_recv_pvt(
		i2c_handle* i2c_h,
		int8_t* buf,
		uint8_t len,
		uint8_t stop)
{
	UCB0CTLW0 &= ~UCTR;

	uint8_t byte_ctr = 0;

	while((UCB0STATW & UCBBUSY) && (!i2c_h->transmission_on));

	UCB0CTLW0 |= UCTXSTT;
	i2c_h->transmission_on = 1;

	while(byte_ctr < len)
	{
		while(!UCB0IFG);

		if(UCB0IFG & UCRXIFG0)
			buf[byte_ctr++] = UCB0RXBUF ;

		else if(UCB0IFG & UCBIT9IFG)
			;

		else
		{
			UCB0CTLW0 |= UCTXSTP;
			i2c_h->transmission_on = 0;
			return I2C_STATUS_SLAVE_NOT_RESPONDING;
		}


		if(stop && (byte_ctr == (len - 1)))
		{
			UCB0CTLW0 |= UCTXSTP;
		}
	}


	while(UCB0CTLW0 & UCTXSTP);
	i2c_h->transmission_on = 0;



}

i2c_handle* i2c_init(void)
{
	i2c_handle* i2c_h = (i2c_handle*)malloc(sizeof(i2c_handle));

	get_i2c_h(i2c_h);

	UCB0CTL1 |= UCSWRST;

	SET_I2C_PIN_FUNCTIONS;

	UCB0CTLW0 	|= UCMODE_3
				| UCSSEL1
				| UCSYNC;

	//if(I2C_MASTER == i2c_h->op_mode)
	{
		UCB0CTLW0 |= UCMST | UCTR;
	}
	//else
	{
	//	UCB0CTLW0 &= ~UCMST & ~UCTR;
	}

	UCB0BRW = 15;
}

void i2c_disable(i2c_handle* i2c_h)
{
	UCB0CTLW0 |= UCTXSTP;

	UCB0CTL1 ^= UCSWRST;
}

void i2c_enable(i2c_handle* i2c_h)
{
	UCB0CTL1 &= ~UCSWRST;

	//UCB0CTLW0 |= UCTXSTP;
}

static I2C_STATUS i2c_send_pvt(i2c_handle* i2c_h, int8_t* buf, uint8_t len, uint8_t stop)
{
	uint8_t byte_ctr = 0;

	UCB0CTLW0 |= UCTR;

	while((UCB0STATW & UCBBUSY) && (!i2c_h->transmission_on));
		//return I2C_STATUS_BUS_BUSY;

	UCB0CTLW0 |= UCTXSTT;
	i2c_h->transmission_on = 1;

	while(byte_ctr < len)
	{
		while(!UCB0IFG);

		if(UCB0IFG & UCTXIFG0)
			UCB0TXBUF = buf[byte_ctr++];

		else if(UCB0IFG & UCBIT9IFG)
			;

		else
		{
			UCB0CTLW0 |= UCTXSTP;
			i2c_h->transmission_on = 0;
			return I2C_STATUS_SLAVE_NOT_RESPONDING;
		}

	}

	while(!(UCB0IFG & UCTXIFG0));

	if(stop)
	{
		UCB0CTLW0 |= UCTXSTP;
		i2c_h->transmission_on = 0;

		while(UCB0CTLW0 & UCTXSTP);
	}
}


/*#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
	i2c_handle* i2c_h = NULL;
	i2c_h = get_i2c_h(i2c_h);

	if(UCB0IFG & UCTXIFG0)
	{
		if(i2c_h->n_bytes_to_be_sent - i2c_h->data_ctr)
		{
			UCB0TXBUF = i2c_h->tx_buf[i2c_h->data_ctr];

			++i2c_h->data_ctr;
		}
		else
		{
			if(i2c_h->generate_stop_at_the_end)				UCB0CTLW0 |= UCTXSTP;
			else											UCB0CTLW0 |= UCTXSTT;

			i2c_h->data_ctr = 0;

			i2c_h->bus_busy = FALSE;
		}
	}

	if(UCB0IFG & UCNACKIFG)
	{
		UCB0CTLW0 |= UCTXSTP;
	}
}*/

I2C_STATUS check_i2c_status(void)
{
	return 0;
}
