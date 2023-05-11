#include <SPI.h>
#include <Wire.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <stdlib.h>

#define NOISEPIN 2
#define NANOADDRESS 0x0B

//receive variables
boolean noReceive = true;
byte radioReceived = 0;


//send variables
char s[32];
byte buf = 0;
boolean send = false;


RF24 radio(9, 8);  // CE, CSN
//const byte address[10] = "ADDRESS01";
const byte address[6] = "00001";
const byte address2[6] = "00002";



void setup() {
  //setup radio and i2c
  Wire.begin(NANOADDRESS);
  Serial.begin(9600);  //Transmit doesnt work otherwise
  radio.begin();

  radio.setPALevel(RF24_PA_MAX);

  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  radio.openWritingPipe(address2);
  radio.openReadingPipe(0, address);

  pinMode(NOISEPIN, OUTPUT);
  digitalWrite(NOISEPIN, LOW);
}

void loop() {
  if (send) {
    Serial.println("Attempt to Send");
    Serial.print(s);
    radio.write(s, sizeof(s));  //sends data
    send = false;
    Serial.println("Sent");
  }

  //while we havent received any data keep looking
  while (noReceive) {
    radio.startListening();
    //radio is constantly listening for any data
    if (radio.available()) {
      byte r;
      radio.read(&r, sizeof(r));
      if (r == 1) {
        radioReceived = 1;
      }
      //when we do get a ping stop looking
      noReceive = false;
      radio.stopListening();
    };
  }
}


//When the MKR requests "radio status"
void requestEvent() {
  Wire.write(radioReceived);  //tells the MKR whether or not we have a successful radio connection
  //resets radio status.
  noReceive = true;
  radioReceived = 0;
}

//If the radio has been 'pinged', the MKR will begin sending data over the i2c pins
void receiveEvent(int howMany) {
  digitalWrite(NOISEPIN, HIGH);
  buf = 0;
  memset(s, 0, sizeof(s));  //reset s

  while (Wire.available()) {
    s[buf] = Wire.read();  // receive byte as a character and put into array
    buf++;
  }
  send = true;

  delay(50);
  radio.stopListening();
  delay(50);
}
