//#include <Time.h>
//#include <TimeLib.h>

/*
 Example Arduino sketch for
 SainSmart IIC/I2C/TWI Serial 2004 20x4 LCD Module Shield For Arduino UNO MEGA R3
 http://www.sainsmart.com/sainsmart-iic-i2c-twi-serial-2004-20x4-lcd-module-shield-for-arduino-uno-mega-r3.html
 
 and
 
 This example uses F Malpartida's NewLiquidCrystal library. Obtain from:
 https://bitbucket.org/fmalpartida/new-liquidcrystal 
 
 This example also uses Tennsy Time.h library. Obtain from:
 http://www.pjrc.com/teensy/td_libs_Time.html 
 
 Tested on:
 Arduino Pro Mini, Arduino Uno - A4 (SDA), A5 (SCL)
 Arduino Mega 2560 - 20 (SDA), 21 (SCL)
 
 Last modified: 16-Dec-2013
 
*/
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#include "scheduler.h"


// Creat a set of new characters
byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

byte armsUp[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00100,
  0b01010
};

byte frownie[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b10001
};


//#define I2C_ADDR      0x27 // I2C address of PCF8574A
#define I2C_ADDR      0x3F // I2C address of PCF8574A



#define BACKLIGHT_PIN 3
#define En_pin        2
#define Rw_pin        1
#define Rs_pin        0
#define D4_pin        4
#define D5_pin        5
#define D6_pin        6
#define D7_pin        7

//LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin, BACKLIGHT_PIN, POSITIVE);
LiquidCrystal_I2C lcd(I2C_ADDR, 2 ,1,0,4,5,6,7,3,POSITIVE);
//LiquidCrystal_I2C lcd(I2C_ADDR, 4, 5, 6, 0, 1, 2, 3, 7, NEGATIVE);
//////////////////////////////////////////////////////////////
#define TRIGGER_INIT_VALUE  0
#define INTERVAL_INIT_VALUE 1
#define INTERVAL_MAX_VALUE  512
#define EMBEDDED_LED_PIN    13
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
long interval = INTERVAL_INIT_VALUE;
byte trigger;
byte ledState = LOW;

int  backlight (void);
int backlightOn=1;
SimpleTask taskBlink(backlight);

//////////////////////////////////////////////////////////////
//override printf output
int my_putc(char outtext, FILE *t){
  Serial.print( outtext );
  lcd.print( outtext );
};
//////////////////////////////////////////////////////////////
void setup(){
  fdevopen( &my_putc, 0 ); //override printf output, this allows to print to more than one output device
  Serial.begin(115200);
  Serial.println(("Start..."));
  pinMode(EMBEDDED_LED_PIN, OUTPUT);
  trigger = TRIGGER_INIT_VALUE;

  
  lcd.begin(20,4);
  
  
  lcd.clear();
  lcd.home();
  lcd.setBacklight(HIGH);
//  lcd.cursor();

  lcd.write ("Hello");

  lcd.createChar (0, smiley);    // load character to the LCD
  lcd.createChar (1, armsUp);    // load character to the LCD
  lcd.createChar (2, frownie);   // load character to the LCD

  
  taskBlink.init();
  taskBlink.start(500);
};


void loop()
{
  taskBlink.loop();
  if (Serial.available())
     {
     char c = Serial.read();

     switch (c)
     {
      case '1':
        Serial.write ("setting cursor to 0,0\n");
        lcd.setCursor(0, 0);
        
        break;
        
      case '2':
        Serial.write ("setting cursor to 0,1\n");
        lcd.setCursor(0, 1);
        break;
      case '3':
        Serial.write ("setting cursor to 0,2\n");
        lcd.setCursor(0, 2);
        break;
      case '4':
        Serial.write ("setting cursor to 0,3\n");
        lcd.setCursor(0, 3);
        break;
      default:
        break;
     }
     }
  // when characters arrive over the serial port...
//  if (Serial.available()) 
//  {
//    // wait a bit for the entire message to arrive
//    delay(100);
//    // clear the screen
////   lcd.clear();
////    lcd.write("Type a Char");
//    // read all the available characters
//    while (Serial.available() > 0) 
//    {
//      // display each character to the LCD
//      lcd.write(Serial.read());
//    }
//  }
}

int backlight(void)
{
  static int animate = 0;
  Serial.write ("backlight\n");

//  lcd.moveCursorRight();
//  lcd.scrollDisplayLeft();

  lcd.scrollDisplayRight();

lcd.setCursor (14,1);
lcd.print (char(animate));
animate++;
if (animate > 2)
  animate=0;
   
//  if (backlightOn)
//  {
//  lcd.setBacklight(LOW);
//  backlightOn=0;
////  lcd.write("Backlight OFF");
//
//  }
//  else
//  {
//  lcd.setBacklight(HIGH);
//  backlightOn=1;
////  lcd.write("Backlight ON");
// 
//  }
return (500);

}

/////////////////////////////////////////////////////////////////////////////////////
