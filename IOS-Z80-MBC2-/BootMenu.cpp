#include <EEPROM.h>

#include "BootMenu.h"
#include "HwDefines.h"
#include "RtcUtilities.h"
#include "OsBootInfo.h"
#include "Utils.h"
#include "StringSplitter.h"

byte ChangeDiskSet(byte curDiskSet, byte maxDiskSet);

String removeLastChar(String s) {
    return (s.length() == 0)
      ? s
      : (s.substring(0, s.length() - 1));
}

void printHelp() {
  Serial.println("HELP");
  Serial.println("LIST");
  Serial.println("BASIC");
  Serial.println("FORTH");
  Serial.println("BOOT");
  Serial.println("AUTOBOOT");
  Serial.println("LOAD | ILOAD");
  Serial.println("SET");
  Serial.println("|-CLOCK");
  Serial.println("| |-HIGH");
  Serial.println("| |-LOW");
  Serial.println("|-AUTOBOOT");
  Serial.println("| |-ON");
  Serial.println("| |-OFF");
  Serial.println("|-DATE");
  Serial.println("|-BOOTDISK");
  Serial.println("| |-[DISK NUMBER]");
  Serial.println("SAVE");
}

byte BootMenu(byte bootMode, byte maxDiskSet, byte foundRTC, ConfigOptions &options)
{
  Serial.println("IOS: Launch Boot Shell\n\r");
  while(1){
    Serial.print("z80-mbc2plus_> ");
    FlushRxBuffer();
    String cmd = "";
    while(1) {
      unsigned long timeStamp = millis();
      while(Serial.available() ==0){
        blinkIOSled(&timeStamp);  // Wait a key
      }
      char input = Serial.read();
      if (input == 13) {
        Serial.println("");
        break;
      }      
      else if (input == 8) {
        if (cmd.length() > 0) {
        cmd = removeLastChar(cmd);
          Serial.write(8);
          Serial.write(' ');
          Serial.write(8);    
        }
      }
      else {
        cmd +=input;
        Serial.write(input);
      }
    }
    if(cmd.length()==0) {
      return 2;          
    }    
    StringSplitter *splitter = new StringSplitter(cmd,' ',3);
    if (splitter->getItemAtIndex(0).equalsIgnoreCase("HELP")) {
      printHelp();
      continue;
    }
    else if (splitter->getItemAtIndex(0).equalsIgnoreCase("LIST")) {
      for (int setNum = 0; setNum != maxDiskSet; ++setNum) {
        OsBootInfo tmpInfo = GetDiskSetBootInfo(setNum);
        Serial.printf(F(" %d: Disk Set %d (%s)\n\r"), setNum, setNum, tmpInfo.OsName);
      }
      continue;
    }
    else if (splitter->getItemAtIndex(0).equalsIgnoreCase("BASIC")) {
      options.BootMode = 0;
      EEPROM.put(configAddr, options);
      return 0;
    }
    else if (splitter->getItemAtIndex(0).equalsIgnoreCase("FORTH")) {
      options.BootMode = 1;
      EEPROM.put(configAddr, options);
      return 1;
    }
    else if (splitter->getItemAtIndex(0).equalsIgnoreCase("BOOT")) {
      options.BootMode = 2;
      EEPROM.put(configAddr, options);
      return 2;
    }
    else if (splitter->getItemAtIndex(0).equalsIgnoreCase("AUTOBOOT")) {
      options.BootMode = 3;
      EEPROM.put(configAddr, options);
      return 3;
    }
    else if (splitter->getItemAtIndex(0).equalsIgnoreCase("LOAD") || splitter->getItemAtIndex(0).equalsIgnoreCase("ILOAD")) {
      options.BootMode = 4;
      EEPROM.put(configAddr, options);
      return 4;
    }
    else if (splitter->getItemAtIndex(0).equalsIgnoreCase("SAVE")) {
      EEPROM.put(configAddr, options);
      Serial.println("Config Saved");
      continue;
    }
    else if (splitter->getItemAtIndex(0).equalsIgnoreCase("SET")) {
      if (splitter->getItemAtIndex(1).equalsIgnoreCase("CLOCK")) {
        if (splitter->getItemAtIndex(2).equalsIgnoreCase("HIGH")) {
          options.ClockMode = 0;
          Serial.println("Clock set to 8MHz");
          continue;
        }
        else if (splitter->getItemAtIndex(2).equalsIgnoreCase("LOW")) {
          options.ClockMode = 1;
          Serial.println("Clock set to 4MHz");
          continue;
        }
      }
      else if (splitter->getItemAtIndex(1).equalsIgnoreCase("AUTOBOOT")) {
                if (splitter->getItemAtIndex(2).equalsIgnoreCase("ON")) {
          options.AutoexecFlag = 1;
          Serial.println("Autoboot enabled");
          continue;
        }
        else if (splitter->getItemAtIndex(2).equalsIgnoreCase("OFF")) {
          options.AutoexecFlag = 0;
          Serial.println("Autoboot disabled");
          continue;
        }
      }
      else if (splitter->getItemAtIndex(1).equalsIgnoreCase("DATE")) {
        ChangeRTC();
        continue;
      }
      else if (splitter->getItemAtIndex(1).equalsIgnoreCase("BOOTDISK")) {
        if(splitter->getItemAtIndex(2).length() == 1 
        && isDigit(splitter->getItemAtIndex(2)[0]) 
        && splitter->getItemAtIndex(2).toInt() >=0 
        && splitter->getItemAtIndex(2).toInt() < maxDiskSet) {
          options.DiskSet = splitter->getItemAtIndex(2).toInt();
          OsBootInfo tmpInfo = GetDiskSetBootInfo(options.DiskSet);
          Serial.printf(F("BootDisk set to : Disk Set %d (%s)\n\r"), options.DiskSet, tmpInfo.OsName);
          continue;
        }
      }
    }
    Serial.println("Syntax Error");
    printHelp();
  }
}