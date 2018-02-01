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
#define servoPin 34
#define ultraSonicSensorNumber 4
#define maxDistance 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33

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
#include <NewPing.h>

// Global Variables
USB Usb;
XBOXRECV Xbox(&Usb);

Servo laserServo;

binarySwitchDevice solidStateRelay(15, HIGH, LOW);
binarySwitchDevice laser(35, HIGH, LOW);

NewPing ultraSonicSensor[ultraSonicSensorNumber] = {   // Sensor object array.
  NewPing(36, 37, maxDistance), // Front
  NewPing(38, 39, maxDistance), // Left
  NewPing(40, 41, maxDistance), // Back
  NewPing(42, 43, maxDistance) // Right
};

int speed;
int direction;
boolean funMode = false;
String msg = "";
unsigned long pingTimer[ultraSonicSensorNumber]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[ultraSonicSensorNumber];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

void setup() {
  setPins();
  speed = 0;
  direction = 1;

  Serial.begin(115200);
  getID();
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < ultraSonicSensorNumber; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;

  // Check if USB Host Driver Started
  if (Usb.Init() == -1) {
    Serial.println(F("\r\nOSC did not start"));
  }
}

void loop() {
  checkSensors();
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

