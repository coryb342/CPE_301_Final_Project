//Cory Bateman
//Final Project

//----Libraries----
#include <LiquidCrystal.h>
#include <dht.h>

//----My Pins----
// --Outputs--
//  -LEDS-
//Green LED (Digital Pin 13, PB7)
//Yellow LED (Digital Pin 12, PB6)
//Red LED (Digital Pin 11, PB5)
//Blue LED (Digital Pin 10, PB4)
volatile unsigned char* port_b = (unsigned char*) 0x25; 
volatile unsigned char* ddr_b  = (unsigned char*) 0x24; 
volatile unsigned char* pin_b  = (unsigned char*) 0x23; 

//  -Motors- 
//Stepper
//Fan


// --Inputs--
//  -Buttons-
//Start (Digital pin 2, PE4)
//Stop (Digital pin 3, PE5)
//Reset (Digital pin 18, PD3)
volatile unsigned char* port_e = (unsigned char*) 0x2E; 
volatile unsigned char* ddr_e  = (unsigned char*) 0x2D; 
volatile unsigned char* pin_e  = (unsigned char*) 0x2C; 

volatile unsigned char* port_d = (unsigned char*) 0x2B; 
volatile unsigned char* ddr_d  = (unsigned char*) 0x2A; 
volatile unsigned char* pin_d  = (unsigned char*) 0x29; 
//  -Temp/Humidity-
dht DHT;
#define DHT11_PIN 22

// --LCD Pins--
const int RS = 4, EN = 5, D4 = 6, D5 = 7, D6 = 8, D7 = 9;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);


//My Delay Params
volatile unsigned char *myTCCR1A = (unsigned char *) 0x80;
volatile unsigned char *myTCCR1B = (unsigned char *) 0x81;
volatile unsigned char *myTCCR1C = (unsigned char *) 0x82;
volatile unsigned char *myTIMSK1 = (unsigned char *) 0x6F;
volatile unsigned int  *myTCNT1  = (unsigned  int *) 0x84;
volatile unsigned char *myTIFR1 =  (unsigned char *) 0x36;


//Serial Params
 #define RDA 0x80
 #define TBE 0x20  
 volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;
 volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;
 volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;
 volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4;
 volatile unsigned char *myUDR0   = (unsigned char *)0x00C6;

string state = "disabled";
string previousState = "disabled";
void setup() {
  //Initialize Serial Port
  U0init(9600);
  //Set all LEDs to Output Mode
  *port_b |= 0b11110000;
  //Set all Buttons as Inputs
  *port_e &= 0b11001111;
  *port_d &= 0b11110111;
  //Set up LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);


}



void loop() {
  // Starts in disabled mode
  //Disabled Mode = yellow led on. No display, no fan. 
  // Wait for start button via ISR, Switch to Idle mode. 
  //Idle mode:
  //-Green LED on
  //Check for the temp
  //Check for the water level. If low go to Error state.
  //if temp is higher than threshold, transition to Run state.
  //if stop button pressed, go back to disabled.
  //Run Mode:
  //Blue LED on.
  //if temp is good, go back to idle
  //if water is low, go to error
  //if stop button pressed, go to disabled.
  //Error Mode:
  //Display error message that water is too low.
  //Red LED on.
  //Reset button pressed goes to Idle Mode.
  //Stop button pressed goes to disabled mode. 


}

//Delay Function
void my_delay(unsigned int ticks)
{
  // stop the timer
  *myTCCR1B &= 0xF8;
  // set the counts
  *myTCNT1 = (unsigned int) (65536 - ticks);
  // start the timer
  * myTCCR1A = 0x0;
  * myTCCR1B |= 0b00000001;
  // wait for overflow
  while((*myTIFR1 & 0x01)==0); // 0b 0000 0000
  // stop the timer
  *myTCCR1B &= 0xF8;   // 0b 0000 0000
  // reset TOV           
  *myTIFR1 |= 0x01;
}


//Serial Functions
void U0init(unsigned long U0baud)
{
 unsigned long FCPU = 16000000;
 unsigned int tbaud;
 tbaud = (FCPU / 16 / U0baud - 1);
 *myUCSR0A = 0x20;
 *myUCSR0B = 0x18;
 *myUCSR0C = 0x06;
 *myUBRR0  = tbaud;
}
unsigned char U0kbhit()
{
  return (*myUCSR0A & RDA);
}

char U0getchar()
{
  unsigned char ch;
  ch = *myUDR0;
  return ch;
}

void U0putchar(unsigned char U0pdata)
{
  while(!(*myUCSR0A & TBE));
  *myUDR0 = U0pdata;
}
