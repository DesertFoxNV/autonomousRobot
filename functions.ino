void getID() {
 Serial.println(deviceID);
}

void scan() {
  int pos = 0;
  delay(1000);
  laser.on();
  for (pos = 40; pos <= 140; pos += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    laserServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 140; pos >= 40; pos -= 2) { // goes from 180 degrees to 0 degrees
    laserServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  laser.off();
  laserServo.write(100);
  delay(1000);
}
