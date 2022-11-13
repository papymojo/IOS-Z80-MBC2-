#if !defined(_MEDIA_SUBSYS_H_)
#define _MEDIA_SUBSYS_H_
#include <Arduino.h>
#include "Opcode.h"

class MediaSubsys {
public:
  MediaSubsys();
  byte probeMedia();

  void sound(byte tone, byte duration);
  Opcode fillMatrix(byte color);
  void setPixel(byte x, byte y, byte color);
  Opcode refreshMatrix();
  Opcode setPixelio(byte ioData);
  Opcode soundio(byte ioData);

private:
  uint8_t isAvailable;
};
#endif