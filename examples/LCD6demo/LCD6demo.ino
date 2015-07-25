#include <LCD6.h>

LCD6 lcd(0);
void setup()
{
  // put your setup code here, to run once:
  
  lcd.specialsymbol(ANT,HIGH);
  lcd.specialsymbol(TX,HIGH);
  lcd.specialsymbol(RX,HIGH);
  lcd.specialsymbol(HRT,HIGH);
  lcd.specialsymbol(EXCL,HIGH);
  lcd.specialsymbol(TMR,HIGH);
  lcd.specialsymbol(REC,HIGH);
  lcd.specialsymbol(NEG,HIGH);
  lcd.specialsymbol(DP1,HIGH);
  lcd.specialsymbol(DP2,HIGH);
  lcd.specialsymbol(DP3,HIGH);
  lcd.specialsymbol(DP4,HIGH);
  lcd.specialsymbol(DP5,HIGH);
  lcd.specialsymbol(A2COL,HIGH);
  lcd.specialsymbol(A4COL,HIGH);
  lcd.specialsymbol(DEG,HIGH);
  delay(1000);
  lcd.clear();
  lcd.print(-125);
  delay(1000);
  lcd.clear();
  lcd.print(46.929);
  delay(1000);
  lcd.clear();
  lcd.print("Hi");
  delay(1000);
  lcd.clear();
  lcd.print("World"); 
}

void loop()
{
  // put your main code here, to run repeatedly:
  
}
