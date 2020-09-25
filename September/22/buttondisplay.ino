#include <SPI.h>
#include <Wire.h>
#include <SparkFun_Qwiic_Button.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

QwiicButton button;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned int count = 0;

void setup(){
  Serial.begin(115200);
  Wire.begin();

  // initialize button
  if(!button.begin()){
    Serial.println("Button fail");
    while(1);
  }
  Serial.println("Button ready");

  // initialize button
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)){
    Serial.println("SSD1306 fail");
    while(1);
  }
  Serial.println("SSD1306 ready");

  clrprint("READY"); // start screen
}

void loop(){
  if(button.isPressed()){
    count++;
    
    clrprint("PRESSED");
    Serial.println("pressed");

    // pause while button is pressed
    // prevents premature unpress event trigger
    while(button.isPressed()) delay(10);

    // display button press counter
    clrprint("PRESSED\n");
    display.print(count);
    display.print(count > 1 ? " TIMES" : " TIME");
    display.display();
    Serial.println("not pressed");
  }

  // ignore micropresses
  delay(20);
}

// helper function for clearing, styling, and displaying messages
void clrprint(String msg){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(msg);
  display.display();
}
