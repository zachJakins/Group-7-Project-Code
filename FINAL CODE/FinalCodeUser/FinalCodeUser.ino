
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(9, 8);  // CE, CSN
//const byte address[10] = "ADDRESS01";
const byte address[6] = "00001";
const byte address2[6] = "00002";

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(1, address2);  //listens on address 2
  radio.openWritingPipe(address);      //writes on address 1

  radio.setPALevel(RF24_PA_MAX);

  Serial.println("Sample Number; Date; Time; Distance(mm); Pressure(mBar); Temperature(C); Humidity(%)");
};

void loop() {
  //transmit to the address
  radio.stopListening();
  byte s = 1;
  radio.write(&s, sizeof(s));


  //read what comes back
  radio.startListening();
  delay(400);
  if (radio.available()) {
    char r[32] = "";
    radio.read(&r, sizeof(r));
    Serial.print(r);
  }
}
