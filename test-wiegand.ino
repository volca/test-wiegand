#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "WiegandNG.h" //https://github.com/jpliew/Wiegand-NG-Multi-Bit-Wiegand-Library-for-Arduino

int bufferlength = 52;
WiegandNG wg;


// Start Networking
void setup() {
  Serial.begin(9600);
  Serial.println(F("RFID test .."));
  
  if(!wg.begin(bufferlength)) {       
    Serial.println(F("Could not begin Wiegand logging,"));            
    Serial.println(F("Out of memory!"));
  }

}

///////////////////////////////////////////////////////
// LOOP function
void loop()
{
  if(wg.available()) {
    Serial.println("scan");

    wg.pause();             // pause Wiegand pin interrupts

    Serial.print("Bits=");
    Serial.println(wg.getBitCounted()); // display the number of bits counted

    Serial.print("RAW Binary=");
    volatile unsigned char *buffer= wg.getRawData();
    unsigned int bufferSize = wg.getBufferSize();
    unsigned int countedBits = wg.getBitCounted();

    unsigned int countedBytes = (countedBits/8);
    if ((countedBits % 8)>0) countedBytes++;
    unsigned int bitsUsed = countedBytes * 8;
    
    for (int i=bufferSize-countedBytes; i< bufferSize;i++) {
      unsigned char bufByte=buffer[i];
      for(int x=0; x<8;x++) {
        if ( (((bufferSize-i) *8)-x) <= countedBits) {
          if((bufByte & 0x80)) {
            Serial.print("1");
          }
          else {
            Serial.print("0");
          }
        }
        bufByte<<=1;
      }
    }
    Serial.println();
    
    wg.clear();             // compulsory to call clear() to enable interrupts for subsequent data
  }

}
