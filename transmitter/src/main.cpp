#include <SPI.h>
#include <RF24.h>
#include <Arduino.h>
RF24 radio(13, 11); // CE, CSN

const byte address[6] = "00001";

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening(); // Передаємо

  Serial.println("Передавач готовий");
}

void loop()
{
  const char text[] = "Привіт!";
  radio.write(&text, sizeof(text));
  Serial.println("Повідомлення надіслано");
  delay(1000);
}