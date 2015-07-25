/*
LCD6.h - Library for MSP430FR6989 built-in LCD display.
Created by Arie Nakhmani, July 24, 2015.
*/

#include "LCD6.h"
#include "msp430.h"

const unsigned char lcd_sym[36][2] = {
	{0xFC, 0x28},  /* "0" LCD segments a+b+c+d+e+f+k+q */
	{0x60, 0x20},  /* "1" */
	{0xDB, 0x00},  /* "2" */
	{0xF3, 0x00},  /* "3" */
	{0x67, 0x00},  /* "4" */
	{0xB7, 0x00},  /* "5" */
	{0xBF, 0x00},  /* "6" */
	{0xE4, 0x00},  /* "7" */
	{0xFF, 0x00},  /* "8" */
	{0xF7, 0x00},  /* "9" */
	{0xEF, 0x00},  /* "A" LCD segments a+b+c+e+f+g+m */
	{0xF1, 0x50},  /* "B" */
	{0x9C, 0x00},  /* "C" */
	{0xF0, 0x50},  /* "D" */
	{0x9F, 0x00},  /* "E" */
	{0x8F, 0x00},  /* "F" */
	{0xBD, 0x00},  /* "G" */
	{0x6F, 0x00},  /* "H" */
	{0x90, 0x50},  /* "I" */
	{0x78, 0x00},  /* "J" */
	{0x0E, 0x22},  /* "K" */
	{0x1C, 0x00},  /* "L" */
	{0x6C, 0xA0},  /* "M" */
	{0x6C, 0x82},  /* "N" */
	{0xFC, 0x00},  /* "O" */
	{0xCF, 0x00},  /* "P" */
	{0xFC, 0x02},  /* "Q" */
	{0xCF, 0x02},  /* "R" */
	{0xB7, 0x00},  /* "S" */
	{0x80, 0x50},  /* "T" */
	{0x7C, 0x00},  /* "U" */
	{0x0C, 0x28},  /* "V" */
	{0x6C, 0x0A},  /* "W" */
	{0x00, 0xAA},  /* "X" */
	{0x00, 0xB0},  /* "Y" */
	{0x90, 0x28}   /* "Z" */
};

LCD6::LCD6(int scrollspeed)
{
	_scrollspeed = scrollspeed;

	PJSEL0 = BIT4 | BIT5;                   // For LFXT

	// Initialize LCD segments 0 - 21; 26 - 43
	LCDCPCTL0 = 0xFFFF;
	LCDCPCTL1 = 0xFC3F;
	LCDCPCTL2 = 0x0FFF;

	// Disable the GPIO power-on default high-impedance mode
	// to activate previously configured port settings
	PM5CTL0 &= ~LOCKLPM5;

	// Configure LFXT 32kHz crystal
	CSCTL0_H = CSKEY >> 8;                  // Unlock CS registers
	CSCTL4 &= ~LFXTOFF;                     // Enable LFXT
	do
	{
		CSCTL5 &= ~LFXTOFFG;                  // Clear LFXT fault flag
		SFRIFG1 &= ~OFIFG;
	}while (SFRIFG1 & OFIFG);               // Test oscillator fault flag
	CSCTL0_H = 0;                           // Lock CS registers

	// Initialize LCD_C
	// ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
	LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;

	// VLCD generated internally,
	// V2-V4 generated internally, v5 to ground
	// Set VLCD voltage to 2.60v
	// Enable charge pump and select internal reference for it
	LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;

	LCDCCPCTL = LCDCPCLKSYNC;               // Clock synchronization enabled

	LCDCMEMCTL = LCDCLRM;                   // Clear LCD memory
}

void LCD6::clear()
{
	LCDCMEMCTL = LCDCLRM;                   // Clear LCD memory
}

void LCD6::print(int position, char symbol)
{
	if ((symbol >= 97) & (symbol <= 122)){ //  'a'-->10
		symbol -= 87;
	}
	if ((symbol >= 65) & (symbol <= 90)){ //  'A'-->10
		symbol -= 55;
	}
	if ((symbol >= 48) & (symbol <= 57)){ //if number...
		symbol -= 48;
	}
	switch (position){
	  case 1:
		  LCDM10 = lcd_sym[symbol][0];
		  LCDM11 = lcd_sym[symbol][1];
		  break;

	  case 2:
		  LCDM6 = lcd_sym[symbol][0];
		  LCDM7 = lcd_sym[symbol][1];
		  break;

	  case 3:
		  LCDM4 = lcd_sym[symbol][0];
		  LCDM5 = lcd_sym[symbol][1];
		  break;

	  case 4:
		  LCDM19 = lcd_sym[symbol][0];
		  LCDM20 = lcd_sym[symbol][1];
		  break;

	  case 5:
		  LCDM15 = lcd_sym[symbol][0];
		  LCDM16 = lcd_sym[symbol][1];
		  break;

	  case 6:
		  LCDM8 = lcd_sym[symbol][0];
		  LCDM9 = lcd_sym[symbol][1];
		  break;
	  default:
		  break;
	}

	//Turn LCD on
	LCDCCTL0 |= LCDON;

	__bis_SR_register(LPM3_bits | GIE);
	__no_operation();
}

/*void LCD6::print(long int number){
// show the last 6 digits 
bool NEGsign = (number < 0);

if (number == 0){
LCD6::print(6,48); //print zero
}

if (number < 0){
number = -number;
}

//show digits from left to right
int position = 6;
while ((number != 0) & (position > 0))
{
LCD6::print(position,(number % 10)+48);
number /= 10;
position--;

}
LCDM11 ^= (-NEGsign ^ LCDM11) & (1 << 2); //add NEG 


//Turn LCD on
LCDCCTL0 |= LCDON;

__bis_SR_register(LPM3_bits | GIE);
__no_operation();
}*/

void LCD6::print(double number){
	if(((long int)number-number)==0){
		long int num = (long int)number;
		bool NEGsign = (num < 0);

		if (number == 0){
			LCD6::print(6,48); //print zero
		}

		if (num < 0){
			num = -num;
		}

		//show digits from left to right
		int position = 6;
		while ((num != 0) & (position > 0))
		{
			LCD6::print(position,(num % 10)+48);
			num /= 10;
			position--;

		}
		LCDM11 ^= (-NEGsign ^ LCDM11) & (1 << 2); //add NEG 


		//Turn LCD on
		LCDCCTL0 |= LCDON;

		__bis_SR_register(LPM3_bits | GIE);
		__no_operation();
	}
	else{
		bool NEGsign = (number < 0);

		if (number < 0){
			number = -number;
		}

		int buffer[6] = {0,0,0,0,0,0};
		//find all digits before decimal point
		double mult = 100000;
		int cnt = 0;
		for(int t=0; t<6;t++){
			if((number/mult >= 1) | (cnt>0)){
				buffer[cnt] = (int)(number/mult);
				number = number-buffer[cnt]*mult;
				cnt++;
			}
			mult /= 10;
		}

		// find all digits after decimal point
		for(int t=cnt; t<6;t++){
			number *= 10;
			buffer[t] = (int)number;
			number = number-buffer[t];
		}


		//show digits from left to right
		for(int t=0; t<6; t++){
			LCD6::print(t+1,buffer[t]+48);
		}

		switch (cnt){ //put decimal point
		case 1:
			LCD6::specialsymbol(DP1,true);
			break;

		case 2:
			LCD6::specialsymbol(DP2,true);
			break;

		case 3:
			LCD6::specialsymbol(DP3,true);
			break;

		case 4:
			LCD6::specialsymbol(DP4,true);
			break;

		case 5:
			LCD6::specialsymbol(DP5,true);
			break;

		}


		LCDM11 ^= (-NEGsign ^ LCDM11) & (1 << 2); //add NEG 


		//Turn LCD on
		LCDCCTL0 |= LCDON;

		__bis_SR_register(LPM3_bits | GIE);
		__no_operation();
	}
}

void LCD6::print(char text[6]){
	// show text
	int t = 1;
	while((t<7) & (text[t-1]!='\0')){
		//for(int t=1; t<7;t++){
		LCD6::print(t,text[t-1]);
		t++;
	}


	//Turn LCD on
	LCDCCTL0 |= LCDON;

	__bis_SR_register(LPM3_bits | GIE);
	__no_operation();
}

void LCD6::specialsymbol(int symbol, bool state)
{
	switch (symbol){
	  case 0:
		  LCDM9 ^= (-state ^ LCDM9) & (1 << 2);
		  break;

	  case 1:
		  LCDM9 ^= (-state ^ LCDM9) & (1 << 0);
		  break;

	  case 2:
		  LCDM5 ^= (-state ^ LCDM5) & (1 << 2);
		  break;

	  case 3:
		  LCDM3 ^= (-state ^ LCDM3) & (1 << 2);
		  break;

	  case 4:
		  LCDM3 ^= (-state ^ LCDM3) & (1 << 0);
		  break;

	  case 5:
		  LCDM3 ^= (-state ^ LCDM3) & (1 << 3);
		  break;

	  case 6:
		  LCDM3 ^= (-state ^ LCDM3) & (1 << 1);
		  break;

	  case 7:
		  LCDM11 ^= (-state ^ LCDM11) & (1 << 2);
		  break;

	  case 8:
		  LCDM11 ^= (-state ^ LCDM11) & (1 << 0);
		  break;

	  case 9:
		  LCDM7 ^= (-state ^ LCDM7) & (1 << 0);
		  break;

	  case 10:
		  LCDM5 ^= (-state ^ LCDM5) & (1 << 0);
		  break;

	  case 11:
		  LCDM20 ^= (-state ^ LCDM20) & (1 << 0);
		  break;

	  case 12:
		  LCDM16 ^= (-state ^ LCDM16) & (1 << 0);
		  break;

	  case 13:
		  LCDM7 ^= (-state ^ LCDM7) & (1 << 2);
		  break;

	  case 14:
		  LCDM20 ^= (-state ^ LCDM20) & (1 << 2);
		  break;

	  case 15:
		  LCDM16 ^= (-state ^ LCDM16) & (1 << 2);
		  break;

	  default:
		  break;

	}

	//Turn LCD on
	LCDCCTL0 |= LCDON;

	__bis_SR_register(LPM3_bits | GIE);
	__no_operation();

}
