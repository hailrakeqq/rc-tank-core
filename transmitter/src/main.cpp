#include <SPI.h>
#include <RF24.h>

RF24 radio(10,8); // CE, CSN
int motorPinA1 = 5;  
int motorPinA2 = 6;  
int motorPinB1 = 3;  
int motorPinB2 = 9;  
int currentLeftPWM = 0;
int currentRightPWM = 0;
int currentLeftDirection = 0;
int currentRightDirection = 0;
const byte address[6] = "00001";

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); 

  pinMode(motorPinA1, OUTPUT);
  pinMode(motorPinA2, OUTPUT);
  pinMode(motorPinB1, OUTPUT);
  pinMode(motorPinB2, OUTPUT);

  Serial.println("Reciever ready");
}

void loop()
{
  if (radio.available())
  {
    char text[32] = "";
    radio.read(&text, sizeof(text));

    int left_track, right_track;
    sscanf(text, "%d;%d", &left_track, &right_track);

    Serial.print("left: ");
    Serial.println(left_track);
    Serial.print("right: ");
    Serial.println(right_track);

    // ====== Death zone ======
    if (abs(left_track) < 10 && abs(right_track) < 10) {
      analogWrite(motorPinA1, 0);
      analogWrite(motorPinA2, 0);
      analogWrite(motorPinB1, 0);
      analogWrite(motorPinB2, 0);
      return;
    }

    // ====== Left track ======
    int leftPWM = constrain(abs(left_track), 0, 255);

    if (left_track > 0) {
      analogWrite(motorPinB1, 0);
      analogWrite(motorPinB2, leftPWM); // forward
    } else if (left_track < 0) {
      analogWrite(motorPinB2, 0);
      analogWrite(motorPinB1, leftPWM); // back
    } else {
      analogWrite(motorPinB1, 0);
      analogWrite(motorPinB2, 0);
    }

    // ====== Right track ======
    int rightPWM = constrain(abs(right_track), 0, 255);

    if (right_track > 0) {
      analogWrite(motorPinA1, 0);
      analogWrite(motorPinA2, rightPWM); // forward
    } else if (right_track < 0) {
      analogWrite(motorPinA2, 0);
      analogWrite(motorPinA1, rightPWM); // back
    } else {
      analogWrite(motorPinA1, 0);
      analogWrite(motorPinA2, 0);
    }
  }
  delay(10);
}
