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
int spawn2 = 0;
int scoreCount = 0;
int score = 0;

int start = 1;

char Start1[] = "Choose";
char Start2[] = "Difficulty";
char Start3[] = "L-Start, R-Diff";
int difficulty = 1;
char difficultyShown[1] = "1";

char gameOver[] = "Game over";
char gameOver2[] = "L to restart";
int gameovercheck = 0;

char clear[] = " ";

/* Interrupt Service Routine */
void user_isr(void) {
	if((IFS(0) & 0x0100)){
		IFS(0) = IFS(0) & 0xfffffeff;
		timecount++;
		spawnflag++;
		scoreCount++;
		
		while (gameovercheck){
			T2CONCLR = 0x8000;
			display_string( 1, gameOver );
			display_string( 2, gameOver2 );
			display_update();	
			if(getbtns() == 0x04){
				*porte = *porte - score;
				int i = 0;
				score = 0;
				spawnflag = 0;
				pos = 0;
				for(i = 0; i < 512; i++){
				Screen[i] = ScreenRestart[i];
				}
				start = 1;
				gameovercheck = 0;
				delay(300);
				T2CONSET = 0x8000;
			}
		}
		while(start) {
		T2CONCLR = 0x8000;
		difficultyShown[0] = difficulty + '0';
		
		display_string( 0, Start1 );
		display_string( 1, Start2 );
		display_string( 2, difficultyShown);
		display_string( 3, Start3 );
		display_update();	
		
		if(getbtns() == 0x04){
			display_string( 0, clear );
			display_string( 1, clear );
			display_string( 2, clear );
			display_string( 3, clear );
			
			start = 0;
			T2CONSET = 0x8000;
			break;
		}
		
		if(getbtns() == 0x02){
			if(difficulty < 4){
				difficulty++;
			}
			else{
				difficulty = 1;
			}
			delay( 175 );
		}
	}
		
	}

	if(timecount == 5){
		display_image(0, Screen);
		timecount = 0;
	}
	
	if(spawnflag == 6/difficulty){
		if(spawn2 == 2) {
			obstaclespawn(rand);
			spawn2 = 0;
		}
		spawn2++;
		spawnflag = 0;
		gameovercheck = checkForCrash(pos);
	}
	if(scoreCount == 15){
		*porte = *porte + 1;
		score++;
		scoreCount = 0;
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
				rand++;
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
	rand++;
	if(rand > 3)
		rand = 0;
	
}


