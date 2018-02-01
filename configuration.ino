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
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);

  // Set Voltage Pin
  pinMode(A8, INPUT);
}
