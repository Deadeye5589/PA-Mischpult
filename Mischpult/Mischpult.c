/*
 * Mischpult.c
 *
 */ 


#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "avr_compiler.h"
#include "twi_master_driver.h"
#include "display.h"
#include "draw.h"
#include "dsp.h"
#include "dmx.h"
#include "adc.h"

uint8_t i = 0;




/*
 * Clock_init
 * Select external 16 MHz crystal for clock source
 */ 
void Clock_init(void)
{
	//Xosc Bit 7&6 to 11 | Bit 3 to 0 as 1011
	OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
	//Bit 3 to 1 to enable external oscillator
	OSC.CTRL |= OSC_XOSCEN_bm;
	//wait for stable clock
	while(!(OSC.STATUS & OSC_XOSCRDY_bm));
	//Disable register protection for 4 clocks
	CCP = CCP_IOREG_gc;
	//Set Bit 2:0 to 011 for external oscillator
	CLK.CTRL = CLK_SCLKSEL_XOSC_gc;
}


/*
* PLL_init
* Double clock rate to 32 MHz via internal PLL
*/  
void PLL_init(void)
{
	//Bit 7:0 to 11 for external oscillator as PLL source | 2 as multiplication value
	OSC.PLLCTRL = OSC_PLLSRC_XOSC_gc | 0x02;
	//enable PLL
	OSC.CTRL |= OSC_PLLEN_bm;
	//wait for stable oscillation
	while (!(OSC.STATUS & OSC_PLLRDY_bm));
	//Disable register protection for 4 clocks
	CCP = CCP_IOREG_gc;
	//Set Bit 2:0 for PLL as clock source
	CLK.CTRL = CLK_SCLKSEL_PLL_gc;
}

int main(void)
{
	Clock_init();														//Start-Up external crystal
	PLL_init();															//Use PLL to generate 32 MHz tact from external crystal
	
	PORTA.DIR &= ~PIN1_bm;												//Pin A0 as Input for ADC
	PORTA.DIR |= PIN6_bm;												//Pin A6 as Output to enable Line-Out IC
	PORTC.DIR |= PIN5_bm;												//Pin C5 as Output to reset DSP
	PORTD.DIR |= PIN0_bm;												//Pin D0 as Output for audio switch 
	PORTD.DIR |= PIN1_bm;												//Pin D1 as Output for audio switch
	PORTD.DIR |= PIN5_bm;												//PIN D5 as Output for Error Display Comm LED
	PORTD.DIR &= ~PIN6_bm;												//Pin D6 as Input for Display SBUF Interrupt
	PORTD.DIR |= PIN7_bm;												//Pin D7 as Output
	PORTE.DIR |= PIN6_bm;												//Pin E6 as Output
	PORTE.DIR |= PIN7_bm;												//Pin E7 as Output
	
	PORTC.OUTCLR = PIN5_bm;												//Pin C5 to Low = Force Reset DSP
	PORTD.OUTCLR = PIN7_bm;												//Pin D7 to Low = Force Reset Display
	
	PORTE.OUTSET |= PIN6_bm;											//Port E to Output and High for routing Uart to SP3T Output
	
	usart_init();
	ADCA_init();
	
	
	/* Initialize TWI master. */
	TWI_MasterInit(&twiLCD, &TWIE, TWI_MASTER_INTLVL_LO_gc, TWI_BAUDSETTING);
	TWI_MasterInit(&twiDSP, &TWIC, TWI_MASTER_INTLVL_LO_gc, TWI_BAUDSETTING);
	
	/* Enable LO interrupt level. */
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	sei();


	_delay_ms(100);														//Wait a little bit longer

	
	PORTC.OUTSET = PIN5_bm;												//Pin C5 to High = Release DSP from reset
	
	_delay_ms(100);														//Wait again for voltage supply to stabilize
	
	PORTD.OUTSET = PIN7_bm;												//Pin D7 to High = Release display from reset
	
	_delay_ms(3000);													//Wait 3 seconds to give display time to boot up
	
	PORTA.OUTSET = PIN6_bm;												//Pin A6 to High = Enable Line-Out
	
	PORTE.OUTSET = PIN6_bm;												//Pin E6 as High 
	PORTE.OUTSET = PIN7_bm;												//Pin E7 as High
																		//Together switch DMX output to USB as Default
	
	
	draw_audio_menu();													//draw audio menu as default start-up page
	switchdspinput(3,3);												//select USB as default start-up audio source	
		

	while(1)															//now we can read buttons back
	{
		if(selected_menu==1)
		{																//Check if the audio menu is displayed
		vumeter(1);														//Read and update display VU Meter graphs
		spectrum(1);													//Read and update display spectrum graphs
		_delay_ms(75);													//Time between to updates of sound analysis
		}
		if (toogle_worklight)											
		{
			dmx_worklight();
		}
		else
		{
			solid_color();
		}
		if (!(PORTD.IN & (1<<6)))										//wait for SBUF signal from display to get low
		{
			
			if(checkbuf())			//check display transmission buffer
			{
				if(answerdecode())
				{
					CheckComm(1); //Display answers right
				}
				else
				CheckComm(0); //Display not answering
				
			}
		}
	}
}

/*! TWIE Master Interrupt vector. */
ISR(TWIE_TWIM_vect)
{
	TWI_MasterInterruptHandler(&twiLCD);
}


/*! TWIC Master Interrupt vector. */
ISR(TWIC_TWIM_vect)
{
	TWI_MasterInterruptHandler(&twiDSP);
}

ISR(DMA_CH0_vect)
{
	//clear interrupt flag
	DMA.CH0.CTRLB |= DMA_CH_TRNIF_bm;
	//Deactivate DMA
	DMA.CH0.CTRLA &= ~DMA_CH_ENABLE_bm;
	//Check for finished physical transmission and clear flag
	while (!(USARTC0.STATUS & USART_TXCIF_bm));
	USARTC0.STATUS |= USART_TXCIF_bm;
	//Go back to start of DMX protocol
	DMXStatus = BREAK;
}
