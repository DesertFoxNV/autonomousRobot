void setPins() {

  // Attach Servo
  laserServo.attach(servoPin);

  // Set Motor Pins
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(rightMotors, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(leftMotors, OUTPUT);

  // Set PIR Sensor Input Pin
  pinMode(PIR_SENSOR, INPUT);

  // Set PIR Sensor Input Pin
  pinMode(FUN_MODE_PIN, OUTPUT);

  // Set starting motor direction
  directionType("forward");

  // Set Voltage Pin
  pinMode(A8, INPUT);
}
