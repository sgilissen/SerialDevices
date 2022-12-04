/******************************************************************************
* This example shows how to control a Partner Tech CD-7220 VFD display
* It may work with other ESCPOS displays as well, but this has not been tested.
* -----------------------------------------------------------------------------
* INTERNAL CONNECTION DIAGRAM (Not applicable when using the DB9 connector)
*
*                     Pin header on back of display
*                  ------------------------------------
*                  |    |    |    |    |    |    |    |
*                 VCC  VCC  GND  GND   ?    ?    RX   ?
*
* Connect to:      +        GND  GND   NC   NC    4   NC  
*                 12V       12V   
*                (Connect second GND to Arduino GND)
******************************************************************************/
#include <SoftwareSerial.h>
SoftwareSerial softSerial(4, 5, true); // RX, TX, reversed bits

 
void setup() {
  Serial.begin(9600);
  softSerial.begin(9600);
  delay(10);
  Serial.println("Init VFD");
  // Initialize the display
  softSerial.write(0x1B);
  softSerial.write(0x40);
  delay(10);
}

void loop() {
  softSerial.write(0x0C); // Clear the display
  delay(10);
  writeLineVFD(1, "Hello World");
  writeLineVFD(2, "Second line");
  delay(2000);
  softSerial.write(0x0C); // Clear the display
  delay(2000);
  softSerial.write(0x0C); // Clear the display
  delay(10);
  writeLineVFD(1, "Some symbols");
  writeLineVFD(2, "!@#*&%$=");
  delay(2000);
}

void setDisplayModeVFD(){
  // Set display mode to ESCPOS
  softSerial.write(0x02);  //  STX
  softSerial.write(0x05);  //  05
  softSerial.write(0x43);  //  C
  softSerial.write(0x31);  //  31h
  softSerial.write(0x03);  //  ETX
}


void setCursorVFD(int x, int y){
    // Set the cursor to position
    softSerial.write(0x1F);   // Set cursor
    softSerial.write(0x24);   // Set Cursor
    softSerial.write(x);      // position x
    softSerial.write(y);      // position y      
}


void setBrightnessVFD(int brightness){
  // Set the brightness of the VFD
  softSerial.write(0x1B);
  softSerial.write(0x2A);
  softSerial.write(brightness);
}

void writeLineVFD(int line, char message[]){
  softSerial.write(0x1B);
  softSerial.write(0x51);
  if(line == 1){
    // top line
    softSerial.write(0x41);
  } else if (line == 2) {
    // bottom line
    softSerial.write(0x42);    
  }
  softSerial.write(message);
  softSerial.write(0x0D);
}


void writeDemoMessageVFD(int line, char message[]){
  // Write the demo message to the displays EEPROM.
  // This is the message that gets displayed when the display is not receiving serial data (yet). 
  // For example, if the host is still booting.
  int hexLine = 0x31;

  if(line == 1){
    hexLine = 0x31;
  } else if (line == 2) {
    hexLine = 0x32;
  }
  softSerial.write(0x02);                 // STX
  softSerial.write(0x05);                 // 05
  softSerial.write(0x4C);                 // L
  softSerial.write(hexLine);                 // 31h -> Line 1
  softSerial.write(message); 
  softSerial.write(0x03);                 // ETX
}

void startDemoModeVFD(){
  // Starts the "demo mode". This will show/scroll the demo messages saved in the displays EEPROM.
  softSerial.write(0x02);                 // STX
  softSerial.write(0x05);                 // 05
  softSerial.write(0x44);                 // D
  softSerial.write(0x08);                 // 08
  softSerial.write(0x03);                 //  ETX
}