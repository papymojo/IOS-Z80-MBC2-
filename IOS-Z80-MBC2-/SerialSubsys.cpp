#include "SerialSubsys.h"
#include "HwDefines.h"


SerialSubsys::SerialSubsys(MediaSubsys &mediaSubsys)
  : lastRxIsEmpty(0), media(mediaSubsys) {
}

Opcode SerialSubsys::Read(Opcode opcode, byte &ioByte) {
  ioByte = 0xFF;
  if (Serial.available() > 0) {
    ioByte = Serial.read();

    // Reset the "Last Rx char was empty" flag
    lastRxIsEmpty = 0;
    media.sound((ioByte % 12) * 6, 1);
  } else {
    // Set the "Last Rx char was empty" flag
    lastRxIsEmpty = 1;
  }

  digitalWrite(INT_, HIGH);

  return NO_OP;
}

Opcode SerialSubsys::Write(Opcode opcode, byte ioByte) {
  switch (opcode) {
    case SERIAL_TX:
      Serial.write(ioByte);
      if (ioByte == '\n')
       media.sound(32, 2);
      break;

    default:
      lastOpcode = NO_OP;
      break;
  }

  return NO_OP;
}

byte SerialSubsys::LastRxIsEmpty() {
  return lastRxIsEmpty;
}

void SerialSubsys::LastRxIsEmpty(byte value) {
  lastRxIsEmpty = value;
}