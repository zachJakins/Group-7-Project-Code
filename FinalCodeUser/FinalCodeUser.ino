
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(9, 8); // CE, CSN
//const byte address[10] = "ADDRESS01";
const byte address[6] = "00001";
char txt[32] = "";

void setup() {
  Serial.begin(9600);

  radio.begin();
  
  radio.setPALevel(RF24_PA_MIN);

  Serial.println("Working");

  Serial.println("Sample Number; Date; Time; Distance(mm); Pressure(mBar); Temperature(C); Humidity(%)\n");
};

void loop() {
  //transmit to the address
  radio.openWritingPipe(address);
  radio.stopListening();
  txt[32] = "1";
  radio.write(&txt, sizeof(txt));
  
  //read what comes back
  radio.openReadingPipe(0, address);
  radio.startListening();

  if (radio.available()) {
    txt[32] = "";
    radio.read(&txt, sizeof(txt));
  }

  if(txt != "#")
  {
    Serial.print(txt);//prints characters
  }
  else {
    Serial.println();//if we have a hashtag start a new line
  }

};
