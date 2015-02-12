#include "Emulator.h"

#define NUM_REGISTERS      16
#define MAX_MEMORY         0x1000

#define SCREEN_MEMORY_ADDR 0x0f00
#define SCREEN_MEMORY_SIZE 0x0100

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

}

void Emulator::loadRomData(unsigned char byte)
{

}

void Emulator::resetEmulator()
{

}

void Emulator::executeInstruction()
{

}

void Emulator::keyDown(unsigned char key)
{
   theKeysDown.insert(key);
}

void Emulator::keyUp(unsigned char key)
{
   theKeysDown.erase(key);
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

}

void Emulator::insJump(unsigned addr)
{

}

void Emulator::insCall(unsigned addr)
{

}

void Emulator::insSetIndexReg(unsigned addr)
{

}

void Emulator::insJumpWithOffset(unsigned addr)
{

}

void Emulator::insSkipNextIfRegEqConst(unsigned reg, unsigned val)
{

}

void Emulator::insSkipNextIfRegNotEqConst(unsigned reg, unsigned val)
{

}

void Emulator::insSetReg(unsigned reg, unsigned val)
{
   theCpuRegisters[reg] = val;
}

void Emulator::insAddReg(unsigned reg, unsigned val)
{

}

void Emulator::insRandomNum(unsigned reg, unsigned mask)
{

}

void Emulator::insSkipNextIfRegEq(unsigned reg1, unsigned reg2)
{

}

void Emulator::insSkipNextIfRegNotEq(unsigned reg1, unsigned reg2)
{

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

}
