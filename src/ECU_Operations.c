/*
 * ECU_Operations.c
 *
 *  Created on: Nov 3, 2017
 *      Author: arnav
 */

#include "ECU_Operations.h"
#include "derivative.h"
#include "ADC.h"
#include "UART.h"
#include "stdlib.h"
#include "Input_Scan.h"
#include "CAN.h"

#define PTE7 7
#define PTC4 20 									/* Port output to fault LED */
#define PTA4 4
#define PTD7 31
//Analog to Digital Converter input values:
uint32_t brakeAngle;
uint32_t steeringAngle;
int APPS_flag = 0; 			//0 when no fault, 1 when fault
int BSE_flag = 0;			//0 when no fault, 1 when fault
int APPS_faultcount = 0; 	//number of times APPS faults have occurred
int BSE_faultcount = 0; 	//number of times BSE faults have occurred
uint16_t Throttle_L = 0;
uint16_t Throttle_R = 0;



//returns 1 if fault, 0 if no fault. (checks acc pedal transfer functions)
int APPS_Fault(uint8_t acc1, uint8_t acc2){
	//25 is 10% of 255 which is the max value for ADC inputs
	//if acc1 or 2 is <10 then there is a disconnect on the line since it is pulled down.
	//if apps flag has been already triggered but fault is still occurring, do nothing
	acc2 = acc2*1.2945;
	if((abs(acc1-acc2) > 25) || (acc1 < 10) || (acc2 < 10)) {
		return 1;
	}
	//if there is no fault occurring, disable APPS flag if it is triggered and continue execution.
	else {
		return 0;
	}
}

//returns 1 if APBS fault, 0 if no fault (checks that brake is not depressed when acc is depressed >25%)
int BSE_Fault(uint8_t brakeAngle, uint8_t acc1, uint8_t acc2){
	acc2 = acc2*1.2945;
	if((BSE_flag) && (acc1 > 13 || acc2 > 13)) { //13 is 5% of 255
		return 1;
	}
	if((acc1 > 64 || acc2 > 64) && brakeAngle > 0x88){ //64 is 25% of 255      0x88 is 53% of 5V where 2.67V is "hard press" of brakes
		BSE_flag = 1;
		return 1;
	}
	else if ((BSE_flag) && (acc1 < 13 && acc2 < 13)){ //13 is 5% of 255  if flag is already set, flag will clear when APPS values are less than 5%
		BSE_flag = 0;
		return 0;
	}
	return 0;

}

//TODO: @Arnav test the averaging function
//sets the throttle value based on the value of received from the front ECU for each wheel.
//This also performs averaging by computing the average of the past 5 samples in order to get a smoother response.
//this is done by setting the compare match value on the PWM output pin. (0-255)
void set_Throttle_Value(uint8_t leftpos, uint8_t rightpos){

	Throttle_L = leftpos;
	Throttle_R = rightpos;
	FTM2_C0V = 4*Throttle_L;
	FTM2_C1V = 4*Throttle_R;
}




#ifdef FrontECU
void Speed (uint8_t Speed)
{
	FGPIOA_PCOR = Speed5A | Speed7A | Speed1B | Speed2B | Speed3B | Speed4B;
	FGPIOB_PCOR = Speed1A | Speed2A | Speed3A | Speed4A | Speed6A | Speed5B | Speed6B | Speed7B;
	uint8_t Digit1;
	uint8_t Digit2;
	if (Speed < 100)
	{
		Digit1 = Speed % 10;
		Speed /= 10;
		Digit2 = Speed % 10;
	}
	else
	{
		//error handler
	}
	switch(Digit1)
	{
		case 0:
		{
			FGPIOA_PSOR = Speed5A;
			FGPIOB_PSOR = Speed1A | Speed2A | Speed3A | Speed4A | Speed6A;
			break;
		}
		case 1:
		{
			FGPIOB_PSOR = Speed2A | Speed3A;
			break;
		}
		case 2:
		{
			FGPIOA_PSOR = Speed5A | Speed7A;
			FGPIOB_PSOR = Speed1A | Speed2A | Speed4A;
			break;
		}
		case 3:
		{
			FGPIOA_PSOR = Speed7A;
			FGPIOB_PSOR = Speed1A | Speed2A | Speed3A | Speed4A;
			break;
		}
		case 4:
		{
			FGPIOA_PSOR = Speed7A;
			FGPIOB_PSOR = Speed2A | Speed3A | Speed6A;
			break;
		}
		case 5:
		{
			FGPIOA_PSOR = Speed7A;
			FGPIOB_PSOR = Speed1A | Speed3A | Speed4A | Speed6A;
			break;
		}
		case 6:
		{
			FGPIOA_PSOR = Speed5A | Speed7A;
			FGPIOB_PSOR = Speed1A | Speed3A | Speed4A | Speed6A;
			break;
		}
		case 7:
		{
			FGPIOB_PSOR = Speed1A | Speed2A | Speed3A;
			break;
		}
		case 8:
		{
			FGPIOA_PSOR = Speed5A | Speed7A;
			FGPIOB_PSOR = Speed1A | Speed2A | Speed3A | Speed4A | Speed6A;
			break;
		}
		case 9:
		{
			FGPIOA_PSOR = Speed7A;
			FGPIOB_PSOR = Speed1A | Speed2A | Speed3A | Speed6A;
			break;
		}
	}
	switch(Digit2)
	{
		case 0:
		{
			FGPIOA_PSOR = Speed1B | Speed2B | Speed3B | Speed4B;
			FGPIOB_PSOR = Speed5B | Speed6B;
			break;
		}
		case 1:
		{
			FGPIOA_PSOR = Speed2B | Speed3B;
			break;
		}
		case 2:
		{
			FGPIOA_PSOR = Speed1B | Speed2B | Speed4B;
			FGPIOB_PSOR = Speed5B | Speed7B;
			break;
		}
		case 3:
		{
			FGPIOA_PSOR = Speed1B | Speed2B | Speed3B | Speed4B;
			FGPIOB_PSOR = Speed7B;
			break;
		}
		case 4:
		{
			FGPIOA_PSOR = Speed2B | Speed3B;
			FGPIOB_PSOR = Speed7B | Speed6B;
			break;
		}
		case 5:
		{
			FGPIOA_PSOR = Speed1B | Speed3B | Speed4B;
			FGPIOB_PSOR = Speed7B | Speed6B;
			break;
		}
		case 6:
		{
			FGPIOA_PSOR = Speed1B | Speed3B | Speed4B;
			FGPIOB_PSOR = Speed5B | Speed6B | Speed7B;
			break;
		}
		case 7:
		{
			FGPIOA_PSOR = Speed1B | Speed2B | Speed3B;
			break;
		}
		case 8:
		{
			FGPIOA_PSOR = Speed1B | Speed2B | Speed3B | Speed4B;
			FGPIOB_PSOR = Speed5B | Speed6B | Speed7B;
			break;
		}
		case 9:
		{
			FGPIOA_PSOR = Speed1B | Speed2B | Speed3B;
			FGPIOB_PSOR = Speed6B | Speed7B;
			break;
		}
	}

}

void Fault_LED(uint8_t IMD, uint8_t BMS)
{
	if(IMD != 0) {
		GPIOA_PSOR |= 1 << 27;
	}
	else {
		GPIOA_PCOR |= 1 << 27;
	}
	if(BMS != 0) {
		GPIOB_PSOR |= 1 << 26;
	}
	else {
		GPIOB_PCOR |= 1 << 26;
	}
	return;
}

void TorqV_LED(uint16_t Throttle_L, uint16_t Throttle_R)
{
	if(Throttle_L > Throttle_R) FGPIOB_PSOR = 1 << 4;
	if(Throttle_R > Throttle_L) FGPIOB_PSOR = 1 << 5;
	return;
}

#endif
