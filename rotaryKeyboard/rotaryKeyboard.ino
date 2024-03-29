#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include<Button2.h>
#include <MD_REncoder.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Keyboard.h>

MD_REncoder R = MD_REncoder(4, 5);
#define SW 7

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET     5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Button2 b = Button2(SW);
const int characterProType = 26;
int counter;
char ABCitems[4][characterProType] = {
  { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' },
  { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' },
  { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '-', '*', '/', '%', '=', '(', ')', '{', '}', '[', ']', '<', '>', '+', '-' },
  { ',', ';', '.', '!', '?', '@', '&', '#', ',', ';', '.', '!', '?', '@', '&', '#', ',', ';', '.', '!', '?', '@', '&', '#', '!', '?' }
};
char selectedABC[characterProType];
char selected;
boolean doubleClicked;
int selectedABCitemsIndex;


void setup() {
  delay(100);
  Serial.begin(9600);
  b.setClickHandler(clickHandler);
  b.setDoubleClickHandler(doubleClickHandler);
  b.setLongClickHandler(longClickHandler);
  R.begin();
  counter = 0;
  selectedABCitemsIndex = 0;
  memcpy(selectedABC, ABCitems[0], characterProType );
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    drawDisplay(counter);
    display.display();
  }
}

void loop() {
  b.loop();
  uint8_t x = R.read();
  if (x == DIR_CW)
  {
    counter = counter + 1;
    drawDisplay(counter);
  }
  else if (x == DIR_CCW)
  {
    counter = counter - 1;
    drawDisplay(counter);
  }
}

void drawDisplay(int counter) {
  int index = counter >= 0 ? counter % characterProType : counter % characterProType == 0 ? counter % characterProType : characterProType - abs(counter) % characterProType;
  selected = selectedABC[index];
  display.clearDisplay();
  display.setFont(&FreeMonoBold18pt7b);
  display.setCursor (52, 24);
  display.print(selected);
  displayOtherChars(index);
  display.display();
}

void displayOtherChars(int index) {
  int beforeFirst = index - 1;
  if ( beforeFirst < 0 ) {
    beforeFirst = beforeFirst + characterProType;
  }
  int beforeSecond = index - 2;
  if ( beforeSecond < 0 ) {
    beforeSecond = beforeSecond + characterProType;
  }
  int beforeThird = index - 3;
  if ( beforeThird < 0 ) {
    beforeThird = beforeThird + characterProType;
  }
  int afterFirst = index + 1;
  if ( afterFirst >=  sizeof(selectedABC) ) {
    afterFirst = afterFirst - characterProType;
  }
  int afterSecond = index + 2;
  if ( afterSecond >=  sizeof(selectedABC) ) {
    afterSecond = afterSecond - sizeof(selectedABC);
  }
  int afterThird = index + 3;
  if ( afterThird  >=  sizeof(selectedABC) ) {
    afterThird = afterThird - characterProType;
  }

  display.setFont(&FreeMono12pt7b);
  display.setCursor (34, 24);
  display.print(selectedABC[beforeFirst]);
  display.setCursor (77, 24);
  display.print(selectedABC[afterFirst]);
  display.setFont(&FreeMono9pt7b);

  display.setCursor (20, 26);
  display.print(selectedABC[beforeSecond]);
  display.setCursor (96, 26);
  display.print(selectedABC[afterSecond]);

  display.setCursor (3, 28);
  display.print(selectedABC[beforeThird]);
  display.setCursor (112, 28);
  display.print(selectedABC[afterThird]);
}

void clickHandler(Button2& btn) {
  if (doubleClicked) {
    Keyboard.write(selected);
  } else {
    Serial.print(selected);
  }
}

void doubleClickHandler(Button2& btn) {
  doubleClicked = !doubleClicked;
}
void longClickHandler(Button2& btn) {
  unsigned int time = btn.wasPressedFor();
  counter = 0;
  selectedABCitemsIndex += 1;
  if (time > 300) {
    memcpy(selectedABC, ABCitems[selectedABCitemsIndex % 4], characterProType);
    drawDisplay(counter);
  }
}
