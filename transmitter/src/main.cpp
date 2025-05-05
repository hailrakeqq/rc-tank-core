#include <SPI.h>
#include <RF24.h>
#include <Arduino.h>
RF24 radio(10, 9); // CE, CSN

const byte address[6] = "00001";

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  Serial.println("Transmitter ready");
}

void loop()
{
  int leftRaw = analogRead(A0);  // 0..1023
  int rightRaw = analogRead(A1); // 0..1023

  int leftTrackSpeed = map(leftRaw, 0, 1023, -256, 256);   // -256..256
  int rightTrackSpeed = map(rightRaw, 0, 1023, -256, 256); // -256..256

  char data[32];
  sprintf(data, "%d;%d", leftTrackSpeed, rightTrackSpeed);
  radio.write(&data, sizeof(data));
  Serial.println("Data send: " + String(data));
}
