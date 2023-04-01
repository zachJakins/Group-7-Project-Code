// Code 1 : Sending Text (Receiver)
// Library: TMRh20/RF24 (https://github.com/tmrh20/RF24/)

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(9, 8); // CE, CSN
//const byte address[10] = "ADDRESS01";
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  Serial.println("Working");
  radio.startListening();
};

void loop() {
  if (radio.available()) {
    char txt[32] = "";
    radio.read(&txt, sizeof(txt));
    Serial.println(txt);
  };

};
