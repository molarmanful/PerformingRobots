#include "Arduino.h"
#include "stdint.h"
#include "SPI.h"
#include "Wire.h"
#include "SCMD.h"
#include "SCMD_config.h"
#include "SparkFun_Qwiic_Button.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "SparkFun_Qwiic_MP3_Trigger_Arduino_Library.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

SCMD driver;
QwiicButton button;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MP3TRIGGER mp3;

unsigned long time;
unsigned long zero;
boolean acting;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");

  Wire.begin();

  // motor driver settings
  driver.settings.commInterface = I2C_MODE;
  driver.settings.I2CAddress = 0x5D;
  driver.settings.chipSelectPin = 10;

  // connect to + initialize motors
  while (driver.begin() != 0xA9) {
    Serial.println("ID mismatch");
    delay(500);
  }
  Serial.println("ID match (0xA9)");
  Serial.print("Waiting for enumeration...");
  while (!driver.ready());
  Serial.println("Motors connected");
  while (driver.busy());
  driver.inversionMode(1, 1);
  while (driver.busy());
  driver.enable();

  // connect to button
  if (!button.begin()) {
    Serial.println("Button didn't connect");
    while (1);
  }
  Serial.println("Button connected");

  // connect to display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println("Display didn't connect");
    while (1);
  }
  Serial.println("Display connected");

  //connect to sound
  if(!mp3.begin()){
    Serial.println("Qwiic MP3 failed to respond. Please check wiring and possibly the I2C address. Freezing...");
    while(1);
  }
  Serial.println("MP3 connected");

  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.cp437(true);

  // resting face
  dcprint("\x03   \x03\n  u");

  driver.setDrive(LEFT_MOTOR, 0, 0);
  driver.setDrive(RIGHT_MOTOR, 0, 0);
  pinMode(13, OUTPUT);

  zero = millis();
}

void loop() {
  // allows for repetition of the routine
  if(button.isPressed() && !acting){
    routine();
  }
}

void routine() {
  acting = true;

  raisehand(LEFT_MOTOR, 200, 125);

  mp3.setVolume(10);
  mp3.playTrack(1);
  reset();

  at(7500);
  raisehand(RIGHT_MOTOR, 200, 125);

  at(17500);
  lowerhands(200, 125);
  
  at(26500);
  raisehands(100, 125);

  at(33000);
  lowerhands(100, 125);

  at(40000);
  raisehands(100, 125);

  at(52000);
  lowerhand(LEFT_MOTOR, 100, 125);

  at(54500);
  lowerhand(RIGHT_MOTOR, 100, 125);\

  at(59500);
  dcprint(F("\x0F   \x0F\n  O"));

  at(65500);
  raisehand(RIGHT_MOTOR, 100, 125);
  
  at(74000);
  lowerhand(RIGHT_MOTOR, 100, 125);
  
  acting = false;
}

// all of the helper functions
// includes facial expressions, printing, and actions

void at(unsigned long ms){
  unsigned long sms = millis();
  for(unsigned long cms = millis(); cms < ms + zero; cms = millis()){
    dcprint("\x03   \x03\n  u");
  }
  dcprint("^   ^\n  \x1f");
}

void reset(){
  zero = millis();
}

void raisehands(int s, int n){
  driver.setDrive(LEFT_MOTOR, 1, s);
  driver.setDrive(RIGHT_MOTOR, 1, s);
  delay(n);
  driver.setDrive(LEFT_MOTOR, 0, 0);
  driver.setDrive(RIGHT_MOTOR, 0, 0);
}

void lowerhands(int s, int n){
  driver.setDrive(LEFT_MOTOR, 0, s);
  driver.setDrive(RIGHT_MOTOR, 0, s);
  delay(n);
  driver.setDrive(LEFT_MOTOR, 0, 0);
  driver.setDrive(RIGHT_MOTOR, 0, 0);
}

void raisehand(int side, int s, int n){
  driver.setDrive(side, 1, s);
  delay(n);
  driver.setDrive(side, 0, 0);
}

void lowerhand(int side, int s, int n){
  driver.setDrive(side, 0, s);
  delay(n);
  driver.setDrive(side, 0, 0);
}

void dcprint(String s) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(s);
  display.display();
}
