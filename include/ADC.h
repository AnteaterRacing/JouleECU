/* ADC.h              (c) 2015 Freescale Semiconductor, Inc.
 * Descriptions: ADC functions for conversion example
 * 28 Sept 2015 Pedro Aguayo: Initial version
 */

#ifndef ADC_H_
#define ADC_H_
#include "main.h"

void ReadAdcBlock(void);
void init_ADC(void);
void ADC_Handler(void);
void ADC_Converter(void);
void ADC0_IRQHandler(void);
uint8_t read_adc_chx(void);
void convertAdcChan(uint16_t adcChan);
uint8_t adc_complete(void);


uint8_t ADC_buf[16];

#ifdef AuxECU
#define ADC_SC1_ADCH10_MASK 0x0A
#define ADC_SC1_ADCH11_MASK 0x0B
#define ADC_SC1_ADCH12_MASK 0x0C
#define ADC_SC1_ADCH13_MASK 0x0D
#define ADC_SC1_ADCH14_MASK 0x0E
#define ADC_SC1_ADCH15_MASK 0x0F
#endif

#define ADC_SC1_ADCH0_MASK 0x00
#define ADC_SC1_ADCH1_MASK 0x01
#define ADC_SC1_ADCH2_MASK 0x02
#define ADC_SC1_ADCH3_MASK 0x03
#define ADC_SC1_ADCH4_MASK 0x04
#define ADC_SC1_ADCH5_MASK 0x05
#define ADC_SC1_ADCH6_MASK 0x06
#define ADC_SC1_ADCH7_MASK 0x07
#define ADC_SC1_ADCH8_MASK 0x08
#define ADC_SC1_ADCH9_MASK 0x09



#endif /* ADC_H_ */
