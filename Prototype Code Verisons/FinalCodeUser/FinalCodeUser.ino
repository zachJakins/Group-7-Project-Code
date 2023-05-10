
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(9, 8);  // CE, CSN
//const byte address[10] = "ADDRESS01";
const byte address[6] = "00001";
const byte address2[6] = "00002";

void setup() {
  Serial.begin(9600);
  while (!Serial);

  radio.begin();
  radio.openReadingPipe(0, address2);//listens on address 2
  radio.openWritingPipe(address); //writes on address 1

  radio.setPALevel(RF24_PA_MAX);

  Serial.println("Sample Number; Date; Time; Distance(mm); Pressure(mBar); Temperature(C); Humidity(%)\n");
};

void loop() {
  //transmit to the address
  radio.stopListening();
  byte s = 1;
  radio.write(&s, sizeof(s));

  //read what comes back
  radio.startListening();
  if (radio.available()) {
    char r[32];
    radio.read(&r, sizeof(r));

  //prints the data from our radio
    for (int i = 0; i < sizeof(r); i++) {
      if (r[i] == '#') {
        Serial.println();  //if we have a hashtag start a new line
      } else {
        Serial.print(r[i]);  //prints characters
      }
    }
  }
}
