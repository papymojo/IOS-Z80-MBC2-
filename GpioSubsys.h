#if !defined(_GPIOSUBSYS_H_)
#define _GPIOSUBSYS_H_
#include "BaseSubsys.h"

class GpioSubsys : public BaseSubsys
{
public:
  Opcodes Read(Opcodes opcode, byte &ioByte);
  Opcodes Write(Opcodes opcode, byte ioByte);
  void Reset(Opcodes opcode);
};

#endif
