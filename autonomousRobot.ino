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
#include <SoftwareSerial.h>
#include <binarySwitchDevice.h>
#include <XBOXRECV.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
XBOXRECV Xbox(&Usb);

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

SoftwareSerial HC12(44, 45); //TX, RX

binarySwitchDevice solidStateRelay(15, HIGH, LOW);


int speed;
int direction;
boolean funMode;


String msg = "";
void setup() {
  myservo.attach(37);  // attaches the servo on pin 9 to the servo object
  solidStateRelay.on();
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
  speed = 0;
  direction = 1;
  funMode = true;

  if (funMode) {
    solidStateRelay.on();
  }

  HC12.begin(9600);
//   Serial.begin(115200);
//#if !defined(__MIPSEL__)
//  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
//#endif
  if (Usb.Init() == -1) {
//    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
}

void loop() {
  if (funMode) {
    xboxController();
  } else {
    char characterRead;
    while (HC12.available()) {        
    characterRead = char(HC12.read());
      msg = msg + characterRead;

      if(characterRead == ')'){
       checkCommand(msg);
       msg="";
      }
  }
  }
}
