# ESP32DCCAccessoryDecoder
ESP32 based accessory decoder script

This script allows you to easily turn an ESP32 Dev Module into a DCC Accessory Decoder to control points/turnouts and anything else you may want your DCC controller to send commands to. The script has been tested no an NCE Powercab and DCC++EX running on an Arduino Mega2560.

The main script contains a single function "ControlAccDecoder" as shown below that will display on the serial port any Accessory decoder commands sent through and contains a basic example switch statement to get you started.
For those who want to delve a bit deeper the DecoderFunction file contains the processes to read the DCC Packets and could be adapted to become a loc decoder quite easily.

Add in your own code for servos, LEDs & relays etc. 

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
