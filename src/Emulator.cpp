#include <QtDebug>
#include "Emulator.h"
#include "QSleeper.h"

#define NUM_REGISTERS      16
#define MAX_MEMORY         0x1000

#define SCREEN_MEMORY_ADDR 0x0f00
#define SCREEN_MEMORY_SIZE 0x0100

#define FONT_HEIGHT        5
#define FONT_ADDR          0x0100

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

void Emulator::loadRomData(unsigned char byte)
{
   qDebug() << "Load at" << theAddress << "of" << byte;

   theMemory[theAddress] = byte;
   theAddress += 1;

}

void Emulator::resetEmulator()
{
   theAddress = 0x0200;
}

void Emulator::executeInstruction()
{

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


void Emulator::insClearScreen()
{
   for(unsigned int addr = SCREEN_MEMORY_ADDR; addr < SCREEN_MEMORY_ADDR + SCREEN_MEMORY_SIZE; addr++)
   {
      theMemory[addr] = 0;
   }
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

}

void Emulator::insOrReg(unsigned reg, unsigned otherReg)
{

}

void Emulator::insAndReg(unsigned reg, unsigned otherReg)
{

}

void Emulator::insXorReg(unsigned reg, unsigned otherReg)
{

}

void Emulator::insAddRegs(unsigned reg, unsigned otherReg)
{

}

void Emulator::insSubRegs(unsigned reg, unsigned otherReg)
{

}

void Emulator::insSubRegsOtherOrder(unsigned reg, unsigned otherReg)
{

}

void Emulator::insRightShift(unsigned reg)
{

}

void Emulator::insLeftShift(unsigned reg)
{

}

void Emulator::insSkipNextIfKeyPressed(unsigned reg)
{

}

void Emulator::insSkipNextIfKeyNotPressed(unsigned reg)
{

}

void Emulator::insWaitForKeyPress(unsigned reg)
{
   // Not sure what this thing would do if more than one key pressed at a time, so we will just return the first item
   // in the set unless I come up with a better idea

   bool waitingForKey = true;
   unsigned char keyPressed;
   while(waitingForKey)
   {
      theKeysLock.lock();

      waitingForKey = !theKeysDown.isEmpty();

      if (!waitingForKey)
      {
         // Get a pressed key
         keyPressed = theKeysDown.toList().first();
      }

      theKeysLock.unlock();

      QSleeper::sleepMilliSecs(1);
   }

   theCpuRegisters[reg] = keyPressed;
}

void Emulator::insSetRegToDelayTimer(unsigned reg)
{

}

void Emulator::insSetDelayTimer(unsigned reg)
{

}

void Emulator::insSetSoundTimer(unsigned reg)
{

}

void Emulator::insAddRegToIndexReg(unsigned reg)
{

}

void Emulator::insSetIndexToCharInReg(unsigned reg)
{
   theIndexRegister = FONT_ADDR + reg * FONT_HEIGHT;
}

void Emulator::insSetIndexMemoryToRegBcd(unsigned reg)
{

}

void Emulator::insStoreRegsToIndexMemory(unsigned reg)
{

}

void Emulator::insLoadRegsFromIndexMemory(unsigned reg)
{

}

void Emulator::insDrawSprite(unsigned xReg, unsigned yReg, unsigned height)
{

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
