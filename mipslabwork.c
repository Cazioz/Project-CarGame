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
int timeoutcount = 0;
int test = 0;
int k = 0;
/* Interrupt Service Routine */
void user_isr(void) {
	if(IFS(0) & 0x100) {
		
		timeoutcount++;
		if((getbtns() >> 2) == 0x1){
			k = ~k;
		}
		if(k){
			test= test - 2;
			display_update();
		display_image(test, block2);
		display_image(test + 32, block1);
		}
		//*porte = *porte + 1;
		
		IFSCLR(0) = 0x100;
	}
	if (IFS(0) & 0x80){
	*porte = *porte + 1;
	IFSCLR(0) = 0x80;
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
	
 //display_update();
}


