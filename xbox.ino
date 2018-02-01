void xboxController() {
  Usb.Task();
  if (Xbox.XboxReceiverConnected) {
    for (uint8_t i = 0; i < 4; i++) {
      if (Xbox.Xbox360Connected[i]) {
        if (Xbox.getButtonClick(UP, i)) {
          Xbox.setLedOn(LED1, i);
          digitalWrite(rightMotorPin1, HIGH);
          digitalWrite(rightMotorPin2, LOW);
          digitalWrite(leftMotorPin1, HIGH);
          digitalWrite(leftMotorPin2, LOW);
        }
        if (Xbox.getButtonClick(DOWN, i)) {
          Xbox.setLedOn(LED3, i);
          digitalWrite(rightMotorPin1, LOW);
          digitalWrite(rightMotorPin2, HIGH);
          digitalWrite(leftMotorPin1, LOW);
          digitalWrite(leftMotorPin2, HIGH);
        }
        
        if (Xbox.getButtonPress(L2, i) || Xbox.getButtonPress(R2, i)) {
          Serial.print("L2: ");
          Serial.print(Xbox.getButtonPress(L2, i));
          Serial.print("\tR2: ");
          Serial.println(Xbox.getButtonPress(R2, i));


          uint8_t right = Xbox.getButtonPress(L2, i);
          uint8_t left = Xbox.getButtonPress(R2, i);

          if (right < 50) {
            right = 0;
          }

          if (left <50) {
            left = 0;
          }
          
          Xbox.setRumbleOn(Xbox.getButtonPress(L2, i), Xbox.getButtonPress(R2, i), i);
          analogWrite(leftMotors, left);
          analogWrite(rightMotors, right);
        }

        if (Xbox.getButtonClick(X, i)) {
           scan();
        }
      }
    }
  }
}
