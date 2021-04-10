/* ESP32AccessoryDecoder by Simon Coward 10/04/2021
 * This allows an ESP32 Dev Module to be used as a DCC Accessory Decoder
 * This was written because I could not get the NmraDcc library to work with an ESP32 and couldn't find anything else that worked with an ESP32 
 * 
 * I have split the functions over two files so that the main file just has the main function you will need to use.
 * There are a couple of sections commented out in the DecoderFunctions file that if uncommented allow you to see the raw packet data etc.
 * 
 * Script has been tested on NCE PowerCab and DCC++EX
 * 
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.  If not, see
 * <http://www.gnu.org/licenses/>.
 * 
 * 
 */


#include <Arduino.h>

//Editable input pin
//IMPORTANT DO NOT CONNECT ESP32 to TRACK
//Use circuit found on http://www.mynabay.com/dcc_monitor/
#define INPUTPIN 2

//works best on this speed but can be changed
#define SERIAL_SPEED 115200

//DO NOT ALTER
#define INTERRUPT_SAFE IRAM_ATTR

#define STD 1
#define EXT 0
static bool ISREvent;
static unsigned long ISRTime;
static unsigned long ISRLastTime;
static byte  Preamble = 0;
static byte  PreambleCounter = 0;
static byte  PacketStart = 0;
static byte  bitcounter = 0;
static byte  packetdata[38];//array to get all the data ready for processing
byte  AddressByte = 0;
byte  AddressByteTwo = 0;
byte  InstructionByte = 0;
byte  ErrorByte = 0;





//This is the function you need to edit to make your decoder work for the addresses you want to use.
//NOTE: Sometimes the same function will be triggered multiple times. This is not a bug. The same packets are sent multiple times very quickly
//in case the decoder was busy and missed a transmission.
void ControlAccDecoder(byte index,byte dir,int AccAddr,int BoardAddr){
  //These can be commented out once when you put the script live.
    Serial.print(" BoardAddr: ");
    Serial.println(BoardAddr);

    Serial.print( "index: ");
    Serial.println(index);

    Serial.println("two values to use in switch statements");
    Serial.print(" AccAddr: ");
    Serial.println(AccAddr);  
    Serial.print( "dir: ");
    Serial.println(dir);

    //Sample of how to use this to control accessories
    switch(AccAddr){
      case 2: //accessory decoder address 2
        if(dir > 0){
            //code to move servo, control relay in here
        }else{
            //code to move servo, control relay opposite direction in here 
        }
        break;
      case 120: //accessory decoder address 120
        if(dir > 0){
            //code to move servo, control relay in here
        }else{
            //code to move servo, control relay opposite direction in here 
        }
        break;
      case 1300: //accessory decoder address 1300
        if(dir > 0){
            //code to move servo, control relay in here
        }else{
            //code to move servo, control relay opposite direction in here 
        }
        break;
      default://ignore any addresses not listed above
        break;
    }
}


void setup() {
  Serial.begin(SERIAL_SPEED);
  Serial.println("ESP32AccessoryDecoder");
  initdecoders(); //sets up the interrupt
}


void loop() {
  //needs to run as often as possible
  DCCPacketread(); //Reads any incoming packets  
}
