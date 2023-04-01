#define NANOSWITCH 0

boolean state = 0;

void setup() {
  //serial setup
  Serial.begin(9600);
  Serial.println("Start");
  
  pinMode(NANOSWITCH,OUTPUT);//Arduino pin 0 switches the nano

}

void loop() {
  
  //flips the power state of our NANO
  digitalWrite(NANOSWITCH, state);
  state=!state;

  //If statement for printing to serial.
  if (state)
  {
    println("State HIGH (Arduino should be OFF)");
  }
  else {
    println("State LOW (Arduino should be ON)")
  }
  delay(5000);//delay 5s

}
