/**
 * A simple demo showing how to put the msp430 into a low power state, but also allowing it
 * to wake up, only when servicing interrupts (Clicking switch and turning on both LED's);
 *
 * */

#include <msp430.h>

#define LED1 BIT0
#define LED2 BIT6
#define ALL_LEDS (LED1 | LED2)

#define S2	 BIT3


int main(void) {
	WDTCTL = WDTPW + WDTHOLD;		//Stop watchdog timer
	P1DIR |= ALL_LEDS;			//Set P1.0 to output direction

	P1IE |= S2;						//Active low switch
	P1IES |= 0x01;					//High to low transition, due to the switch is pulled to ground (active low).

	__enable_interrupt();			//Enable interrupt for this port
	for (;;){
		__low_power_mode_0();		//Turn off the CPU, let interrupts wake it up.
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void VECTORA_ISR (void)
{
	P1OUT ^= ALL_LEDS;		//Toggle the LED on and off
	P1IFG ^= S2; 				//IFG gets set when an interrupt has occurred.
								//We have to clear this, in-order to request another interrupt.
}
