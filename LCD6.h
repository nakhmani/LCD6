/*
  LCD6.h - Library for MSP430FR6989 built-in LCD display.
  Created by Arie Nakhmani, July 24, 2015.
*/
#ifndef LCD6_h
#define LCD6_h

#define TX   	 0
#define RX		 1
#define ANT		 2
#define HRT		 3
#define EXCL	 4
#define TMR		 5
#define REC		 6
#define NEG		 7
#define DP1		 8
#define DP2		 9
#define DP3		10
#define DP4	    11
#define DP5 	12
#define A2COL	13
#define A4COL	14
#define DEG		15

class LCD6
{
  public:
    LCD6(int scrollspeed);
    void print(int position, char symbol);
	void print(double number);
	void print(char text[6]);
	void clear();
	void specialsymbol(int symbol, bool state);
  private:
    bool _scrollspeed;
	bool _leftalign;
};

#endif
