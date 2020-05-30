#include <SoftwareSerial.h>

SoftwareSerial BTserial(13, 12); // RX | TX 

// Motor Left
int dir1PinA = 7;
int dir2PinA = 6;

// Motor Right
int dir1PinB = 9;
int dir2PinB = 8;

// Spinner
int refSpinnerPin = 3;
int ctrlSpinnerPin = 2;

// Enums
enum Direction {
  Forward = 1, Backward = 0, Stop = 2
};
enum Motor {
  Left = 1, Right = 0
};

// Globals
int pin1 = -1;
int pin2 = -1;
int controlState = -1;

void setup() {
  Serial.begin(9600);
  BTserial.begin(9600);

  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  pinMode(dir1PinB, OUTPUT);
  pinMode(dir2PinB, OUTPUT);
  pinMode(refSpinnerPin, OUTPUT);
  pinMode(ctrlSpinnerPin, OUTPUT);

  digitalWrite(refSpinnerPin, HIGH);
}

void loop() {
  updateControlState();
  runMotor();
}

void runSpinner() {
  if (controlState == '8') {
    digitalWrite(ctrlSpinnerPin, HIGH);
  } else if (controlState == '9') {
    digitalWrite(ctrlSpinnerPin, LOW);
  }
}

void updateControlState() {
  // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (BTserial.available()) {
    char msg = BTserial.read();
    Serial.println(msg);
    controlState = msg;
  }

  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available()) {
    char msg = Serial.read();
    BTserial.write(msg);
  }
}

void runMotor() {
  switch (controlState) {
  case '0':
    runMotor(Left, Stop);
    runMotor(Right, Stop);
    break;

  case '1':
    runMotor(Left, Forward);
    runMotor(Right, Forward);
    break;

  case '2':
    runMotor(Left, Backward);
    runMotor(Right, Backward);
    break;

  case '3':
    runMotor(Left, Backward);
    runMotor(Right, Forward);
    break;

  case '4':
    runMotor(Left, Forward);
    runMotor(Right, Backward);
    break;
  }
}

void runMotor(Motor m, Direction d) {
  pin1 = dir1PinA;
  pin2 = dir2PinA;
  if (m == Right) {
    pin1 = dir1PinB;
    pin2 = dir2PinB;
  }
  int valPin1 = HIGH;
  int valPin2 = LOW;
  if (d == Backward) {
    valPin1 = LOW;
    valPin2 = HIGH;
  } else if (d == Stop) {
    valPin1 = LOW;
    valPin2 = LOW;
  }

  digitalWrite(pin1, valPin1);
  digitalWrite(pin2, valPin2);

  return;
}
