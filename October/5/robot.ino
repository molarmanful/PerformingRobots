#include <Arduino.h>
#include <stdint.h>
#include "SCMD.h"
#include "SCMD_config.h"
#include "Wire.h"
#include "SparkFun_Qwiic_MP3_Trigger_Arduino_Library.h"

MP3TRIGGER mp3;
SCMD driver;

void setup(){
  Serial.begin(9600);
  Serial.println("Starting");

  Wire.begin();

  // initialize mp3
  if(!mp3.begin())
  {
    Serial.println("MP3 ERROR");
    while(1);
  }
  mp3.setVolume(10);

  // motor driver settings
  driver.settings.commInterface = I2C_MODE;
  driver.settings.I2CAddress = 0x5A;
  driver.settings.chipSelectPin = 10;

  // connect to + initialize motors
  while(driver.begin() != 0xA9){
    Serial.println("ID mismatch");
    delay(500);
  }
  Serial.println("ID match (0xA9)");

  Serial.print("Waiting for enumeration...");
  while(!driver.ready());
  Serial.println("Done.");
  
  while(driver.busy());
  driver.inversionMode(1, 1);
  
  while(driver.busy());
  driver.enable();
}

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1
int i = 0;

void loop(){

  // likely not necessary but put as a precaution
  if(i == 0){
    driver.setDrive(LEFT_MOTOR, 0, 0);
    driver.setDrive(RIGHT_MOTOR, 0, 0);
  }

  // looks odd, but it just smoothly ramp up speed
  // this is skipped in subsequent iterations of loop()
  // left motor turns slightly slower to create wide left-circular turn
  for(; i < 100; i++){
    driver.setDrive(LEFT_MOTOR, 0, 15 + i / 2);
    driver.setDrive(RIGHT_MOTOR, 0, 20 + i / 2);
    delay(5);
  }

  // once motor is at full speed, begin looping song
  if(!mp3.isPlaying()){
    mp3.playTrack(1);
  }
}
