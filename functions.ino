void getID() {
 Serial.println(deviceID);
}

void scan() {
  int pos = 0;
  delay(1000);
  laser.on();

  int duration = 40; //duration is 100 loops
  for (int pos=0; pos<duration; pos++){
    //move servo from 40 and 140 degrees forward
    laserServo.write(Easing::easeInOutCubic(pos, 60, 120, duration));
    delay(15); //wait for the servo to move
  }

  for (int pos=0; pos<duration; pos++){
    //move servo -100 degrees from position 140 (back to 40)
    laserServo.write(Easing::easeInOutCubic(pos, 120, -60, duration));
    delay(15);
  }

  laser.off();

  duration = 50;
  for (int pos=0; pos<duration; pos++){
    //move servo from 40 and 100 degrees forward
    laserServo.write(Easing::easeInOutCubic(pos, 60, 100, duration));
    delay(15); //wait for the servo to move
  }
  
  laserServo.write(100);
  delay(1000);
}

void readVoltage() {
  float voltage= analogRead(A8) * (5.0 / 1023.0);
  if (debug) {
    // Serial.print(voltage); 
    // Serial.println(" V");
  }
  if (voltage < 3.5) {
    lowVoltageCount = lowVoltageCount + 1;
    if (lowVoltageCount > 50) {
      if (debug) {
        Serial.println("Low Voltage");  
        Serial.print(voltage);
        Serial.println(" V");
      }
    solidStateRelay.off();
    while (1) {}
    }
  } else {
    lowVoltageCount = 0;
  }
}

void checkUltraSonicSensors() {
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
    
    if (debug) {
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

    if((cm[0] < detectionDistance && cm[0] > 0) ||
    (cm[1] < sideDistance && cm[1] > 0) ||
    (cm[3] < sideDistance && cm[3] > 0)) {
      detectionCount++;
    } else {
      detectionCount = 0;
    }

    if (detectionCount > 10) {
      objectDetected = true;
      timeOfLastDetection = millis();
    }
  }
  if(debug) {
    Serial.println();
  }
}

void explore(){
  

  if (resetSensors) { // Allow time for sensors to reset after a blocking scan operation
    if (debug) {
      Serial.println("resetting sensors");
      Serial.print("sensorResetTimer");
      Serial.println(sensorResetTimer);
    }
    
    if(sensorResetTimer == 0) {
      sensorResetTimer = millis() + sensorResetDelay;
    }

    if (debug) {
      Serial.print("millis() ");
      Serial.print(millis());
    }
    

    long difference = sensorResetTimer - millis();

    if (debug) {
     Serial.print("Difference ");
     Serial.println(difference); 
    }
    
    if(difference < 0) {
      resetSensors = false;
      sensorResetTimer = 0;
    }
    return;
  }

  
  if(!objectDetected){ // If no object detected proceed with movement
    
    if (debug) {
     Serial.println("Object Not Detected"); 
    }
    
    analogWrite(leftMotors, exploreSpeed);
    analogWrite(rightMotors, exploreSpeed);
    
  } else if(!objectScanned) { // If object not scanned, turn off motors
    
    if (debug) {
      Serial.println("Object Not Scanned");
    }
    analogWrite(leftMotors, 0);
    analogWrite(rightMotors, 0);
    objectScanned = true;
    scan();
  } else { // If object has been scanned, initiate turn
    if (debug) {
      Serial.println("Object Not Scanned");
    }
    
    analogWrite(leftMotors, 0);
    analogWrite(rightMotors, 0);
    
    turn();
    
    objectDetected = false;
    timeOfLastDetection = millis();
    objectScanned = false;
    detectionCount = 0;
    resetSensors = true;
  }
}

void directionType(String type) {
  if(type == "forward") {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  }else if(type == "backward"){
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  }
}

void turn() {
  backUpSlightly();

  // Picks random direction for turn and random number of iterations
  int turnDirection = random(1,10);
  int numberOfIterations = random(2,10);

  if(debug) {
    Serial.print("turnDirection ");
    Serial.println(turnDirection);
    Serial.print("numberOfIterations ");
    Serial.println(numberOfIterations);
  }

  int numberOfTurns;
  for (numberOfTurns = 0; numberOfTurns < numberOfIterations; numberOfTurns++) {
     if (turnDirection < 3) {
      backOffSlightlyLeft();
      turnRight();
     } else {
      backOffSlightlyRight();
      turnLeft();
     }
  }
}

void backUpSlightly() {
  directionType("backward");
      long starttime = millis();
      long endtime = starttime;
      int duration = 800;
      int pos;
      int ease;
      while ((millis() - starttime) <= duration) // do this loop for up to 1000mS
      {
        pos = (millis() - starttime);
        ease = Easing::easeOutCubic(pos, 30, 90, duration);
        analogWrite(rightMotors, ease);
        analogWrite(leftMotors, ease);
      }
      analogWrite(leftMotors, 0);
      analogWrite(rightMotors, 0);
}

void backOffSlightlyLeft() {
      directionType("backward");
      long starttime = millis();
      long endtime = starttime;
      int duration = 1000;
      int pos;
      int ease;
      while ((millis() - starttime) <= duration) // do this loop for up to 1000mS
      {
        pos = (millis() - starttime);
        ease = Easing::easeOutCubic(pos, 100, 200, duration);
        analogWrite(rightMotors, 0);
        analogWrite(leftMotors, ease);
      }
      analogWrite(leftMotors, 0);
      analogWrite(rightMotors, 0);
}

void backOffSlightlyRight() {
      directionType("backward");
      long starttime = millis();
      long endtime = starttime;
      int duration = 1000;
      int pos;
      int ease;
      while ((millis() - starttime) <= duration) // do this loop for up to 1000mS
      {
        pos = (millis() - starttime);
        ease = Easing::easeOutCubic(pos, 100, 200, duration);
        analogWrite(rightMotors, ease);
        analogWrite(leftMotors, 0 );
      }
      analogWrite(leftMotors, 0);
      analogWrite(rightMotors, 0);
}

void turnRight() {
  directionType("forward");
      long starttime = millis();
      long endtime = starttime;
      solidStateRelay.on();
      int duration = 1000;
      int pos;
      int ease;
      while ((millis() - starttime) <= duration) // do this loop for up to 1000mS
      {
        pos = (millis() - starttime);
        ease = Easing::easeOutCubic(pos, 100, 200, duration);
        analogWrite(rightMotors, ease);
        analogWrite(leftMotors, 0);
      }
      analogWrite(leftMotors, 0);
      analogWrite(rightMotors, 0);
}

void turnLeft() {
  directionType("forward");
      long starttime = millis();
      long endtime = starttime;
      solidStateRelay.on();
      int duration = 1000;
      int pos;
      int ease;
      while ((millis() - starttime) <= duration) // do this loop for up to 1000mS
      {
        pos = (millis() - starttime);
        ease = Easing::easeOutCubic(pos, 100, 200, duration);
        analogWrite(rightMotors, 0);
        analogWrite(leftMotors, ease);
      }
      analogWrite(leftMotors, 0);
      analogWrite(rightMotors, 0);
}

void readSerialPort() {
  char characterRead;
      while (Serial.available()) {        
      characterRead = char(Serial.read());
        msg = msg + characterRead;
  
        if(characterRead == ')'){
         checkCommand(msg);
         msg="";
        }
    }
}

void moveServo(){
  int dur = 80; //duration is 100 loops
  for (int pos=0; pos<dur; pos++){
    //move servo from 0 and 140 degrees forward
    laserServo.write(Easing::easeInOutCubic(pos, 0, 100, dur));
    delay(15); //wait for the servo to move
  }

  delay(1000); //wait a second, then move back using "bounce" easing

  for (int pos=0; pos<dur; pos++){
    //move servo -140 degrees from position 140 (back to 0)
    laserServo.write(Easing::easeOutBounce(pos, 100, -100, dur));
    delay(15);
  }
}

void checkLastDetectionTimer() {
  if (timeOfLastDetection == 0) {
    timeOfLastDetection = millis();
  }

if(timeOfLastDetection + (15 * 1000) < millis()) {
    timeOfLastDetection = 0;
    if (debug) {
     Serial.println("Probably stuck, try to get unstuck"); 
    }
    turn();
    resetSensors = true;
  }
}

void checkExploreTimer() {

  if (explorationTimer == 0) {
    if (debug) {
     Serial.println("Entering explore mode"); 
    }
    explorationTimer = millis() + (exploreMinutes * 60 * 1000);

    if (debug) {
      Serial.print("exploreMinutes");
    Serial.println(exploreMinutes);
    Serial.print("exploreMinutes * 60 * 1000 ");
    Serial.println(exploreMinutes * 60 * 1000);
    Serial.print("millis ");
  Serial.println(millis());
    }
    
  }

  if (debug) {
   Serial.print("explorationTimer ");
  Serial.println(explorationTimer);
  Serial.print("millis ");
  Serial.println(millis());
  }
 

  if(explorationTimer < millis()) {
    sleeping = true;
    explorationTimer = 0;
    solidStateRelay.off();
    if (debug) {
     Serial.println("Entering sleep mode"); 
    }
  }
}

void checkPIR() {
  if (digitalRead(PIR_SENSOR)) {
    if(debug) {
     Serial.print("PIR SENSOR ");
     Serial.println(digitalRead(PIR_SENSOR)); 
    }
    sleeping = false;
    solidStateRelay.on();
  } 
}



