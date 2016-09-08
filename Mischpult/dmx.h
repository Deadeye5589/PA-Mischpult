/*
 * dmx.h
 */ 


#ifndef DMX_H_
#define DMX_H_


#define dmxchannels 512

void dma_set(unsigned char *dmxdata, unsigned int length);
void start_dmx(unsigned char *dmxdata, unsigned int length);
void usart_init(void);

void dmx_internal(void);
void dmx_external(void);
void dmx_s2l(void);
void dmx_worklight(void);

void flash_white(void);
void flash_puke(void);
void flash_color(void);

void solid_color(void);

void level_brightness(unsigned char slider);
void level_red(unsigned char slider);
void level_blue(unsigned char slider);
void level_green(unsigned char slider);
void frequency(unsigned char slider);

enum{BREAK, STARTBYTE, DMXDATA, FIRSTBREAK};
unsigned char DMXStatus;

extern unsigned char global_brightness;
extern unsigned char global_red;
extern unsigned char global_blue;
extern unsigned char global_green;
extern unsigned char global_frequency;	

extern _Bool toogle_red;
extern _Bool toogle_green;
extern _Bool toogle_blue;
extern _Bool toogle_solid;
extern _Bool toogle_worklight;

#endif /* DMX_H_ */