#include "Arduino.h"
#include "stdint.h"
#include "SPI.h"
#include "Wire.h"
#include "SCMD.h"
#include "SCMD_config.h"
#include "SparkFun_Qwiic_Button.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

SCMD driver;
QwiicButton button;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned long time;
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

  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.cp437(true);

  // resting face
  dcprint("_   _\n  _", 0);

  driver.setDrive(LEFT_MOTOR, 0, 0);
  driver.setDrive(RIGHT_MOTOR, 0, 0);
  pinMode(13, OUTPUT);
}

void loop() {
  // allows for repetition of the routine
  if (button.isPressed() && !acting) {
    routine();
  }
}

void routine() {
  acting = true;

  eyeopen(50, 1000);
  eyeblink(10, 200);
  eyeblink(10, 1000);
  eyeclose(50, 1000);

  oneopen(1000);
  dcprint("_   _\n  _", 1000);

  msg(F("HELLO..."), 3000);
  msg(F("STILL A\nBIT\nSLEEPY."), 3000);

  dcprint("_   _\n  _", 1000);
  eyeopen(50, 1000);
  eyeblink(10, 200);
  eyeblink(10, 1000);
  eyeblink(10, 200);
  eyeblink(10, 1000);

  msg(F("SYSTEM\nCHECK!"), 3000);

  msg(F("LIGHTS..."), 1000);
  lightcheck();
  nprint(F("PASSED!"), 3000);

  msg(F("DISPLAY..."), 1000);
  happy(10, 3000);
  msg(F("PASSED!"), 3000);

  msg(F("ARMS..."), 1000);
  armcheck();

  eyeclose(50, 0);
  
  acting = false;
}

// all of the helper functions
// includes facial expressions, printing, and actions

void dcprint(String s, int n) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(s);
  display.display();
  delay(n);
}

void eyeclose(int n, int o) {
  dcprint("o   o\n  _", n);
  dcprint("_   _\n  _", o);
}

void eyeopen(int n, int o) {
  dcprint("o   o\n  _", n);
  dcprint("O   O\n  _", o);
}

void eyeblink(int n, int o) {
  eyeclose(n, 0);
  eyeopen(n, o);
}

void oneopen(int n) {
  dcprint("o   _\n  _", n);
}

void happy(int n, int o) {
  dcprint("-   -\n  u", n);
  dcprint("^   ^\n  U", o);
}

void shocked(int n, int o) {
  dcprint("O   o\n  =", n);
  dcprint("o   O\n  -", o);
}

void broken(int n, int o) {
  dcprint("X   x\n  O", n);
  dcprint("x   X\n  o", o);
}

void msg(String s, int n) {
  display.clearDisplay();
  display.setTextSize(2);
  dcprint(s, n);
  display.setTextSize(4);
}

void nprint(String s, int n) {
  display.setTextSize(2);
  display.println(s);
  display.display();
  display.setTextSize(4);
  delay(n);
}

void lightcheck() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(13, HIGH);
    delay(10);
    digitalWrite(13, LOW);
    delay(100);
  }
}

void armcheck() {
  driver.setDrive(LEFT_MOTOR, 0, 100);
  driver.setDrive(RIGHT_MOTOR, 0, 80);
  for(int i = 0; i < 30; i++)
    shocked(50, 50);
  
  msg(F("MY ARMS\nDON'T WORK!"), 3000);
  
  driver.setDrive(LEFT_MOTOR, 0, 200);
  driver.setDrive(RIGHT_MOTOR, 0, 200);
  for(int i = 0; i < 50; i++)
    broken(50, 50);

  driver.setDrive(LEFT_MOTOR, 0, 0);
  driver.setDrive(RIGHT_MOTOR, 0, 0);
  msg(F("SHUT...\nDOWN..."), 3000);
  msg(F("WAKING...\nWAS A...\nMISTAKE."), 3000);
}
