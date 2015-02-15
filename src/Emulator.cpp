#include <QtDebug>
#include "Emulator.h"
#include "QSleeper.h"

#define NUM_REGISTERS      16
#define MAX_MEMORY         0x1000

#define SCREEN_MEMORY_ADDR 0x0f00
#define SCREEN_MEMORY_SIZE 0x0100

#define FONT_HEIGHT        5
#define FONT_ADDR          0x0100

const int MS_PER_TIMER_COUNT = 1000 / 60;

Emulator::Emulator()
{
   for (int i = 0; i < NUM_REGISTERS; i++)
   {
      theCpuRegisters.push_back(0);
   }

   for(int i = 0; i < MAX_MEMORY; i++)
   {
      theMemory.push_back(0);
   }

   loadFonts();

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
   for(int i = 0; i < theMemory.size(); i++)
   {
      theMemory[i] = 0;
   }

   for(int i = 0; i < theCpuRegisters.size(); i++)
   {
      theCpuRegisters[i] = 0;
   }

   theIndexRegister = 0;
   theKeysDown.clear();
   theCpuStack.clear();

   theScreen->clearScreen();

   unsigned int loadAddr = theAddress;
   for(int i = 0; i < theRomData.size(); i++)
   {
      theMemory[loadAddr++] = theRomData[i];
   }

   loadFonts();

   theSoundTimerExpiration = QDateTime::currentDateTime();
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
      qDebug() << "Illegal register value requested in " << __PRETTY_FUNCTION__;
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
   theAddress = theCpuStack.pop();
}

void Emulator::insJump(unsigned addr)
{
   theAddress = addr - 0x2;
}

void Emulator::insCall(unsigned addr)
{
   theCpuStack.push(theAddress);
   theAddress = addr - 0x2;
}

void Emulator::insSetIndexReg(unsigned addr)
{
   theIndexRegister = addr;
}

void Emulator::insJumpWithOffset(unsigned addr)
{
   theAddress = addr + theCpuRegisters[0] - 0x2;

   if (theAddress > 0x1000)
   {
      qDebug() << "jump with offset trying to jump out of memory";
      qDebug() << "  addr=" << addr << " offset=" << theCpuRegisters[0];
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
   if (theCpuRegisters[reg] > theCpuRegisters[otherReg])
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
   if (theCpuRegisters[reg] < theCpuRegisters[otherReg])
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
   if (theCpuRegisters[reg] & 0x8)
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
   theSoundTimerExpiration = QDateTime::currentDateTime();
   theSoundTimerExpiration.addMSecs(theCpuRegisters[reg] * MS_PER_TIMER_COUNT);
}

void Emulator::insAddRegToIndexReg(unsigned reg)
{
   theIndexRegister += theCpuRegisters[reg] % 0x10000;
}

void Emulator::insSetIndexToCharInReg(unsigned reg)
{
   theIndexRegister = FONT_ADDR + theCpuRegisters[reg] * FONT_HEIGHT;
}

void Emulator::insSetIndexMemoryToRegBcd(unsigned reg)
{
   unsigned char val = theCpuRegisters[reg];

   theMemory[theIndexRegister] = val / 100;
   val = val % 100;

   theMemory[theIndexRegister+1] = val / 10;
   val = val % 10;

   theMemory[theIndexRegister+2] = val;
}

void Emulator::insStoreRegsToIndexMemory(unsigned reg)
{
   for(unsigned int i = 0; i <= reg; i++)
   {
      theMemory[theIndexRegister + i] = theCpuRegisters[i];
   }
}

void Emulator::insLoadRegsFromIndexMemory(unsigned reg)
{
   for(unsigned int i = 0; i <= reg; i++)
   {
      theCpuRegisters[i] = theMemory[theIndexRegister + i];
   }
}

void Emulator::insDrawSprite(unsigned xReg, unsigned yReg, unsigned height)
{
   vector<unsigned char> spriteData;
   for(unsigned int i = 0; i < height; i++)
   {
      spriteData.push_back(theMemory[theIndexRegister+i]);
   }

   bool collision = theScreen->drawSprite(theCpuRegisters[xReg] % 64,
                                          theCpuRegisters[yReg] % 32,
                                          spriteData);

   if (collision)
      theCpuRegisters[0xf] = 1;
   else
      theCpuRegisters[0xf] = 0;
}

void Emulator::insBad(unsigned opCode)
{
   qDebug() << "Bad opcode" << QString::number(opCode, 16);
}

void Emulator::loadFonts()
{
   // Font codes found at:  http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#dispcoords
   unsigned int addr = FONT_ADDR;

   // 0
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0xf0;

   // 1
   theMemory[addr++] = 0x20;
   theMemory[addr++] = 0x60;
   theMemory[addr++] = 0x20;
   theMemory[addr++] = 0x20;
   theMemory[addr++] = 0x70;

   // 2
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x10;
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x80;
   theMemory[addr++] = 0xf0;

   // 3
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x10;
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x10;
   theMemory[addr++] = 0xf0;

   // 4
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x10;
   theMemory[addr++] = 0x10;

   // 5
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x80;
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x10;
   theMemory[addr++] = 0xf0;

   // 6
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x80;
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0xf0;

   // 7
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x10;
   theMemory[addr++] = 0x20;
   theMemory[addr++] = 0x40;
   theMemory[addr++] = 0x40;

   // 8
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0xf0;

   // 9
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x10;
   theMemory[addr++] = 0xf0;

   // a
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0x90;

   // b
   theMemory[addr++] = 0xe0;
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0xe0;
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0xe0;

   // c
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x80;
   theMemory[addr++] = 0x80;
   theMemory[addr++] = 0x80;
   theMemory[addr++] = 0xf0;

   // d
   theMemory[addr++] = 0xe0;
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0x90;
   theMemory[addr++] = 0xe0;

   // e
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x80;
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x80;
   theMemory[addr++] = 0xf0;

   // f
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x80;
   theMemory[addr++] = 0xf0;
   theMemory[addr++] = 0x80;
   theMemory[addr++] = 0x80;
}

void Emulator::run()
{
   theStopFlag = false;

   while(!theStopFlag)
   {
      executeInstruction();

      usleep(500);

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
