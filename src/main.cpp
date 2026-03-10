#include <Arduino.h>
#include <SoftwareSerial.h>

namespace {
constexpr uint8_t kServoPins[] = {5, 6, 7, 8};
constexpr uint8_t kGripperIn1Pin = 9;
constexpr uint8_t kGripperIn2Pin = 10;
constexpr uint8_t kGripperEnablePin = 11;

SoftwareSerial bluetooth(3, 4);

int servoTargetPos[4] = {90, 90, 90, 90};
int servoCurrentPos[4] = {90, 90, 90, 90};
int lastS5Value = 90;
int speedDelayMs = 20;

uint16_t angleToPulseUs(int angle) {
  angle = constrain(angle, 0, 180);
  return static_cast<uint16_t>(map(angle, 0, 180, 544, 2400));
}

void sendServoPulse(uint8_t pin, uint16_t pulseUs) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(pulseUs);
  digitalWrite(pin, LOW);

  const uint16_t frameUs = 20000;
  if (pulseUs < frameUs) {
    delayMicroseconds(frameUs - pulseUs);
  }
}

void writeServoAngle(uint8_t pin, int angle) {
  const uint16_t pulseUs = angleToPulseUs(angle);

  for (int i = 0; i < 3; ++i) {
    sendServoPulse(pin, pulseUs);
  }
}

void stopGripper() {
  digitalWrite(kGripperIn1Pin, LOW);
  digitalWrite(kGripperIn2Pin, LOW);
  analogWrite(kGripperEnablePin, 0);
}

void moveServo(uint8_t servoIndex, int targetPos) {
  targetPos = constrain(targetPos, 0, 180);

  int &currentPos = servoCurrentPos[servoIndex];
  if (currentPos == targetPos) {
    return;
  }

  const int step = (currentPos < targetPos) ? 1 : -1;
  while (currentPos != targetPos) {
    currentPos += step;
    writeServoAngle(kServoPins[servoIndex], currentPos);
    delay(speedDelayMs);
  }
}

void handleCommand(const String &command) {
  if (command.startsWith("s1")) {
    servoTargetPos[0] = constrain(command.substring(2).toInt(), 0, 180);
    moveServo(0, servoTargetPos[0]);
    Serial.println("Servo 1 set to: " + String(servoTargetPos[0]));
    return;
  }

  if (command.startsWith("s2")) {
    servoTargetPos[1] = constrain(command.substring(2).toInt(), 0, 180);
    moveServo(1, servoTargetPos[1]);
    Serial.println("Servo 2 set to: " + String(servoTargetPos[1]));
    return;
  }

  if (command.startsWith("s3")) {
    servoTargetPos[2] = constrain(command.substring(2).toInt(), 0, 180);
    moveServo(2, servoTargetPos[2]);
    Serial.println("Servo 3 set to: " + String(servoTargetPos[2]));
    return;
  }

  if (command.startsWith("s4")) {
    servoTargetPos[3] = constrain(command.substring(2).toInt(), 0, 180);
    moveServo(3, servoTargetPos[3]);
    Serial.println("Servo 4 set to: " + String(servoTargetPos[3]));
    return;
  }

  if (command.startsWith("speed")) {
    speedDelayMs = constrain(command.substring(5).toInt(), 1, 100);
    Serial.println("Speed set to: " + String(speedDelayMs) + " ms per step");
    return;
  }

  if (command.startsWith("s5")) {
    const int value = constrain(command.substring(2).toInt(), 0, 180);

    if (value == lastS5Value) {
      Serial.println("Same s5 value received - motor not triggered.");
      return;
    }

    lastS5Value = value;

    if (value > 100) {
      digitalWrite(kGripperIn1Pin, HIGH);
      digitalWrite(kGripperIn2Pin, LOW);
      analogWrite(kGripperEnablePin, 200);
      delay(3000);
      stopGripper();
      Serial.println("Gripper opened.");
      return;
    }

    if (value < 80) {
      digitalWrite(kGripperIn1Pin, LOW);
      digitalWrite(kGripperIn2Pin, HIGH);
      analogWrite(kGripperEnablePin, 200);
      delay(3000);
      stopGripper();
      Serial.println("Gripper closed.");
      return;
    }

    stopGripper();
    Serial.println("Gripper stopped.");
    return;
  }

  Serial.println("Unknown command!");
}
}

void setup() {
  for (uint8_t pin : kServoPins) {
    pinMode(pin, OUTPUT);
  }

  pinMode(kGripperIn1Pin, OUTPUT);
  pinMode(kGripperIn2Pin, OUTPUT);
  pinMode(kGripperEnablePin, OUTPUT);

  Serial.begin(9600);
  bluetooth.begin(9600);

  for (uint8_t i = 0; i < 4; ++i) {
    writeServoAngle(kServoPins[i], servoCurrentPos[i]);
  }

  stopGripper();
}

void loop() {
  if (!bluetooth.available()) {
    return;
  }

  String dataIn = bluetooth.readString();
  dataIn.trim();

  if (dataIn.length() == 0) {
    return;
  }

  Serial.println("Received: " + dataIn);
  handleCommand(dataIn);
}
