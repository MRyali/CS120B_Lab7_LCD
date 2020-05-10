/*	Author: Mayur Ryali
 *  Partner(s) Name:
 *	Lab Section: 21
 *	Assignment: Lab #7 Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

// Demo: https://drive.google.com/open?id=10B8TpJxC_qLaQbtFya1L92sbIMxeI4pU

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "io.h"
#endif

enum States{Start, light1, light2, light3, light4, wait, release, reset} state;
unsigned char tempB = 0x00; //temp for Port B
unsigned char button; //input from PA0
unsigned char s = 0; //tracks if previous state was reset or not
unsigned char tmp = 1;
unsigned char score = 5;

void Tick() {
	switch(state){
		case Start:
			state = light1;
			break;
		case light1: //PB0
			if (!button) {
				tmp = 1;
			}
			if (button && s == 0 && tmp == 1) { //previous state was NOT rest
				state = wait;
				if (score > 0) {
					score--;
				}
			}
			else if (button && s == 1) { //previous state WAS reset
				tmp = 0;
				s = 0; //reset tracker
				state = light2;
			}
			else {
				s = 0;
				state = light2;
			}
			break;
		case light2: //PB1
			if (!button) {
				tmp = 1;
			}
			if (button && tmp == 1) {
				state = wait;
				if (score < 9) {
					score++;
				}
			}
			else {
				state = light3;
			}
			break;
		case light3: //PB2
			if (!button) {
				tmp = 1;
			}
			if (button && tmp == 1) {
				state = wait;
				if (score > 0) {
					score--;
				}
			}
			else {
				state = light4;
			}
			break;
		case light4:
			if (!button) {
				tmp = 1;
			}
			if (button && tmp == 1) {
				state = wait;
				if (score < 9) {
					score++;
				}
			}
			else {
				state = light1;
			}
			break;
		case wait:
			if (!button) {
				state = release;
			}
			else {
				state = wait;
			}
			break;
		case release:
			if (button) {
				state = reset;
			}
			else {
				state = release;
			}
			break;
		case reset:
		       	tmp = 1;
			s = 1;
			state = light1;
			break;
		default:
			state = Start;
			break;
	}

	switch(state) {
		case Start:
			break;
		case light1: //PB0
			tempB = 0x01;
			break;
		case light2: //PB1
			tempB = 0x02;
			break;
		case light3: //PB2
			tempB = 0x04;
			break;
		case light4:
			tempB = 0x02;
			break;
		case wait:
			break;
		case release:
			break;
		case reset:
			break;
		default:
			break;

	}
	/*
	if (score == 9) {
                LCD_DisplayString(1, "You win");
        }
        else {
         	LCD_WriteData(score + '0');
        }
	*/
}


int main(void) {
	DDRA = 0x00; PORTA = 0xFF; //input
	DDRB = 0xFF; PORTB = 0x00; //output
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	LCD_init();
	LCD_ClearScreen();

	TimerSet(300); //300ms = 1s
	TimerOn();

	state = Start;
	score = 5;

	while (1) {
		LCD_Cursor(1);
		button = ~PINA & 0x01; //PA0

		Tick();

		if (score == 9) {
			LCD_ClearScreen();
			LCD_DisplayString(1, "You win");
		}
		else {
			LCD_ClearScreen();
			LCD_WriteData(score + '0');
		}

		while(!TimerFlag) {}
		TimerFlag = 0;

		PORTB = tempB;
   	}

    	return 1;
}
