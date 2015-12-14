/*
 * com-module.c
 *
 * Created: 2015-11-06 10:54:37
 * Author : Jacob 
 */
 

#define F_CPU 15000000UL    // CPU clock frequency is 15MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "i2c_slave.h"  // Com module is a Slave on the I2C-bus.
#include "bluetooth.h"  // Add bluetooth compatibility.

/******** Sine wave parameters ********/
#define PI2 6.283185 // 2*PI saves calculation later
#define AMP 255 // Scaling factor for sine wave
#define OFFSET 128 // Offset shifts wave to all >0 values

/******** tone frequenzy *****/
#define D 106
#define E 95
#define Fi 84
#define G 80
#define A 71
#define B 63
#define ci 56
#define d 53
#define e 47
#define fi 42
#define g 40
#define a 36
#define b 32
#define ci4 28
#define d4 27
#define e4 24
#define fi4 20
#define g4 20

/******** tone legnth ********/
#define fj 280  //fjärdedel
#define ei 130  //eigth

/******** Lookup table ********/
#define LENGTH 256 // Length of the wave lookup table
static int wave[LENGTH]; // Storage for waveform

// My ding ding dong
#define songLength 26
int song[songLength][2] = {
	{d4,fj}, // 0
	{b,ei},  // LA
	{ci4,ei}, // LA
	{d4,fj},
	{b,ei},
	{ci4,ei},
	{d4,fj},  //6
			
	{b,ei}, // 7
	{ci4,ei},
	{d4,ei},
	{fi4,ei},
	{e4,ei},
	{d4,ei},
	{ci4,fj}, // 13
			
	{a,ei}, // 14
	{b,ei},
	{ci4,fj},
	{a,ei},
	{b,ei},
	{ci4,fj},  //19
			
	{a,ei}, //20
	{b,ei},
	{ci4,ei},
	{e4,ei},
	{d4,ei},
	{ci4,ei}  //25
};

// Delay en fjärdedelsnot
 void delay_fj(){
	 _delay_ms(fj);
 }
 
// Delay en åttondelsnot
 void delay_ei(){
	 _delay_ms(ei);
 }
 
void pauseSong(){
	TCCR3B |= (0 << CS31);    // Set prescaler to full 16MHz
	TCCR2B |= (0 << CS21); // Set prescaler to divide by 8
}
 
 static int initSong = 0;
 static int s = 0;
// plays the song
void playSong()
{

	if(initSong == 0){
		initSong = 1;
		
		/* Populate the waveform table with a square wave */
		for (int i=0; i<LENGTH/2; i++) { // Step across wave table
			wave[i] = AMP;
		}

		/****Set timer1 for 8-bit fast PWM output ****/
		DDRB = 0xFF;			  // Set output
		TCCR3B |= (1 << CS31);    // Set prescaler to full 16MHz
		TCCR3A |= (1 << COM3B1);  // Pin low when TCNT1=OCR1A
		TCCR3A |= (1 << WGM30);   // Use 8-bit fast PWM mode
		TCCR3B |= (1 << WGM32);
	
		/******** Set up timer2 to call ISR ********/
		TCCR2A = 0; // No options in control register A
		TCCR2B |= (1 << CS21); // Set prescaler to divide by 8
		TIMSK2 |= (1 << OCIE2A); // Call ISR when TCNT2 = OCRA2
	
		sei(); // Enable interrupts to generate waveform!
	}
	
	while(1){
		OCR2A = song[s][0]; // Set frequency of generated wave			
		if(song[s][1] == fj){
			delay_fj();
		} else if (song[s][1] == ei){
			delay_ei();
		}
		s++;
	}
}

uint8_t i=0;
/******** Called every time TCNT2 = OCR2A ********/
ISR(TIMER2_COMPA_vect) { // Called when TCNT2 == OCR2A
	OCR3B = wave[i++]; // Update the PWM output
	TCNT2 = 6; // Timing to compensate for ISR run time
}
