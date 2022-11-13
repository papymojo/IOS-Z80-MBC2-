#if !defined(_SERIALSUBSYS_H_)
#define _SERIALSUBSYS_H_
#include <TWILiquidCrystal.h>
#include "BaseSubsys.h"
#include "MediaSubsys.h"

class SerialSubsys : public BaseSubsys {
public:
  SerialSubsys(MediaSubsys &mediaSubsys);
  void init();

  Opcode Read(Opcode opcode, byte &ioByte);
  Opcode Write(Opcode opcode, byte ioByte);

  byte LastRxIsEmpty();
  void LastRxIsEmpty(byte value);

private:
  byte lastRxIsEmpty;
  MediaSubsys &media;
};
#endif