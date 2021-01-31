#if !defined(_RTCSUBSYS_H_)
#define _RTCSUBSYS_H_
#include <time.h>
#include "BaseSubsys.h"

class RtcSubsys : BaseSubsys
{
public:
  RtcSubsys();
  
  Opcodes Read(Opcodes opcode, byte &ioByte);
  Opcodes Write(Opcodes opcode, byte ioByte);
  void Reset(Opcodes opcode);

  void foundRtc(byte found);
  byte foundRtc();

private:
  Opcodes ReadTime(byte &ioByte);

private:
  byte rtcFound;
  struct tm curTime;
  int8_t tempC;
};
#endif