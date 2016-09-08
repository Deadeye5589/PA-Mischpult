/*
 * dsp.h
 *
 * Created: 03.12.2014 13:15:49
 *  Author: kampch2
 */ 


#ifndef DSP_H_
#define DSP_H_


//Defines of PPS mapped registers

#define muxreg 0x23				//Input Multiplex
#define volreg 0x24				//Master Volume Regulator
#define vuenbl 0x42				//VU Meter Register
#define specenbl 0x47			//Spectrum Analyzer Register



//Function declarations

void switchdspinput(uint8_t int_channel, uint8_t ext_channel);
uint8_t readmastervolume(void);
void setmastervolume(uint8_t level);
void vumeter(unsigned char update_on);
void spectrum(unsigned char update_on);
void mute(unsigned char mute_on_off);
void volumeup(void);
void volumedown(void);



#endif /* DSP_H_ */