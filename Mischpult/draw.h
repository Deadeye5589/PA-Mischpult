/*
 * draw.h
 *
 * Created: 03.12.2014 13:11:03
 *  Author: kampch2
 */ 



#ifndef DRAW_H_
#define DRAW_H_

#include <avr/io.h>

_Bool draw_basic_frame(uint8_t menu);
_Bool draw_audio_menu(void);
_Bool draw_dmx_menu(void);
_Bool draw_setup_menu(void);
_Bool showlevel(uint8_t level);
_Bool update_levels(void);
_Bool show_mute(void);
_Bool clear_area(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

extern unsigned char pushed_audio;			//Global variable to remember which button for source selection was pressed
extern unsigned char  pushed_dmx;			//Global variable to remember which button for source selection was pressed
extern unsigned char  selected_menu;			//Global variable to remember which menu was selected last
extern unsigned char  mute_selected;

extern unsigned char selected_red;
extern unsigned char selected_green;
extern unsigned char selected_blue;
extern unsigned char selected_solid_color;
extern unsigned char selected_work_light;

#endif /* DRAW_H_ */