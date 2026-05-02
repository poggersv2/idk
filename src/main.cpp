#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BleKeyboard.h>
BleKeyboard bleKeyboard;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LAYOUT_LENGTH 28
#define KEYBOARD_LAYOUT "QWERTYUIOP\nASDFGHJKL\nZXCVBNM"
#define MARGIN 2

byte selectedChar = 0;

void drawKeyboard() {
  // Clear the buffer
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  byte line = 0;
  byte x = MARGIN;
  for(int16_t i=0; i<LAYOUT_LENGTH; i++) {
    if(KEYBOARD_LAYOUT[i] == '\n') {
      line ++;
      x = MARGIN;
      display.setCursor(0, line*10);
    } else {
      display.setCursor(x, display.getCursorY());
      if (i == selectedChar) {
        display.fillRect(display.getCursorX() -1, display.getCursorY()-1, 7, 9, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
        display.write(KEYBOARD_LAYOUT[i]);
        display.setTextColor(SSD1306_WHITE);
      } else {
        display.write(KEYBOARD_LAYOUT[i]);
      }
      x += 7;
    }

  }

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
}

// const int horizontal_pot = 27;
// const int vertical_pot = 26;
const int BAUD_RATE = 115200;

void setup() {
  Serial.begin(BAUD_RATE);
  bleKeyboard.begin();

  // initialize potentiometer
  // pinMode(horizontal_pot, INPUT);
  // pinMode(vertical_pot, INPUT);
  // Wait for display
  delay(500);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

void loop() {
  delay(200);
  if (!bleKeyboard.isConnected()) return;
  Serial.println("Writing to keyboard");
  // int horizontal_value = analogRead(horizontal_pot);
  // int vertical_value = analogRead(vertical_pot);

  // Serial.print("Horizontal Potentiometer Value: ");
  // Serial.println(horizontal_value);

  // selectedChar = 28 * ((float)horizontal_value / (float)4095);
  if (selectedChar > 28) selectedChar = 0;
  drawKeyboard();
  bleKeyboard.print(KEYBOARD_LAYOUT[selectedChar]);
  delay(200);
}