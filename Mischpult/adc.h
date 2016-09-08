/*
 * adc.h
 *
 * Created: 06.08.2015 08:38:22
 *  Author: kampch2
 */ 


#ifndef ADC_H_
#define ADC_H_

#define BYTES 8;

#include <avr/io.h>

void ADCA_init();
unsigned int ADCA_Conversion(ADC_CH_t *Channel, char Pin);
int LeseKalibrationsbyte(int Index);
void ADCA_Cal(void);
unsigned int get_seed(unsigned char bit_count);
signed int get_rand(signed int min, signed int max);



#endif /* ADC_H_ */