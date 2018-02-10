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
#define MOTORS_FORWARD 1
#define MOTORS_BACKWARD 2
#define MOTORS_LEFT 3
#define MOTORS_RIGHT 4
#define PIR_SENSOR 46
#define FUN_MODE_PIN 53

//Device ID
String deviceID = "Ro Ro";

//Libraries
#include <binarySwitchDevice.h>
#include <XBOXRECV.h>
#include <Easing.h>
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
boolean debug = false;
boolean objectScanned = false;
boolean objectDetected = false;
boolean resetSensors = false;
long sensorResetTimer = 0;
long sensorResetDelay = 1000; // milliseconds
int detectionCount = 0;
int detectionDistance = 40;
int sideDistance = 10;
int exploreSpeed = 80;
boolean lowVoltage = false;
boolean funMode = false;
String msg = "";
unsigned long pingTimer[ultraSonicSensorNumber]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[ultraSonicSensorNumber];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.
double exploreMinutes = 2;
long explorationTimer = 0;
long timeOfLastDetection = 0;
long lowVoltageCount = 50;
boolean sleeping = true;

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
    if (debug) {
      Serial.println(F("\r\nOSC did not start"));
    } 
  }
  
  solidStateRelay.on();

  Serial.println("Starting in 10 seconds");
  delay(1000);
  for(int i = 1; i < 10; i++) {
    Serial.print(10 - i);
    Serial.println(" Seconds");
    delay(1000);
  }

  if(digitalRead(FUN_MODE_PIN)) {
    funMode = true;
  }
  Serial.println("Entering sleep mode");
}

void loop() {

  // Check voltage
  readVoltage();

  // Check if in fun mode
  if (funMode) {
    xboxController();
    return;
  }

  // Check and see if robot is in sleep mode
  if(sleeping) {
    checkPIR(); // Exit sleep mode if PIR sensor is triggered
    return;
  }

  // Check ultrasonic sensors
  checkUltraSonicSensors();

  // Turn on motors and search for objects with ultra sonic sensors
  explore();

  checkExploreTimer();

  checkLastDetectionTimer();
}
