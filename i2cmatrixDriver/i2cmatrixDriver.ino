#include <TimerFreeTone.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#define NEOPIXEL 6
#define BUZZER 13

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(64, NEOPIXEL, NEO_RGB + NEO_KHZ800);


int brightness = 12;
int note = 0, duration = 0;

const uint8_t PROGMEM gamma8[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
  2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
  5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

int palette[16][3] = {
  { 0x39, 0x34, 0x57 },
  { 0x6a, 0x37, 0x71 },
  { 0xcb, 0x4d, 0x68 },
  { 0xc9, 0x24, 0x64 },
  { 0x60, 0x6c, 0x81 },
  { 0x11, 0xad, 0xc1 },
  { 0x1e, 0x88, 0x75 },
  { 0x6d, 0xf7, 0xc1 },
  { 0xf9, 0x92, 0x52 },
  { 0xf7, 0xe4, 0x76 },
  { 0x5b, 0xb3, 0x61 },
  { 0xa1, 0xe5, 0x5a },
  { 0xf4, 0x8c, 0xb6 },
  { 0xf7, 0xb6, 0x9e },
  { 0x9b, 0x9c, 0x82 },
  { 0xff, 0xff, 0xff }
};

int instrument[96] = {
  33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62,
  65, 69, 73, 78, 82, 87, 93, 98, 104, 110, 117, 123,
  131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247,
  262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
  523, 554, 587, 622, 659, 699, 740, 784, 831, 880, 932, 988,
  1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1975,
  2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951,
  4186, 4435, 4698, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7458, 7902
};

void setPixelColor(int x, int y, int color) {
  pixels.setPixelColor((7 - x) + (y * 8), pixels.Color(
                                            pgm_read_byte(&gamma8[palette[color][0]]) / brightness,
                                            pgm_read_byte(&gamma8[palette[color][1]]) / brightness,
                                            pgm_read_byte(&gamma8[palette[color][2]]) / brightness));
}
void fill(int color) {
  pixels.fill(pixels.Color(
                pgm_read_byte(&gamma8[palette[color][0]]) / brightness,
                pgm_read_byte(&gamma8[palette[color][1]]) / brightness,
                pgm_read_byte(&gamma8[palette[color][2]]) / brightness),
              0, 64);
}

void receiveEvent(int howMany) {
  int inst = Wire.read();
  if (inst == 0xFF) {
    int x = Wire.read();
    int y = Wire.read();
    int color = Wire.read();
    if (x >= 0 && x < 8 && y >= 0 && y < 8 && color >= 0 && color < 16) {
      setPixelColor(x, y, color);
    }
  } else if (inst == 0xFE) {
    int color = Wire.read();
    fill(color);
  } else if (inst == 0xFD) {
    pixels.show();
  } else if (inst == 0xF0) {
    note = Wire.read();
    duration = Wire.read();
    if (!(note >= 0 && note < 96 && duration >= 0 && duration < 128)) {
      note = 0;
      duration = 0;
    }
  }
}

void setup() {
  Wire.begin(42);                // join I2C bus with address #8
  Wire.onReceive(receiveEvent);  // register event
  pixels.begin();
  int color = 0;
  fill(color);
  for (int x = 0; x < 8; x += 2) {
    for (int y = 0; y < 8; y += 2) {
      setPixelColor(x, y, color);
      color++;
      pixels.show();
      delay(25);
    }
  }
}

void loop() {
  if (duration > 0) {
    duration--;
    if (millis() % 60 <= 20) {
      TimerFreeTone(BUZZER, instrument[note + 12], 2);
    } else if (millis() % 60 >= 40) {
      TimerFreeTone(BUZZER, instrument[note + 24], 2);
    } else {
      TimerFreeTone(BUZZER, instrument[note], 2);
    }
  }
}