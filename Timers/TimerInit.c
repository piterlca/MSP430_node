#include <msp430fr4133.h>
#include <stdint.h>
#include "../generalDefs.h"
#include "../TaskMgr.h"

void timerInit(uint16_t compareValue)
{
	_EINT();

	TA0CTL |= TACLR;
	TA0CCR0 = compareValue;
	TA0CTL |=  MC0 | ID0 | ID1 | TASSEL1;
	TA0CCTL0 |= CCIE;
	TA0EX0 |= TAIDEX2 | TAIDEX1 | TAIDEX0;
}

#pragma vector=TIMER0_A0_VECTOR

__interrupt void Timer_A(void)
{
	switch(state)
	{
	case IDLE:
		if(last_state == MEASURE)
		{
			state = REPORT;
		}
		else if(last_state == REPORT)
		{
			state = MEASURE;
		}
		break;

	case MEASURE:
		state = REPORT;
		last_state = MEASURE;
		break;

	case REPORT:
		state = MEASURE;
		last_state = REPORT;
		break;
	}
}
