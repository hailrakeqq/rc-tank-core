#include <SPI.h>
#include <RF24.h>
#include <Arduino.h>

RF24 radio(10, 9);
const byte address[6] = "00001";


void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening(); 


  radio.setChannel(108);        
  radio.setDataRate(RF24_250KBPS); 
  

  Serial.println("Передавач готовий");
}

void loop()
{
  int leftRaw = analogRead(A2);
  int rightRaw = analogRead(A1);


  int leftTrackSpeed = map(leftRaw, 0, 1023, -256, 256);
  int rightTrackSpeed = map(rightRaw, 0, 1023, -256, 256);


  char data[32];
  sprintf(data, "%d;%d", leftTrackSpeed, rightTrackSpeed);
  // radio.write(&data, sizeof(data));
  bool sent = radio.write(&data, sizeof(data));
  if (sent) {
    Serial.println("Data sent successfully");
    Serial.println("Data send: " + String(data));
  } else {
    Serial.println("Send failed!");
  }
    delay(20);
  }
