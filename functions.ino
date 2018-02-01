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

void readVoltage() {
  float voltage= analogRead(A8) * (5.0 / 1023.0);
  Serial.println(voltage);
}

void checkSensors() {
  for (uint8_t i = 0; i < ultraSonicSensorNumber; i++) { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * ultraSonicSensorNumber;  // Set next time this sensor will be pinged.
      if (i == 0 && currentSensor == ultraSonicSensorNumber - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
      ultraSonicSensor[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      ultraSonicSensor[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (ultraSonicSensor[currentSensor].check_timer())
    cm[currentSensor] = ultraSonicSensor[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  // The following code would be replaced with your code that does something with the ping results.
  for (uint8_t i = 0; i < ultraSonicSensorNumber; i++) {
    switch (i) {
      case 0:
        Serial.print("Front");
      break;
      case 1:
        Serial.print("Left");
      break;
      case 2:
        Serial.print("Back");
      break;
      case 3:
        Serial.print("Right");
      break;
    }
    
    Serial.print("=");
    Serial.print(cm[i]);
    Serial.print("cm ");
  }
  Serial.println();
}
