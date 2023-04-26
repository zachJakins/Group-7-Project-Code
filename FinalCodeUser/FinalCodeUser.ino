
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(9, 8);  // CE, CSN
//const byte address[10] = "ADDRESS01";
const byte address[6] = "00001";
char txt = "";

void setup() {
  Serial.begin(9600);
  while(!Serial);

  radio.begin();

  radio.setPALevel(RF24_PA_MIN);

  Serial.println("Sample Number; Date; Time; Distance(mm); Pressure(mBar); Temperature(C); Humidity(%)\n");
};

void loop() {
  //transmit to the address
  radio.openWritingPipe(address);
  radio.stopListening();
  char txt = "1";
  radio.write(&txt, sizeof(txt));

  //read what comes back
  radio.openReadingPipe(0, address);
  radio.startListening();

  if (radio.available()) {
    char txt = "";
    radio.read(&txt, sizeof(txt));

    if (txt != "#") {
      Serial.print(txt);  //prints characters
    } else {
      Serial.println();  //if we have a hashtag start a new line
    }
  }
};
