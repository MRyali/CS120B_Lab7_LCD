/*	Author: Mayur Ryali
 *  Partner(s) Name:
 *	Lab Section: 21
 *	Assignment: Lab #7 Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

 // Demo: https://drive.google.com/open?id=1-H2FFMoAWYfxVfR9-F5Kz63ZMj1wJPfE
 
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "io.h"
#endif

enum States {Start, Inc, Dec, Zero, Wait} state;

unsigned char tempB = 0; //temp for PORTB
unsigned char timer = 0; //timer
unsigned char button0; //PA0
unsigned char button1; //PA1

void Tick() {
	switch(state) {
		case Start:
			state = Wait;
			break;
		/*
		case Init:
			if (button0 && !button1) { //check if PA0 is 1
				state = Inc;
			}
			else if (!button0 && button1) { //check if PA1 is 1
				state = Dec;
			}
			else if (button0 && button1) { //check if both are pressed
				state = Zero;
			}
			else { //wait for button push
				state = Init;
			}
			break;
		*/
		case Inc:
			if (button1 && button0) {//check if both pressed
				state = Zero;
			}
			else if (!button0) { //wait for new action
				state = Wait;
			}
			else {
				state = Inc;
			}
			break;
		case Dec:
			if (button0 && button1) {//check if both pressed
				state = Zero;
			}
			else if (!button1){ //wait for new action
				state = Wait;
			}
			else {
				state = Dec;
			}
			break;
		case Zero: //check which button is pushed during zero state
			if ((!button0) && (!button1)) {
				state = Wait;
			}
			else {
				state = Zero;
			}
			break;
		case Wait: //wait for new action
			if (button0) {
				state = Inc;
			}
			else if (button1) {
				state = Dec;
			}
			else {
				state = Wait;
			}
			break;
		default:
			state = Start;
			break;
	}

	switch(state) {
		case Start:
			break;
		/*
		case Init:
			timer = 0;
			break;
		*/
		case Inc: //increase while temp is below 9
			if (timer == 0 && tempB < 9) {
				tempB++;
				timer++;
			}
			timer++;
			if (timer == 10) { //10 sec
				if (tempB < 9) {
					tempB++;
					timer = 1;
				}
			}/*
			if (tempB < 9) {
				tempB++;
			}*/
			break;
		case Dec: //decrease while temp is greater than 0
			if (timer == 0 && tempB > 0) {
				tempB--;
				timer++;
			}
			timer++;
			if (timer == 10) { //10 sec
				if (tempB > 0) {
					tempB--;
					timer = 1;
				}
			}/*
			if (tempB > 0) {
				tempB--;
			}*/
			break;
		case Zero: //set to zero
			tempB = 0;
			timer = 0;
			break;
		case Wait:
			timer = 0;
			break;
		default:
			break;
	}
}



int main(void) {
	DDRA = 0x00; PORTA = 0xFF; //input
	DDRB = 0xFF; PORTB = 0x00; //output
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	LCD_init();
	LCD_ClearScreen();

	TimerSet(100); //10ms
	TimerOn();

	state = Start;
	tempB = 0x00; //initially start with 7

    	while (1) {
		LCD_Cursor(1);
		button0 = ~PINA & 0x01; //PA0, negated to account for pull-up mode
		button1 = ~PINA & 0x02; //PA1, negated for pull-up mode
		
		Tick();

		LCD_WriteData(tempB + '0');

		while(!TimerFlag);
		TimerFlag = 0;

		PORTB = tempB;
    	}
    	return 1;
}
