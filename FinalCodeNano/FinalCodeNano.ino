#include <SPI.h>
#include <Wire.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(9, 8); // CE, CSN
//const byte address[10] = "ADDRESS01";
const byte address[6] = "00001";
char txt = "";

boolean radioReceived = 0;

void setup() {
  //setup radio and i2c
  Wire.begin(10);
  radio.begin();

  radio.setPALevel(RF24_PA_MIN);

  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  pinMode(1,OUTPUT);

  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop() {
  //radio is constantly listening for any data
  if (radio.available()) {
    txt = "";
    radio.read(&txt, sizeof(txt));
    radioReceived = txt;
  };

}

void requestEvent()
{
  Wire.write(radioReceived);//tells the MKR whether or not we have a successful radio connection
}

void receiveEvent(int howMany)
{
  digitalWrite(1,HIGH);//turns on the speaker so the user can locate the box
  //radio stops listening and begins transmitting
  radio.stopListening();
  radio.openWritingPipe(address);

  while(Wire.available())
  {
    txt = "";
    txt = Wire.read(); // receive byte as a character
  }
  radio.write(&txt, sizeof(txt));

  
}
