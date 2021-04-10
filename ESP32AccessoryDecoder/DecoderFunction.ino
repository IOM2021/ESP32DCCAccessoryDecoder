void interruptHandler(){
  ISREvent = 1;
  ISRTime = micros();
}

void initdecoders(){
  pinMode(INPUTPIN, INPUT_PULLUP);
  Serial.print("INPUTPIN=");
  Serial.println(INPUTPIN);
  attachInterrupt(digitalPinToInterrupt(INPUTPIN),interruptHandler, RISING);
} 



//test function
void printpacket(byte packtype){
  for(int q = 0;q<37;q++){
    if(q == 1 || q == 9 || q == 10 || q == 18 ||q == 19 || q == 27 || q == 28 || q == 36){
        Serial.print(" ");  
     }
      Serial.print(packetdata[q]); 
    }
    Serial.print(" packtype: ");
    Serial.print(packtype);
    Serial.println(" ");
}


//Acc decoder results
void AccDecoder(byte packtype){
  byte index = 0;
  byte dir;
  int AccAddr;
  int BoardAddr;
  
  if(packtype == (STD)){
    dir = bitRead(InstructionByte,0);
    index = bitRead(InstructionByte,1);
    if(bitRead(InstructionByte,2) > 0){
      bitSet(index,1);
    }
    Serial.print(" addr 1st bit: ");
    Serial.println(bitRead(AddressByte,7));

    BoardAddr = AddressByte - 0b10000000;
    //now get the weird address system from instruction byte
    if(bitRead(InstructionByte,4) < 1){
      bitSet(BoardAddr,6);
    }
    if(bitRead(InstructionByte,5) < 1){
      bitSet(BoardAddr,7);
    }
    if(bitRead(InstructionByte,6) < 1){
      bitSet(BoardAddr,8);
    }

    AccAddr = ((BoardAddr - 1) * 4) + index + 1;
    ControlAccDecoder(index,dir,AccAddr,BoardAddr);
  }else{
    Serial.println("Extended Accessory Decoder Control Packet Format NOT supported...yet!");
  }
}

void processpacket(byte packtype){
  
  if(packtype == STD){
    for(int q = 0;q < 8;q++){
      if(packetdata[1 + q] > 0){
        bitSet(AddressByte,7-q);
      }else{
        bitClear(AddressByte,7-q);
      }
      //bitClear(AddressByteTwo,7-q);//No data so clear all bits
      if(packetdata[10 + q] > 0){
        bitSet(AddressByteTwo,7-q);
      }else{
        bitClear(AddressByteTwo,7-q);
      }
      //end of edit
      if(packetdata[10 + q] > 0){
        bitSet(InstructionByte,7-q);
      }else{
        bitClear(InstructionByte,7-q);
      }

      if(packetdata[19 + q] > 0){
        bitSet(ErrorByte,7-q);
      }else{
        bitClear(ErrorByte,7-q);
      }
      
      
    }
  }else{ //extended packet
    for(int q = 0;q < 8;q++){
      if(packetdata[1 + q] > 0){
        bitSet(AddressByte,7-q);
      }else{
        bitClear(AddressByte,7-q);
      }

      if(packetdata[10 + q] > 0){
        bitSet(AddressByteTwo,7-q);
      }else{
        bitClear(AddressByteTwo,7-q);
      }

      if(packetdata[19 + q] > 0){
        bitSet(InstructionByte,7-q);
      }else{
        bitClear(InstructionByte,7-q);
      }

      if(packetdata[28 + q] > 0){
        bitSet(ErrorByte,7-q);
      }else{
        bitClear(ErrorByte,7-q);
      }
    }
  }
  //printpacket(packtype); //Uncomment to see the raw data packet
  //uncomment to see the sections of Data
  /*
  if(AddressByte > 0 && AddressByte != 0b11111110 && AddressByte != 0b11111111 ){
    Serial.print("Addr: ");
    Serial.print(AddressByte,BIN);
    Serial.print(" Addr2: ");
    Serial.print(AddressByteTwo,BIN);
    Serial.print(" Inst: ");
    Serial.print(InstructionByte,BIN);
    Serial.print(" Err: ");
    Serial.println(ErrorByte,BIN);
  }
  */
  //everything it now packed ready for processing

  //check it's not a repeat of last instruction
  
    if(bitRead(AddressByte,7) == 1 && bitRead(AddressByte,6) == 0){//Accessory decoder
      AccDecoder(packtype);
    }
}

void DCCPacketread(){
  int timepassed;
  byte packettype = 0;
  byte thisBit;
  if(ISREvent > 0){
    timepassed = ISRTime - ISRLastTime;
    if(timepassed > 10){//junk filter
      if(timepassed < 125){//130 Mega //125 NCE
        thisBit = 1;
      }else{
        thisBit = 0;
      }
      //start working on preamble
      if(Preamble < 1){
        if(thisBit > 0){
          PreambleCounter++;
        }else{
          PreambleCounter = 0;
        }
        if(PreambleCounter > 10){//must collect more than 10 preamble bits
          Preamble = 1;  
        }
      }else{//preamble complete look for start of packet data
        if(PacketStart < 1){
          if(thisBit < 1){
            PacketStart = 1;
            packetdata[bitcounter] = thisBit;
            bitcounter++;
          }
        }else{//got first bit
          if(bitcounter < 37){
            packetdata[bitcounter] = thisBit;
            bitcounter++;
            if(bitcounter == 37){
                packettype = packetdata[27];
                processpacket(packettype);
                //restart process
                Preamble = 0;
                PreambleCounter = 0;
                PacketStart = 0;
                bitcounter = 0; 
            }  
          }
        }
      }
    }
    ISRLastTime = ISRTime ;
    ISREvent = 0;
  }
}
