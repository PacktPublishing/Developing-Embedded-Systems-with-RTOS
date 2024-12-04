#include <Arduino.h>

#define LED (2)

void setup() {
  Serial.begin(9600);
  printf("Entering setup\n");
  // Set pin mode
  pinMode(LED,OUTPUT);
  
}

void loop() {
  printf("Entering loop\n");
  delay(500);
  printf("Setting LED HIGH\n");
  digitalWrite(LED,HIGH);
  delay(500);
  printf("Setting LED LOW\n");
  digitalWrite(LED,LOW);
}