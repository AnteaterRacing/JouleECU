/*
 * CAN.h
 *
 *  Created on: Feb 8, 2018
 *      Author: arnav
 */

#ifndef CAN_H_
#define CAN_H_


//defining the ECU number to change CAN parameters when initializing.
#define Front_ECU_CAN_Init 1
#define Rear_ECU_CAN_Init 0

//defining Message ID for front to rear ECU message transmit
//size: 6 bytes: Accelerator, BrakeAngle, SteeringAngle, TVEnable, StartButton, FrontFault
#define FrontToRearDataMessageID 10
#define FrontToRearDataMessageSize 6
//defining message ID for telemetry signals
//size: 8 bytes: Wheelspeed L, Wheelspeed R, Tire temps L1,L2,L3,R1,R2,R3
#define FrontToRearTelemetryMessageID 20
#define FrontToRearTelemetryMessageSize 8

//defining Message ID for rear to front ECU message transmit
//size: 7 bytes: IMDFault,BMSFault,BSPDFault, Speedometer, TractionLED, TV_LED, MotorTempLED
#define RearToFrontDataMessageID 11
#define RearToFrontDataMessageSize 7


//TODO add message ID definitions for BMS


#define Accelerator 	1
#define BrakeAngle  	2
#define SteeringAngle	3
#define TVEnable		4
#define	StartButton		5
#define FrontFault		6

#define WheelSpeed_L	1
#define WheelSpeed_R	2
#define TireTemp_L1		3
#define TireTemp_L2		4
#define TireTemp_L3		5
#define TireTemp_R1		6
#define TireTemp_R2		7
#define TireTemp_R3		8

#define IMDFault		1
#define BMSFault		2
#define BSPDFault		3
#define Speedometer		4
#define TractionLED		5
#define	TV_LED			6
#define MotorTempLED	7


void CAN_Init(uint8_t ecu); //front or rear ecu init
void CAN_Transmit(uint16_t messageID, uint8_t* message);
void CAN_Receive(uint16_t messageID, uint8_t* message);
uint8_t startSignal(void); //returns 0 if no start message received. 1 if received
void delay(void);

#endif /* CAN_H_ */
