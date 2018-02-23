/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

volatile int * porte = (volatile int *) 0xbf886110;
int test = 0;
int flag = 0;
int i = 0;
int click = 0;
/* Interrupt Service Routine */
void user_isr(void) {
	if((IFS(0) & 0x0100)){
		IFS(0) = IFS(0) & 0xfffffeff;
		flag++;
	}
	
		if(flag == 10){
			flag = 0;
		
	}
	
	
}


/* Lab-specific initialization goes here */
void labinit(void)
{
	volatile int* trise = (volatile int *) 0xbf886100; // pointer to TRISE address
	*trise = *trise & 0xff00; // set bits 7-0 of PORT E to outputs
	*porte = 0x0; // initialize to 0
	TRISDSET = 0xfe0; // set bit 11-5 of PORT D as output
	
	PR2 = 31250;
	T2CON = 0x70; // sets prescale to 256
	TMR2 = 0;
	T2CONSET = 0x8000; // set bit 15 to 1 (enable timer)
	IEC(0) = 0x180;
	IPC(2) = 0x7;
	IPC(1) = 0x1c000000;
	enable_interrupt();
	return;
}

/* This function is called repetitively from the main program */
void labwork(void) {
	switch(getbtns()) { 
		
		case 0x04  :
			
			if(click > 0){
				delay( 200 );
				click--;
			}
			
			break;

		case 0x02  :
		
			if(click < 4){
				delay( 200 );
				click++;
			}
			
			break;
		
	}
	
	switch(click) { 
			
			case 4  :
			
				display_update();
				display_image(0, block4);
				
				break;

			case 2  :
			
				display_update();
				display_image(0, block3);
				
				break;
				
			case 1  :
			
				display_update();
				display_image(0, block2);
				
				break; 
				
			case 0  :
				
				
				display_update();
				display_image(0, block1);
				
				break;
			
		}
	display_update();
}


