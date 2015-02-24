#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <iomanip>
#
#include <stdio.h>
#include "Disassembler.h"

using namespace std;

Disassembler::Disassembler():
   thePrintAddressOption(true)
{
}

void Disassembler::printAddresses(bool printAddr)
{
   thePrintAddressOption = printAddr;
}

void Disassembler::insClearScreen()
{
  theDisassembly[theAddress] = "cls";
}

void Disassembler::insReturnFromSub()
{
   theDisassembly[theAddress] = "ret";
}

void Disassembler::insJump(unsigned addr)
{
   ostringstream ss;
   ss << "jp loc_" << setfill('0') << setw(4) << hex << addr;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insCall(unsigned addr)
{
   ostringstream ss;
   ss << "call loc_" << setfill('0') << setw(4) << hex << addr;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSetIndexReg(unsigned addr)
{
   ostringstream ss;
   ss << "ld I, #" << setfill('0') << setw(4) << hex << addr;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insJumpWithOffset(unsigned addr)
{
   ostringstream ss;
   ss << "jp v0, #" << setfill('0') << setw(4) << hex << addr;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSkipNextIfRegEqConst(unsigned reg, unsigned val)
{
   ostringstream ss;
   ss << "se v" << hex << reg << ", #" << setfill('0') << setw(2) << hex << val;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSkipNextIfRegNotEqConst(unsigned reg, unsigned val)
{
   ostringstream ss;
   ss << "sne v" << hex << reg << ", #" << setfill('0') << setw(2) << hex << val;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSetReg(unsigned reg, unsigned val)
{
   ostringstream ss;
   ss << "ld v" << hex << reg << ", #" << setfill('0') << setw(2) << hex << val;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insAddReg(unsigned reg, unsigned val)
{
   ostringstream ss;
   ss << "add v" << hex << reg << ", #" << setfill('0') << setw(2) << hex << val;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insRandomNum(unsigned reg, unsigned mask)
{
   ostringstream ss;
   ss << "rnd v" << hex << reg << ", #" << setfill('0') << setw(2) << hex << mask;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSkipNextIfRegEq(unsigned reg1, unsigned reg2)
{
   ostringstream ss;
   ss << "se v" << hex << reg1 << ", v" << reg2;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSkipNextIfRegNotEq(unsigned reg1, unsigned reg2)
{
   ostringstream ss;
   ss << "sne v" << hex << reg1 << ", v" << reg2;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSetRegToRegVal(unsigned regToSet, unsigned regVal)
{
   ostringstream ss;
   ss << "ld v" << hex << regToSet << ", v" << regVal;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insOrReg(unsigned reg, unsigned otherReg)
{
   ostringstream ss;
   ss << "or v" << hex << reg << ", v" << otherReg;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insAndReg(unsigned reg, unsigned otherReg)
{
   ostringstream ss;
   ss << "and v" << hex << reg << ", v" << otherReg;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insXorReg(unsigned reg, unsigned otherReg)
{
   ostringstream ss;
   ss << "xor v" << hex << reg << ", v" << otherReg;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insAddRegs(unsigned reg, unsigned otherReg)
{
   ostringstream ss;
   ss << "add v" << hex << reg << ", v" << otherReg;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSubRegs(unsigned reg, unsigned otherReg)
{
   ostringstream ss;
   ss << "sub v" << hex << reg << ", v" << otherReg;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSubRegsOtherOrder(unsigned reg, unsigned otherReg)
{
   ostringstream ss;
   ss << "subn v" << hex << reg << ", v" << otherReg;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insRightShift(unsigned reg)
{
   ostringstream ss;
   ss << "shr v" << hex << reg;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insLeftShift(unsigned reg)
{
   ostringstream ss;
   ss << "shl v" << hex << reg;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSkipNextIfKeyPressed(unsigned reg)
{
   ostringstream ss;
   ss << "skp v" << hex << reg;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSkipNextIfKeyNotPressed(unsigned reg)
{
   ostringstream ss;
   ss << "sknp v" << hex << reg;

   theDisassembly[theAddress] = ss.str();
}

void Disassembler::insWaitForKeyPress(unsigned reg)
{
   ostringstream ss;
   ss << "ld v" << hex << reg << ", k";

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSetRegToDelayTimer(unsigned reg)
{
   ostringstream ss;
   ss << "ld v" << hex << reg << ", dt";

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSetDelayTimer(unsigned reg)
{
   ostringstream ss;
   ss << "ld dt, v" << hex << reg;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSetSoundTimer(unsigned reg)
{
   ostringstream ss;
   ss << "ld st, v" << hex << reg;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insAddRegToIndexReg(unsigned reg)
{
   ostringstream ss;
   ss << "add I, v" << hex << reg;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSetIndexToCharInReg(unsigned reg)
{
   ostringstream ss;
   ss << "ld f, v" << hex << reg;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insSetIndexMemoryToRegBcd(unsigned reg)
{
   ostringstream ss;
   ss << "ld b, v" << hex << reg;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insStoreRegsToIndexMemory(unsigned reg)
{
   ostringstream ss;
   ss << "ld [I], v" << hex << reg;

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insLoadRegsFromIndexMemory(unsigned reg)
{
   ostringstream ss;
   ss << "ld v" << hex << reg << ", [I]";

   theDisassembly[theAddress] = ss.str();
}


void Disassembler::insDrawSprite(unsigned xReg, unsigned yReg, unsigned height)
{
   ostringstream ss;
   ss << "drw v" << hex << xReg << ", v" << yReg << ", #" << height;

   theDisassembly[theAddress] = ss.str();
}

void Disassembler::printDisassembly()
{
   unsigned int curAddress = 0x200;

   while (curAddress < theAddress)
   {
      if (thePrintAddressOption)
      {
         printf("0x%04x\t%s\n", curAddress, theDisassembly[curAddress].c_str());
      }
      else
      {
         printf("%s\n", theDisassembly[curAddress].c_str());
      }
      curAddress += 2;
   }
}

void Disassembler::insBad(unsigned opCode)
{
   ostringstream ss;
   ss << "BAD OPCODE 0x" << setfill('0') << setw(4) << hex << opCode;

   theDisassembly[theAddress] = ss.str();
}

// Super Chip-8 Instructions
void Disassembler::insScrollDown(unsigned char numLines)
{
   ostringstream ss;
   ss << "scd " << numLines;

   theDisassembly[theAddress] = ss.str();
}

void Disassembler::insScrollLeft()
{
   ostringstream ss;
   ss << "scl";

   theDisassembly[theAddress] = ss.str();
}

void Disassembler::insScrollRight()
{
   ostringstream ss;
   ss << "scr";

   theDisassembly[theAddress] = ss.str();
}

void Disassembler::insQuitEmulator()
{
   ostringstream ss;
   ss << "exit";

   theDisassembly[theAddress] = ss.str();
}

void Disassembler::insEnterLowResMode()
{
   ostringstream ss;
   ss << "low";

   theDisassembly[theAddress] = ss.str();
}

void Disassembler::insEnterHighResMode()
{
   ostringstream ss;
   ss << "high";

   theDisassembly[theAddress] = ss.str();
}

void Disassembler::insSaveHp48Flags(unsigned char reg)
{
   ostringstream ss;
   ss << "save_hp_flags" << hex << reg;

   theDisassembly[theAddress] = ss.str();
}

void Disassembler::insLoadHp48Flags(unsigned char reg)
{
   ostringstream ss;
   ss << "load_hp_flags" << hex << reg;

   theDisassembly[theAddress] = ss.str();
}

void Disassembler::insSetIndexToHiResCharInReg(unsigned char reg)
{
   ostringstream ss;
   ss << "ld hf, v" << hex << (unsigned int) reg;

   theDisassembly[theAddress] = ss.str();
}

