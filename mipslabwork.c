/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 
   This file modified 2018-02-xx by Oscar Eklund and Vilhelm Elofsson
   For copyright and licensing, see file COPYING */
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

volatile int * porte = (volatile int *) 0xbf886110;
int timecount = 0;
int spawnflag = 0;
int pos = 0;
int rand;
/* Interrupt Service Routine */
void user_isr(void) {
	if((IFS(0) & 0x0100)){
		IFS(0) = IFS(0) & 0xfffffeff;
		timecount++;
		spawnflag++;
	}

		if(timecount == 2){
			display_image(0, Screen);
			timecount = 0;
	}
	
		if(spawnflag == 20){
			obstaclespawn(rand);
			spawnflag = 0;
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
	IEC(0) = 0x100;
	IPC(2) = 0x7;
	enable_interrupt();
	return;
}
/* This function is called repetitively from the main program */
void labwork(void) {
	
	switch(getbtns()) { 
		case 0x04  :
			if(pos > 0){
				pos--;
				moveup(pos);
				delay( 150 );
			}
			
			break;
		case 0x02  :
			if(pos < 3){
				pos++;
				movedown(pos);
				delay( 150 );
			}
			break;
	}
	if(rand > 3)
	{rand = 0;}
			rand++;
}


