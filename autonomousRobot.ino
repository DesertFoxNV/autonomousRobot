/*
 Example sketch for the Xbox Wireless Reciver library - developed by Kristian Lauszus
 It supports up to four controllers wirelessly
 For more information see the blog post: http://blog.tkjelectronics.dk/2012/12/xbox-360-receiver-added-to-the-usb-host-library/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#define rightMotorPin1 4
#define rightMotorPin2 5
#define rightMotors 6
#define leftMotorPin1 7
#define leftMotorPin2 3
#define leftMotors 2

//Device ID
String deviceID = "Ro Ro";

//Libraries
#include <binarySwitchDevice.h>
#include <XBOXRECV.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include <Servo.h>

// Global Variables
USB Usb;
XBOXRECV Xbox(&Usb);

Servo laserServo;

binarySwitchDevice solidStateRelay(15, HIGH, LOW);
binarySwitchDevice laser(35, HIGH, LOW);

int speed;
int direction;
boolean funMode = false;
String msg = "";

void setup() {
  setPins();
  speed = 0;
  direction = 1;

  Serial.begin(115200);
  getID();

  // Check if USB Host Driver Started
  if (Usb.Init() == -1) {
    Serial.println(F("\r\nOSC did not start"));
  }
}

void loop() {
  if (funMode) {
    xboxController();
  } else {
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
}
