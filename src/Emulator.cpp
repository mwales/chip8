#include <QtDebug>
#include <QThreadPool>
#include "Emulator.h"
#include "QSleeper.h"
#include "audio_player.h"

#define NUM_REGISTERS      16
#define MAX_MEMORY         0x1000

#define SCREEN_MEMORY_ADDR 0x0f00
#define SCREEN_MEMORY_SIZE 0x0100

#define FONT_HEIGHT_STD        5
#define FONT_HEIGHT_HIRES      10


const int MS_PER_TIMER_COUNT = 1000 / 60;

Emulator::Emulator():
   theHighResMode(false),
   theInstructionPeriodMicroSecs(500),
   theSoundEnabled(true)

{
   for (int i = 0; i < NUM_REGISTERS; i++)
   {
      theCpuRegisters.push_back(0);
   }

   loadFonts();
   for(int i = theMemory.size(); i < MAX_MEMORY; i++)
   {
      theMemory.push_back(0);
   }


}

void Emulator::clearRomData()
{
   theRomData.clear();
}

void Emulator::loadRomData(unsigned char byte)
{
   theRomData.append(byte);
}

void Emulator::resetEmulator()
{
   // Reset the IP
   theAddress = 0x0200;

   // Clear everything
   theMemory.clear();
   loadFonts();
   for(int i = theMemory.size(); i < MAX_MEMORY; i++)
   {
      theMemory.append(0);
   }

   for(int i = 0; i < theCpuRegisters.size(); i++)
   {
      theCpuRegisters[i] = 0;
   }

   theIndexRegister = 0;
   theKeysDown.clear();
   theCpuStack.clear();

   theScreen->clearScreen();
   theScreen->hiResModeEnable(false);
   theHighResMode = false;

   unsigned int loadAddr = theAddress;
   for(int i = 0; i < theRomData.size(); i++)
   {
      theMemory[loadAddr++] = theRomData[i];
   }

   //loadFonts();

   theDelayTimerExpiration = QDateTime::currentDateTime();
}

void Emulator::executeInstruction()
{
   if (theAddress > (0x1000 - 2))
   {
      qDebug() << "Instruction address " << QString::number(theAddress, 16) << " out of bounds";
      return;
   }

   unsigned char opCode[2];
   opCode[0] = theMemory[theAddress];
   opCode[1] = theMemory[theAddress+1];
   decodeInstruction(opCode);
}

void Emulator::keyDown(unsigned char key)
{
   theKeysLock.lock();
   theKeysDown.insert(key);
   theKeysLock.unlock();
}

void Emulator::keyUp(unsigned char key)
{
   theKeysLock.lock();
   theKeysDown.remove(key);
   theKeysLock.unlock();
}

void Emulator::setEmulationScreen(EmulationScreen* screen)
{
   theScreen = screen;
}

unsigned char Emulator::getRegister(unsigned char reg)
{
   if (reg >= 16)
   {
      qFatal("Illegal register value requested in Emulator::getRegister()");
      return 0;
   }

   return theCpuRegisters[reg];
}

unsigned int Emulator::getIP()
{
   return theAddress;
}

unsigned int Emulator::getIndexRegister()
{
   return theIndexRegister;
}

QStack<unsigned int> Emulator::getStack()
{
   return theCpuStack;
}

unsigned char Emulator::getDelayTimer()
{
   QDateTime curTime = QDateTime::currentDateTime();

   if (curTime > theDelayTimerExpiration)
   {
      return 0;
   }
   else
   {
      int numMilliSecs = curTime.msecsTo(theDelayTimerExpiration);
      return numMilliSecs / MS_PER_TIMER_COUNT;
   }
}

void Emulator::insClearScreen()
{
   theScreen->clearScreen();
}

void Emulator::insReturnFromSub()
{
   if (theCpuStack.isEmpty())
   {
      qDebug() << "Return statement with no return addresses on the stack";
      theStopFlag = true;
      return;
   }

   theAddress = theCpuStack.pop();
}

void Emulator::insJump(unsigned addr)
{
   if (addr > 0x1000)
   {
      qDebug() << "Jump to address" << QString::number(addr, 16) << "is out of bounds";
      theStopFlag = true;
      return;
   }

   theAddress = addr - 0x2;
}

void Emulator::insCall(unsigned addr)
{
   if (addr > 0x1000)
   {
      qDebug() << "Call to address" << QString::number(addr, 16) << "is out of bounds";
      theStopFlag = true;
      return;
   }

   theCpuStack.push(theAddress);
   theAddress = addr - 0x2;
}

void Emulator::insSetIndexReg(unsigned addr)
{
   if (addr > 0x1000)
   {
      qDebug() << "Set Index Register address to" << QString::number(addr, 16) << "is out of bounds";
      theStopFlag = true;
      return;
   }

   theIndexRegister = addr;
}

void Emulator::insJumpWithOffset(unsigned addr)
{
   theAddress = addr + theCpuRegisters[0] - 0x2;

   if (theAddress > 0x1000)
   {
      qDebug() << "jump with offset trying to jump out of memory";
      qDebug() << "  addr=" << addr << " offset=" << theCpuRegisters[0];
      theStopFlag = true;
      return;
   }
}

void Emulator::insSkipNextIfRegEqConst(unsigned reg, unsigned val)
{
   if (theCpuRegisters[reg] == val)
      theAddress += 2;
}

void Emulator::insSkipNextIfRegNotEqConst(unsigned reg, unsigned val)
{
   if (theCpuRegisters[reg] != val)
      theAddress += 2;
}

void Emulator::insSetReg(unsigned reg, unsigned val)
{
   theCpuRegisters[reg] = val;
}

void Emulator::insAddReg(unsigned reg, unsigned val)
{
   theCpuRegisters[reg] += val;
}

void Emulator::insRandomNum(unsigned reg, unsigned mask)
{
   unsigned int num = qrand() % 256;
   theCpuRegisters[reg] = num & mask;
}

void Emulator::insSkipNextIfRegEq(unsigned reg1, unsigned reg2)
{
   if (theCpuRegisters[reg1] == theCpuRegisters[reg2])
      theAddress += 2;
}

void Emulator::insSkipNextIfRegNotEq(unsigned reg1, unsigned reg2)
{
   if (theCpuRegisters[reg1] != theCpuRegisters[reg2])
      theAddress += 2;
}

void Emulator::insSetRegToRegVal(unsigned regToSet, unsigned regVal)
{
   theCpuRegisters[regToSet] = theCpuRegisters[regVal];
}

void Emulator::insOrReg(unsigned reg, unsigned otherReg)
{
   theCpuRegisters[reg] = theCpuRegisters[reg] | theCpuRegisters[otherReg];
}

void Emulator::insAndReg(unsigned reg, unsigned otherReg)
{
   theCpuRegisters[reg] = theCpuRegisters[reg] & theCpuRegisters[otherReg];
}

void Emulator::insXorReg(unsigned reg, unsigned otherReg)
{
   theCpuRegisters[reg] = theCpuRegisters[reg] ^ theCpuRegisters[otherReg];
}

void Emulator::insAddRegs(unsigned reg, unsigned otherReg)
{
   unsigned int res = theCpuRegisters[reg] + theCpuRegisters[otherReg];

   if (res > 255)
   {
      theCpuRegisters[0xf] = 1;
      theCpuRegisters[reg] = res % 256;
   }
   else
   {
      theCpuRegisters[0xf] = 0;
      theCpuRegisters[reg] = res;
   }

}

void Emulator::insSubRegs(unsigned reg, unsigned otherReg)
{
   // I passed one of the Chip-8 ROM tests by adding equality to the following
   // test, but I'm not sure why this is
   if (theCpuRegisters[reg] >= theCpuRegisters[otherReg])
   {
      theCpuRegisters[0xf] = 1;
   }
   else
   {
      theCpuRegisters[0xf] = 0;
   }

   theCpuRegisters[reg] -= theCpuRegisters[otherReg];
}

void Emulator::insSubRegsOtherOrder(unsigned reg, unsigned otherReg)
{
   // I passed one of the Chip-8 ROM tests by adding equality to the following
   // test, but I'm not sure why this is
   if (theCpuRegisters[reg] <= theCpuRegisters[otherReg])
   {
      theCpuRegisters[0xf] = 1;
   }
   else
   {
      theCpuRegisters[0xf] = 0;
   }

   theCpuRegisters[reg] = theCpuRegisters[otherReg] - theCpuRegisters[reg];
}

void Emulator::insRightShift(unsigned reg)
{
   if (theCpuRegisters[reg] & 0x1)
   {
      theCpuRegisters[0xf] = 1;
   }
   else
   {
      theCpuRegisters[0xf] = 0;
   }

   theCpuRegisters[reg] = theCpuRegisters[reg] >> 1;
}

void Emulator::insLeftShift(unsigned reg)
{
   if (theCpuRegisters[reg] & 0x80)
   {
      theCpuRegisters[0xf] = 1;
   }
   else
   {
      theCpuRegisters[0xf] = 0;
   }

   theCpuRegisters[reg] = theCpuRegisters[reg] << 1;
}

void Emulator::insSkipNextIfKeyPressed(unsigned reg)
{
   if (theKeysDown.contains(theCpuRegisters[reg]))
   {
      theAddress += 2;
   }
}

void Emulator::insSkipNextIfKeyNotPressed(unsigned reg)
{
   if (!theKeysDown.contains(theCpuRegisters[reg]))
   {
      theAddress += 2;
   }
}

void Emulator::insWaitForKeyPress(unsigned reg)
{
   // Not sure what this thing would do if more than one key pressed at a time, so we will just return the first item
   // in the set unless I come up with a better idea

   theKeysLock.lock();

   bool waitingForKey = theKeysDown.isEmpty();

   if (!waitingForKey)
   {
      // Get a pressed key
      theCpuRegisters[reg] = theKeysDown.toList().first();
   }

   theKeysLock.unlock();

   if (waitingForKey)
   {
      // Rather than blocking, just move the IP back 2 and repeat this instruction over and over
      theAddress -= 2;
   }

}

void Emulator::insSetRegToDelayTimer(unsigned reg)
{
   theCpuRegisters[reg] = getDelayTimer();
}

void Emulator::insSetDelayTimer(unsigned reg)
{
   theDelayTimerExpiration = QDateTime::currentDateTime();
   theDelayTimerExpiration = theDelayTimerExpiration.addMSecs(theCpuRegisters[reg] * MS_PER_TIMER_COUNT);
}

void Emulator::insSetSoundTimer(unsigned reg)
{
   // I should be extending the sound duration if the sound timer set while it is already running, but I'm ignoring
   // that feature for now.  Sound is annoying enough as is.

   if (theSoundEnabled)
   {
      AnnoyingSound* player = new AnnoyingSound();
      player->setDuration(theCpuRegisters[reg] / 60.0);

      QThreadPool::globalInstance()->start(player);
   }
   else
   {
      qDebug() << "Sound Muted";
   }
}

void Emulator::insAddRegToIndexReg(unsigned reg)
{
   theIndexRegister += theCpuRegisters[reg] % 0x10000;
}

void Emulator::insSetIndexToCharInReg(unsigned reg)
{
   // Low-res fonts start at address 0
   theIndexRegister = theCpuRegisters[reg] * FONT_HEIGHT_STD;
}

void Emulator::insSetIndexMemoryToRegBcd(unsigned reg)
{
   if (theIndexRegister + 2 >= MAX_MEMORY)
   {
      qDebug() << "Index register out of valid memory range for the BCD instruction.  Index = "
               << QString::number(theIndexRegister, 16) << "and address is" << QString::number(theAddress, 16);
      theStopFlag = true;
      return;
   }

   unsigned char val = theCpuRegisters[reg];

   theMemory[theIndexRegister] = val / 100;
   val = val % 100;

   theMemory[theIndexRegister+1] = val / 10;
   val = val % 10;

   theMemory[theIndexRegister+2] = val;
}

void Emulator::insStoreRegsToIndexMemory(unsigned reg)
{
   if (theIndexRegister + reg >= MAX_MEMORY)
   {
      qDebug() << "Index register out of valid memory range for the store regs instruction.  Index = "
               << QString::number(theIndexRegister, 16) << "and address is" << QString::number(theAddress, 16);
      theStopFlag = true;
      return;
   }

   for(unsigned int i = 0; i <= reg; i++)
   {
      theMemory[theIndexRegister + i] = theCpuRegisters[i];
   }
}

void Emulator::insLoadRegsFromIndexMemory(unsigned reg)
{
   if (theIndexRegister + reg >= MAX_MEMORY)
   {
      qDebug() << "Index register out of valid memory range for the load regs instruction.  Index = "
               << QString::number(theIndexRegister, 16) << "and address is" << QString::number(theAddress, 16);
      theStopFlag = true;
      return;
   }

   for(unsigned int i = 0; i <= reg; i++)
   {
      theCpuRegisters[i] = theMemory[theIndexRegister + i];
   }
}

void Emulator::insDrawSprite(unsigned xReg, unsigned yReg, unsigned height)
{
   if (theIndexRegister >= MAX_MEMORY)
   {
      qDebug() << "Index register out of valid memory range for the draw sprite instruction.  Index = "
               << QString::number(theIndexRegister, 16) << "and address is" << QString::number(theAddress, 16);
      theStopFlag = true;
      return;
   }

   QVector<unsigned char> spriteData;
   bool collision;

   if (height > 0)
   {
      for(unsigned int i = 0; i < height; i++)
      {
         spriteData.push_back(theMemory[theIndexRegister+i]);
      }

      collision = theScreen->drawSprite(theCpuRegisters[xReg] % (theHighResMode ? 128 : 64),
                                        theCpuRegisters[yReg] % (theHighResMode ? 64 : 32),
                                        spriteData);

   }
   else
   {
      // height = 0, special super chip-8 form of instruction
      if (theHighResMode)
      {
         // Draw super sprite!
         for(unsigned int i = 0; i < 32; i++)
         {
            spriteData.push_back(theMemory[theIndexRegister+i]);
         }

         collision = theScreen->drawSuperSprite(theCpuRegisters[xReg] % 128,
                                                theCpuRegisters[yReg] % 64,
                                                spriteData);
      }
      else
      {
         // Draw 16 row sprite!
         for(unsigned int i = 0; i < 16; i++)
         {
            spriteData.push_back(theMemory[theIndexRegister+i]);
         }

         collision = theScreen->drawSprite(theCpuRegisters[xReg] % 64,
                                           theCpuRegisters[yReg] % 32,
                                           spriteData);
      }
   }





   if (collision)
      theCpuRegisters[0xf] = 1;
   else
      theCpuRegisters[0xf] = 0;
}

void Emulator::insScrollDown(unsigned char numLines)
{
   theScreen->shiftDown(numLines);
}

void Emulator::insScrollLeft()
{
   theScreen->shiftLeft();
}

void Emulator::insScrollRight()
{
   theScreen->shiftRight();
}

void Emulator::insQuitEmulator()
{
   qDebug() << "Quit Emulator Instruction";

   theStopFlag = true;

   // To not allow someone to step over the flag, reset the address
   theAddress -= 2;
}

void Emulator::insEnterLowResMode()
{
   theScreen->hiResModeEnable(false);
   theHighResMode = false;
}

void Emulator::insEnterHighResMode()
{
   theScreen->hiResModeEnable(true);
   theHighResMode = true;
}

void Emulator::insSaveHp48Flags(unsigned char)
{
   qDebug() << __FUNCTION__;
}

void Emulator::insLoadHp48Flags(unsigned char)
{
   qDebug() << __FUNCTION__;
}

void Emulator::insSetIndexToHiResCharInReg(unsigned char reg)
{
   // High-res fonts start at address the
   theIndexRegister = theHiResFontsAddr + theCpuRegisters[reg] * FONT_HEIGHT_HIRES;
}

void Emulator::insBad(unsigned opCode)
{
   qDebug() << "Bad opcode" << QString::number(opCode, 16);
   theStopFlag = true;
}

void Emulator::loadFonts()
{
   // Low res Font codes found at:  http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#dispcoords

   theMemory << 0xf0 << 0x90 << 0x90 << 0x90 << 0xf0; // 0
   theMemory << 0x20 << 0x60 << 0x20 << 0x20 << 0x70;
   theMemory << 0xf0 << 0x10 << 0xf0 << 0x80 << 0xf0;
   theMemory << 0xf0 << 0x10 << 0xf0 << 0x10 << 0xf0;
   theMemory << 0x90 << 0x90 << 0xf0 << 0x10 << 0x10;
   theMemory << 0xf0 << 0x80 << 0xf0 << 0x10 << 0xf0; // 5
   theMemory << 0xf0 << 0x80 << 0xf0 << 0x90 << 0xf0;
   theMemory << 0xf0 << 0x10 << 0x20 << 0x40 << 0x40;
   theMemory << 0xf0 << 0x90 << 0xf0 << 0x90 << 0xf0;
   theMemory << 0xf0 << 0x90 << 0xf0 << 0x10 << 0xf0;
   theMemory << 0xf0 << 0x90 << 0xf0 << 0x90 << 0x90; // A
   theMemory << 0xe0 << 0x90 << 0xe0 << 0x90 << 0xe0;
   theMemory << 0xf0 << 0x80 << 0x80 << 0x80 << 0xf0;
   theMemory << 0xe0 << 0x90 << 0x90 << 0x90 << 0xe0;
   theMemory << 0xf0 << 0x80 << 0xf0 << 0x80 << 0xf0;
   theMemory << 0xf0 << 0x80 << 0xf0 << 0x80 << 0x80; // F

   theHiResFontsAddr = theMemory.size();

   theMemory << 0x3c << 0x66 << 0xc3 << 0x81 << 0x81
             << 0x81 << 0x81 << 0xc3 << 0x66 << 0x3c; // 0
   theMemory << 0x10 << 0x30 << 0x10 << 0x10 << 0x10
             << 0x10 << 0x10 << 0x10 << 0x10 << 0x7e;
   theMemory << 0x3c << 0x66 << 0x81 << 0x01 << 0x03
             << 0x06 << 0x38 << 0xc0 << 0x80 << 0xff;
   theMemory << 0x7e << 0x83 << 0x81 << 0x01 << 0x06
             << 0x06 << 0x01 << 0x81 << 0x83 << 0x7e;
   theMemory << 0x04 << 0x0c << 0x14 << 0x24 << 0x44
             << 0xff << 0x04 << 0x04 << 0x04 << 0x04;
   theMemory << 0xff << 0x80 << 0x80 << 0x80 << 0xfe
             << 0xc2 << 0x01 << 0x01 << 0x83 << 0x7e; // 5
   theMemory << 0x3e << 0x41 << 0x81 << 0x80 << 0xbc
             << 0xc2 << 0x81 << 0x81 << 0x81 << 0x7e;
   theMemory << 0xff << 0x01 << 0x03 << 0x06 << 0x08
             << 0x10 << 0x20 << 0x60 << 0x40 << 0x80;
   theMemory << 0x18 << 0x24 << 0x42 << 0x42 << 0x3c
             << 0x42 << 0x81 << 0x81 << 0x81 << 0x7e;
   theMemory << 0x7e << 0x81 << 0x81 << 0x81 << 0x43
             << 0x3d << 0x01 << 0x81 << 0x86 << 0x7c;
   theMemory << 0x18 << 0x66 << 0x42 << 0x81 << 0x81
             << 0xff << 0x81 << 0x81 << 0x81 << 0x81; // A
   theMemory << 0xfc << 0x82 << 0x82 << 0x82 << 0xfc
             << 0x82 << 0x81 << 0x81 << 0x81 << 0xfe;
   theMemory << 0x3c << 0x66 << 0xc3 << 0x81 << 0x80
             << 0x80 << 0x81 << 0xc3 << 0x66 << 0x3c;
   theMemory << 0xf8 << 0x84 << 0x82 << 0x81 << 0x81
             << 0x81 << 0x81 << 0x82 << 0x84 << 0xf8;
   theMemory << 0xff << 0x80 << 0x80 << 0x80 << 0x80
             << 0xfc << 0x80 << 0x80 << 0x80 << 0xff;
   theMemory << 0xff << 0x80 << 0x80 << 0x80 << 0x80
             << 0xfc << 0x80 << 0x80 << 0x80 << 0x80; // F

}

void Emulator::run()
{
   theStopFlag = false;

   while(!theStopFlag)
   {
      executeInstruction();

      usleep(theInstructionPeriodMicroSecs);

      if (theBreakpoints.contains(theAddress))
      {
         qDebug() << "Breakpoint at" << QString::number(theAddress, 16) << "hit";
         theStopFlag = true;
      }
   }
}

void Emulator::setBreakpoint(unsigned int addr)
{
   qDebug() << "Breakpoint added at" << QString::number(addr);
   theBreakpoints.insert(addr);
}

void Emulator::clearBreakpoints()
{
   theBreakpoints.clear();
}

void Emulator::stopEmulator()
{
   if (isRunning())
   {
      theStopFlag = true;
      wait();
   }
}

void Emulator::setInstPerSecond(int ips)
{
   theInstructionPeriodMicroSecs = 1000000 / ips;
}

void Emulator::enableSound(bool enable)
{
   theSoundEnabled = enable;
}
