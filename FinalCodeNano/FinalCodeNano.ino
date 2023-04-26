#include <SPI.h>
#include <Wire.h>
#include <RF24.h>
#include <nRF24L01.h>

#define NOISEPIN 2
#define NANOADDRESS 0x0B


boolean noReceive = true;


RF24 radio(9, 8);  // CE, CSN
//const byte address[10] = "ADDRESS01";
const byte address[6] = "00001";
char txt = "";

boolean radioReceived = 0;

void setup() {
  //setup radio and i2c
  Wire.begin(NANOADDRESS);
  radio.begin();

  radio.setPALevel(RF24_PA_MIN);

  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  pinMode(NOISEPIN, OUTPUT);
  digitalWrite(NOISEPIN, LOW);

  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop() {
//while we havent received any data keep looking
  while (noReceive) {
    radio.openReadingPipe(0, address);
    radio.startListening();
    //radio is constantly listening for any data
    if (radio.available()) {
      char txt = "";
      radio.read(&txt, sizeof(txt));
      radioReceived = txt;
      //when we do get a ping stop looking
      noReceive=false;
    };
  }
}


//When the MKR requests "radio status"
void requestEvent() {
  Wire.write(radioReceived);  //tells the MKR whether or not we have a successful radio connection

  //resets radio status.
  noReceive=true;
  radioReceived = "0";
}

//If the radio has been 'pinged', the MKR will begin sending data over the i2c pins
void receiveEvent(int howMany) {
  digitalWrite(NOISEPIN, HIGH);  //turns on the speaker so the user can locate the box
  //radio stops listening and begins transmitting
  radio.stopListening();
  radio.openWritingPipe(address);

  while (Wire.available()) {
    char txt = "";
    txt = Wire.read();  // receive byte as a character
    radio.write(&txt, sizeof(txt));
  }
}
