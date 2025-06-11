#include <SPI.h>
#include <RF24.h>

RF24 radio(10,8); // CE, CSN
int motorPinA1 = 5;  
int motorPinA2 = 6;  
int motorPinB1 = 3;  
int motorPinB2 = 9;  
const byte address[6] = "00001";

unsigned long lastReceiveTime = 0;
const unsigned long signalTimeout = 500; //ms

void stopMotors() {
  analogWrite(motorPinA1, 0);
  analogWrite(motorPinA2, 0);
  analogWrite(motorPinB1, 0);
  analogWrite(motorPinB2, 0);
}

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); 
  
  radio.setChannel(108);        
  radio.setDataRate(RF24_250KBPS); 

  pinMode(motorPinA1, OUTPUT);
  pinMode(motorPinA2, OUTPUT);
  pinMode(motorPinB1, OUTPUT);
  pinMode(motorPinB2, OUTPUT);

  stopMotors();
  Serial.println("Receiver ready");
}

void loop()
{
  if (radio.available())
  {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.print("Received: ");
    Serial.println(text);

    int left_track = 0, right_track = 0;
    sscanf(text, "%d;%d", &left_track, &right_track);

    Serial.print("left: ");
    Serial.println(left_track);
    Serial.print("right: ");
    Serial.println(right_track);

    lastReceiveTime = millis(); // Обновляем таймер

    // ====== Dead zone ======
    if (abs(left_track) < 10 && abs(right_track) < 10) {
      stopMotors();
      return;
    }

    // ====== Left Track ======
    int leftPWM = constrain(abs(left_track), 0, 255);
    if (left_track > 0) {
      analogWrite(motorPinB1, 0);
      analogWrite(motorPinB2, leftPWM);
    } else {
      analogWrite(motorPinB2, 0);
      analogWrite(motorPinB1, leftPWM);
    }

    // ====== Right Track ======
    int rightPWM = constrain(abs(right_track), 0, 255);
    if (right_track > 0) {
      analogWrite(motorPinA1, 0);
      analogWrite(motorPinA2, rightPWM);
    } else {
      analogWrite(motorPinA2, 0);
      analogWrite(motorPinA1, rightPWM);
    }
  }

  // timeout
  if (millis() - lastReceiveTime > signalTimeout) {
    stopMotors();
    Serial.println("No signal — motors stopped!");
  }

  delay(20);
}
