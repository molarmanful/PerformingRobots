#include <Arduino.h>
#include <stdint.h>
#include <SparkFun_Qwiic_Button.h>
#include "SCMD.h"
#include "SCMD_config.h"
#include "Wire.h"

QwiicButton button;
SCMD motor;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting sketch.");
  Wire.begin();

  // BUTTON SETUP START
  
  if(!button.begin()){
    Serial.println("Button failed to connect");
    while(1);
  }

  // BUTTON SETUP END
  
  // MOTOR SETUP START
  
  motor.settings.commInterface = I2C_MODE;
  motor.settings.I2CAddress = 0x5D;
  motor.settings.chipSelectPin = 10;
  while (motor.begin() != 0xA9){
    Serial.println( "ID mismatch, trying again" );
    delay(500);
  }
  Serial.println( "ID matches 0xA9" );

  Serial.print("Waiting for enumeration...");
  while(!motor.ready());
  Serial.println("Done.");
  Serial.println();

  while(motor.busy());
  motor.enable();
  
  //MOTOR SETUP END

  motor.setDrive(0, 0, 0);
}

void loop()
{
  if(button.isPressed()){
    motor.setDrive(0, 1, 255);
    delay(300);
    motor.setDrive(0, 0, 0);
  }
}
