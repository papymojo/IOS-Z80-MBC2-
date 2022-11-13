#include <Arduino.h>
#include "WireUtils.h"
#include "MediaSubsys.h"
#include <Wire.h>
#include "Opcode.h"
#define ADDR 42


MediaSubsys::MediaSubsys()
  : isAvailable(0) {
}

byte MediaSubsys::probeMedia() {
  this->isAvailable = ProbeAddress(ADDR);
  return this->isAvailable;
}

Opcode MediaSubsys::soundio(byte iodata) {
  static byte tone = 0;
  static byte state = 0;
  if (state == 0) {
    tone = iodata;
    state = 1;
  } else if (state == 1) {
    state = 0;
    this->sound(tone, iodata);
  }
  return SOUND;
}

void MediaSubsys::sound(byte tone, byte duration) {
  if (isAvailable == 1) {
    uint8_t sound_d[2] = { tone, duration };
    if (isAvailable == 1) {
      WriteRegisters(ADDR, 0xF0, 2, sound_d);
    }
  }
}

Opcode MediaSubsys::fillMatrix(byte color) {
  if (isAvailable == 1) {
    uint8_t color_d[1] = { color };
    if (isAvailable == 1) {
      WriteRegisters(ADDR, 0xFE, 1, color_d);
    }
  }
  return FILL_MATRIX;
}

Opcode MediaSubsys::setPixelio(byte iodata) {
  if (isAvailable == 1) {
    static byte x = 0;
    static byte y = 0;
    static byte state = 0;
    if (state == 0) {
      x = iodata;
      state = 1;
    } else if (state == 1) {
      y = iodata;
      state = 2;
    } else if (state == 2) {
      state = 0;
      this->setPixel(x, y, iodata);
    }
  }
  return S_PIXEL_MATRIX;
}

void MediaSubsys::setPixel(byte x, byte y, byte color) {
  if (isAvailable == 1) {
    uint8_t pixel_d[3] = { x, y, color };
    if (isAvailable == 1) {
      WriteRegisters(ADDR, 0xFF, 3, pixel_d);
    }
  }
}

Opcode MediaSubsys::refreshMatrix() {
  if (isAvailable == 1) {
    Wire.beginTransmission(ADDR);
    Wire.write(0xFD);
    Wire.endTransmission(true);
  }
  return REFRESH_MATRIX;
}