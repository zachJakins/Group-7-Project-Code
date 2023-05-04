
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(9, 8);  // CE, CSN
//const byte address[10] = "ADDRESS01";
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  while (!Serial);

  radio.begin();

  radio.setPALevel(RF24_PA_MIN);

  Serial.println("Sample Number; Date; Time; Distance(mm); Pressure(mBar); Temperature(C); Humidity(%)\n");
};

void loop() {
  //transmit to the address
  radio.openWritingPipe(address);
  radio.stopListening();
  byte s = 1;
  radio.write(&s, sizeof(s));

  //read what comes back
  radio.openReadingPipe(0, address);
  radio.startListening();

  if (radio.available()) {
    boolean finished = false;
    Serial.println("Recieved")
    while (!finished)
    {
      byte r;
      radio.read(&r, sizeof(r));
      Serial.print((char)r);  //prints characters
      if (r == '#') {
        Serial.println();  //if we have a hashtag start a new line
      }
      if (!(radio.available())) {
      finished = true;
    }
  }
}

};
