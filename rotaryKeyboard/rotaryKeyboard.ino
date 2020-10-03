#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include<Button2.h>
#include <MD_REncoder.h>
//rotary
MD_REncoder R = MD_REncoder(2, 3);
#define SW 4
//oled screen
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32
#define OLED_RESET     5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Button2 b = Button2(SW);
int counter;
String rotaryDirection;
char ABC[] = { 'A', 'B', 'C' };

void setup() {
    // Setup Serial Monitor
  Serial.begin(9600);
  b.setTapHandler(click);
  R.begin();
  counter = 0;
  rotaryDirection = "";
  //oled start display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  //Address 0x3C for 128x32
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } else {
     display.clearDisplay();
     display.setTextSize (1); 
     display.setTextColor (WHITE);
     display.display();
  }
}

void loop() {
  b.loop();
  uint8_t x = R.read();
  if (x == DIR_CW) 
  {
    rotaryDirection = "Clockwise";
    counter = counter + 1;
    drawDisplay(rotaryDirection, counter);
  } 
  else if (x == DIR_CCW)
  {
    rotaryDirection = "CounterClockwise";
    counter = counter - 1;
    drawDisplay(rotaryDirection, counter);
  }
}

void drawDisplay(String rotaryDirection, int counter){
  int index = counter >= 0 ? counter % 3 : counter % 3 == 0 ? counter % 3 : 3 - abs(counter) % 3;
  char selected = ABC[index];
  display.clearDisplay();
  display.setTextSize (1);
  display.setCursor (1,16);
  display.print(selected);
  display.display();
}

void click(Button2& btn) {
  Serial.println("Click!");
  display.setCursor(2,25);
  display.print("Button pressed");
  display.display();
}
