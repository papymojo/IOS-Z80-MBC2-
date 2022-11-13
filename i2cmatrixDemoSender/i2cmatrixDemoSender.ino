#include <Wire.h>
#include <DigiCDC.h>

#define ADDR 42

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  SerialUSB.begin();
  Wire.beginTransmission(ADDR);
  if (Wire.endTransmission() == 0) {
    SerialUSB.println("F");
  } else {
    SerialUSB.println("NF");
  }
}

void loop() {
  Wire.beginTransmission(ADDR);
  Wire.write(0xF0);
  Wire.write(43);
  Wire.write(40);
  Wire.endTransmission(true);
  SerialUSB.delay(400);
  Wire.beginTransmission(ADDR);
  Wire.write(0xF0);
  Wire.write(45);
  Wire.write(40);
  Wire.endTransmission(true);
  SerialUSB.delay(400);
  Wire.beginTransmission(ADDR);
  Wire.write(0xF0);
  Wire.write(41);
  Wire.write(40);
  Wire.endTransmission(true);
  SerialUSB.delay(400);
  Wire.beginTransmission(ADDR);
  Wire.write(0xF0);
  Wire.write(19);
  Wire.write(50);
  Wire.endTransmission(true);
  SerialUSB.delay(300);
  Wire.beginTransmission(ADDR);
  Wire.write(0xF0);
  Wire.write(36);
  Wire.write(100);
  Wire.endTransmission(true);
  SerialUSB.delay(700);

  for (int f = 0; f < 128; f++) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        int color = random(0, 15);
        Wire.beginTransmission(ADDR);
        Wire.write(0xFF);
        Wire.write(i);
        Wire.write(j);
        Wire.write(color);
        Wire.endTransmission(true);
      }
    }
    Wire.beginTransmission(ADDR);
    Wire.write(0xFD);
    Wire.endTransmission(true);
  }
  int color = random(0, 15);
  Wire.beginTransmission(ADDR);
  Wire.write(0xFE);
  Wire.write(color);
  Wire.endTransmission(true);
  Wire.beginTransmission(ADDR);
  Wire.write(0xFD);
  Wire.endTransmission(true);
  SerialUSB.delay(2000);
  Wire.beginTransmission(ADDR);
  Wire.write(0xF0);
  Wire.write(26);
  Wire.write(10);
  Wire.endTransmission(true);
  SerialUSB.delay(100);
  Wire.beginTransmission(ADDR);
  Wire.write(0xF0);
  Wire.write(24);
  Wire.write(10);
  Wire.endTransmission(true);
  SerialUSB.delay(100);
  Wire.beginTransmission(ADDR);
  Wire.write(0xF0);
  Wire.write(30);
  Wire.write(40);
  Wire.endTransmission(true);
  SerialUSB.delay(500);
  for (int i = 0; i < 4096; i++) {
    int x = random(0, 8);
    int y = random(0, 8);
    int color = random(0, 16);
    Wire.beginTransmission(ADDR);
    Wire.write(0xFF);
    Wire.write(x);
    Wire.write(y);
    Wire.write(color);
    Wire.endTransmission(true);
    Wire.beginTransmission(ADDR);
    Wire.write(0xFD);
    Wire.endTransmission(true);
    if (i % 128 == 0) {
      int tone = random(0, 96);
      int duration = random(0, 128);
      Wire.beginTransmission(ADDR);
      Wire.write(0xF0);
      Wire.write(tone);
      Wire.write(duration);
      Wire.endTransmission(true);
    }
  }
  SerialUSB.delay(2000);
  Wire.beginTransmission(ADDR);
  Wire.write(0xFE);
  Wire.write(0);
  Wire.endTransmission(true);
  Wire.beginTransmission(ADDR);
  Wire.write(0xFD);
  Wire.endTransmission(true);
  SerialUSB.delay(2000);
}