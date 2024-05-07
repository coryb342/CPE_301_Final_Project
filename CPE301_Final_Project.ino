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




//My Delay Params
volatile unsigned char *myTCCR1A = (unsigned char *) 0x80;
volatile unsigned char *myTCCR1B = (unsigned char *) 0x81;
volatile unsigned char *myTCCR1C = (unsigned char *) 0x82;
volatile unsigned char *myTIMSK1 = (unsigned char *) 0x6F;
volatile unsigned int  *myTCNT1  = (unsigned  int *) 0x84;
volatile unsigned char *myTIFR1 =  (unsigned char *) 0x36;
volatile unsigned char *portDDRB = (unsigned char *) 0x24;
volatile unsigned char *portB = (unsigned char *) 0x25;

//Serial Params
 #define RDA 0x80
 #define TBE 0x20  
 volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;
 volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;
 volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;
 volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4;
 volatile unsigned char *myUDR0   = (unsigned char *)0x00C6;

void setup() {
  //Initialize Serial Port
  U0init(9600);
  
  //Set all LEDs to Output Mode
  *port_b |= 11110000;
}



void loop() {
  // put your main code here, to run repeatedly:

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
