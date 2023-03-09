
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 1

#include "frame_00.h"
#include "frame_01.h"
#include "frame_02.h"
#include "frame_03.h"
#include "frame_04.h"
#include "frame_05.h"
#include "frame_06.h"
#include "frame_07.h"
#include "frame_08.h"
#include "frame_09.h"
#include "frame_10.h"
#include "frame_11.h"

#include "macos_face.h"
#include "rtc.h"

#include <Fonts/FreeMono24pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>


// any pins can be used
#define SHARP_SCK     6
#define SHARP_MOSI    7
#define SHARP_SS      8
#define SHARP_DISP    10


// Set the size and color depth, e.g. 3 bits for LS013B7DH06 (8 colors 128x128 display)
Adafruit_SharpMem display(&SPI, SHARP_SS, 128, 128, 1, 2000000); // 2100000 ok, 2500000 wrong, 230000 LDO ok

// Set the size of the display here, e.g. 144x168!
// Adafruit_SharpMem display(SHARP_SCK, SHARP_MOSI, SHARP_SS, 144, 168);
// The currently-available SHARP Memory Display (144x168 pixels)
// requires > 4K of microcontroller RAM; it WILL NOT WORK on Arduino Uno
// or other <4K "classic" devices!  The original display (96x96 pixels)
// does work there, but is no longer produced.

#define BLACK 0
#define WHITE 1

int minorHalfSize; // 1/2 of lesser of display width or height

void graphic_tests() {
  // draw a single pixel
  display.drawPixel(10, 10, BLACK);
  display.refresh();
  Serial.println("Hello!");
  delay(500);
  display.clearDisplay();

  // draw many lines
  testdrawline();
  delay(500);
  display.clearDisplay();

  // draw a circle, 10 pixel radius
  display.fillCircle(display.width() / 2, display.height() / 2, 10, BLACK);
  display.refresh();
  delay(500);
  display.clearDisplay();

  testdrawchar();
  display.refresh();
  for (int i = 0; i < 4; i++) {
    display.refresh();
    delay(500);
  }

  test_rotation_text();
  display.clearDisplay();

  testdrawtriangle();
  display.refresh();
  delay(500);
  display.clearDisplay();

  testfilltriangle();
  display.refresh();
  delay(500);
  display.clearDisplay();

  testdrawroundrect();
  display.refresh();
  delay(500);
  display.clearDisplay();

  testfillroundrect();
  display.refresh();
  delay(500);
  display.clearDisplay();

  // draw rectangles
  testdrawrect();
  delay(500);
  display.clearDisplay();

  // draw multiple rectangles
  testfillrect();
  display.refresh();
  delay(500);
  // display.clearDisplay();
}

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  1        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR char stringToKeep[20];

long boot_time;

RTC_DATA_ATTR uint32_t last_cost;

const uint8_t * frames[] = {
  nyan_128x93_frame_00,
  nyan_128x93_frame_01,
  nyan_128x93_frame_02,
  nyan_128x93_frame_03,
  nyan_128x93_frame_04,
  nyan_128x93_frame_05,
  nyan_128x93_frame_06,
  nyan_128x93_frame_07,
  nyan_128x93_frame_08,
  nyan_128x93_frame_09,
  nyan_128x93_frame_10,
  nyan_128x93_frame_11,
};

void show_colors() {
  uint8_t color = BLACK;
  for (int i = 0; i < minorHalfSize; i += 8) {
    // alternate colors
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color & 0b111);
    display.refresh();
    color++;
  }
  delay(10000);
}

void show_anim() {
  display.clearDisplay();
  while (1) {
    for ( size_t i = 0; i < sizeof(frames) / sizeof(frames[0]); i++) {
      display.drawRGBBitmap(0, 0, (uint16_t*)frames[i], 128, 93);
      display.refresh();
      // delay(100);
    }
  }
}


void show_macos_time() {
  while (1) {
    display.drawRGBBitmap(0, 0, (uint16_t*)macos_128x128, 128, 128);
    display.fillRect(3, 37, 107, 33, 0b100);
    display.setCursor(3, 37 + 25);
    display.setTextSize(1);
    // display.setFont(&FreeMono24pt7b);
    display.setFont(&FreeMonoBold12pt7b);
    display.setTextColor(0b111);
    char buf[16];
    DateTime now = rtc.now();
    snprintf(buf, sizeof(buf) / sizeof(buf[0]), "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    display.println(buf);

    display.refresh();
    delay(1000);
  }
}


void show_time() {
  DateTime now = rtc.now();

  display.setRotation(0);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Hello, world!");
  display.setTextColor(WHITE, BLACK); // inverted text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.print("cost: "); display.println(last_cost, HEX);

  display.print(now.year(), DEC);
  display.print('/');
  display.print(now.month(), DEC);
  display.print('/');
  display.println(now.day(), DEC);

  display.print(now.hour(), DEC);
  display.print(':');
  display.print(now.minute(), DEC);
  display.print(':');
  display.print(now.second(), DEC);
  display.println();

  // sprintf(stringToKeep, "string");
  display.refresh();
  last_cost = millis() - boot_time;
}


void setup(void) {
  boot_time = millis();
  Serial.begin(115200);
  pinMode(SHARP_DISP, OUTPUT);
  digitalWrite(SHARP_DISP, HIGH);
  SPI.begin(SHARP_SCK, 11, SHARP_MOSI, SHARP_SS);

  // start & clear the display
  display.begin();
  display.clearDisplay();

  // Several shapes are drawn centered on the screen.  Calculate 1/2 of
  // lesser of display width or height, this is used repeatedly later.
  minorHalfSize = min(display.width(), display.height()) / 2;
  init_rtc();
  // show_anim();
  // show_time();
  display.setRotation(0);
  while (1) {
    // show_anim();
    show_macos_time();
    // show_time();
  }
}

void loop(void)
{
  Serial.println(last_cost);
  // delay(500);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void test_rotation_text() {
  for (int i = 0; i < 4; i++) {
    display.setRotation(i);
    display.clearDisplay();
    // text display tests
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0, 0);
    display.println("Hello, world!");
    display.setTextColor(WHITE, BLACK); // inverted text
    display.println(3.141592);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.print("0x"); display.println(0xDEADBEEF, HEX);
    // Screen must be refreshed at least once per second
    for (int j = 0; j < 4; j++) {
      display.refresh();
      delay(500); // 1/2 sec delay
    } // x4 = 2 second pause between rotations
  }
}


void testdrawline() {
  for (int i = 0; i < display.width(); i += 4) {
    display.drawLine(0, 0, i, display.height() - 1, BLACK);
    display.refresh();
  }
  for (int i = 0; i < display.height(); i += 4) {
    display.drawLine(0, 0, display.width() - 1, i, BLACK);
    display.refresh();
  }
  delay(250);

  display.clearDisplay();
  for (int i = 0; i < display.width(); i += 4) {
    display.drawLine(0, display.height() - 1, i, 0, BLACK);
    display.refresh();
  }
  for (int i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(0, display.height() - 1, display.width() - 1, i, BLACK);
    display.refresh();
  }
  delay(250);

  display.clearDisplay();
  for (int i = display.width() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, i, 0, BLACK);
    display.refresh();
  }
  for (int i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, 0, i, BLACK);
    display.refresh();
  }
  delay(250);

  display.clearDisplay();
  for (int i = 0; i < display.height(); i += 4) {
    display.drawLine(display.width() - 1, 0, 0, i, BLACK);
    display.refresh();
  }
  for (int i = 0; i < display.width(); i += 4) {
    display.drawLine(display.width() - 1, 0, i, display.height() - 1, BLACK);
    display.refresh();
  }
  delay(250);
}

void testdrawrect(void) {
  for (int i = 0; i < minorHalfSize; i += 2) {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, BLACK);
    display.refresh();
  }
}

void testfillrect(void) {
  uint8_t color = BLACK;
  for (int i = 0; i < minorHalfSize; i += 3) {
    // alternate colors
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color & 0b111);
    display.refresh();
    color++;
  }
}

void testdrawroundrect(void) {
  for (int i = 0; i < minorHalfSize / 2; i += 2) {
    display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, minorHalfSize / 2, BLACK);
    display.refresh();
  }
}

void testfillroundrect(void) {
  uint8_t color = BLACK;
  for (int i = 0; i < minorHalfSize / 2; i += 2) {
    display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, minorHalfSize / 2, color & 0b111);
    display.refresh();
    color++;
  }
}

void testdrawtriangle(void) {
  for (int i = 0; i < minorHalfSize; i += 5) {
    display.drawTriangle(display.width() / 2, display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, BLACK);
    display.refresh();
  }
}

void testfilltriangle(void) {
  uint8_t color = BLACK;
  for (int i = minorHalfSize; i > 0; i -= 5) {
    display.fillTriangle(display.width() / 2  , display.height() / 2 - i,
                         display.width() / 2 - i, display.height() / 2 + i,
                         display.width() / 2 + i, display.height() / 2 + i, color & 0b111);
    display.refresh();
    color++;
  }
}

void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.cp437(true);

  for (int i = 0; i < 256; i++) {
    if (i == '\n') continue;
    display.write(i);
  }
  display.refresh();
}
