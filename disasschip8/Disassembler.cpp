#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <iomanip>
#
#include <stdio.h>
#include "Disassembler.h"

using namespace std;

Disassembler::Disassembler()
{
}

void Disassembler::insClearScreen()
{
   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = "clear_screen";

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}

void Disassembler::insReturnFromSub()
{
   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = "ret";

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}

void Disassembler::insJump(unsigned addr)
{
   ostringstream ss;
   ss << "jmp 0x" << setfill('0') << setw(4) << hex << addr;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insCall(unsigned addr)
{
   ostringstream ss;
   ss << "call 0x" << setfill('0') << setw(4) << hex << addr;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSetIndexReg(unsigned addr)
{
   ostringstream ss;
   ss << "mov I, 0x" << setfill('0') << setw(4) << hex << addr;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insJumpWithOffset(unsigned addr)
{
   ostringstream ss;
   ss << "jmp 0x" << setfill('0') << setw(4) << hex << addr << " + v0";

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSkipNextIfRegEqConst(unsigned reg, unsigned val)
{
   ostringstream ss;
   ss << "skipnext_eq v" << hex << reg << ", 0x" << setfill('0') << setw(2) << hex << val;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSkipNextIfRegNotEqConst(unsigned reg, unsigned val)
{
   ostringstream ss;
   ss << "skipnext_ne v" << hex << reg << ", 0x" << setfill('0') << setw(2) << hex << val;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSetReg(unsigned reg, unsigned val)
{
   ostringstream ss;
   ss << "mov v" << hex << reg << ", 0x" << setfill('0') << setw(2) << hex << val;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insAddReg(unsigned reg, unsigned val)
{
   ostringstream ss;
   ss << "add v" << hex << reg << ", 0x" << setfill('0') << setw(2) << hex << val;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insRandomNum(unsigned reg, unsigned mask)
{
   ostringstream ss;
   ss << "rand v" << hex << reg << ", 0x" << setfill('0') << setw(2) << hex << mask;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSkipNextIfRegEq(unsigned reg1, unsigned reg2)
{
   ostringstream ss;
   ss << "skipnext_eq v" << hex << reg1 << ", v" << reg2;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSkipNextIfRegNotEq(unsigned reg1, unsigned reg2)
{
   ostringstream ss;
   ss << "skipnext_ne v" << hex << reg1 << ", v" << reg2;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSetRegToRegVal(unsigned regToSet, unsigned regVal)
{
   ostringstream ss;
   ss << "mov v" << hex << regToSet << ", v" << regVal;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insOrReg(unsigned reg, unsigned otherReg)
{
   ostringstream ss;
   ss << "or v" << hex << reg << ", v" << otherReg;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insAndReg(unsigned reg, unsigned otherReg)
{
   ostringstream ss;
   ss << "and v" << hex << reg << ", v" << otherReg;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insXorReg(unsigned reg, unsigned otherReg)
{
   ostringstream ss;
   ss << "xor v" << hex << reg << ", v" << otherReg;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insAddRegs(unsigned reg, unsigned otherReg)
{
   ostringstream ss;
   ss << "add v" << hex << reg << ", v" << otherReg;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSubRegs(unsigned reg, unsigned otherReg)
{
   ostringstream ss;
   ss << "sub v" << hex << reg << ", v" << otherReg;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSubRegsOtherOrder(unsigned reg, unsigned otherReg)
{
   ostringstream ss;
   ss << "subrev v" << hex << reg << ", v" << otherReg;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insRightShift(unsigned reg)
{
   ostringstream ss;
   ss << "right_shift v" << hex << reg;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insLeftShift(unsigned reg)
{
   ostringstream ss;
   ss << "left_shift v" << hex << reg;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSkipNextIfKeyPressed(unsigned reg)
{
   ostringstream ss;
   ss << "skipnext_on_keypress v" << hex << reg;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSkipNextIfKeyNotPressed(unsigned reg)
{
   ostringstream ss;
   ss << "skipnext_on_not_keypress v" << hex << reg;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}

void Disassembler::insWaitForKeyPress(unsigned reg)
{
   ostringstream ss;
   ss << "store_keypress v" << hex << reg;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSetRegToDelayTimer(unsigned reg)
{
   ostringstream ss;
   ss << "mov v" << hex << reg << ", delay_timer";

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSetDelayTimer(unsigned reg)
{
   ostringstream ss;
   ss << "mov delay_timer, v" << hex << reg;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSetSoundTimer(unsigned reg)
{
   ostringstream ss;
   ss << "mov sound_timer, v" << hex << reg;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insAddRegToIndexReg(unsigned reg)
{
   ostringstream ss;
   ss << "add I, v" << hex << reg;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSetIndexToCharInReg(unsigned reg)
{
   ostringstream ss;
   ss << "mov I, (font_base + v" << hex << reg << ")";

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insSetIndexMemoryToRegBcd(unsigned reg)
{
   ostringstream ss;
   ss << "mov [I:I+2], bcd(" << hex << reg << ")";

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insStoreRegsToIndexMemory(unsigned reg)
{
   ostringstream ss;
   ss << "mov [I:I+" << reg-1 << "], v0:v" << hex << reg;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insLoadRegsFromIndexMemory(unsigned reg)
{
   ostringstream ss;
   ss << "mov v0:v" << reg << ", [I:I+" << reg-1 << "]";

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}


void Disassembler::insDrawSprite(unsigned xReg, unsigned yReg, unsigned height)
{
   ostringstream ss;
   ss << "draw_spirte xreg=v" << xReg << ", yreg=v" << yReg << ", h=" << height;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}

void Disassembler::printDisassembly()
{
   //printf("%s\n", theDisassembly.c_str());

   vector< pair<unsigned int, string> >::iterator curIns = theDisassembly.begin();
   while(curIns != theDisassembly.end())
   {
      printf("0x%04x\t%s\n", (*curIns).first, (*curIns).second.c_str());
      curIns++;
   }


}

void Disassembler::insBad(unsigned opCode)
{
   ostringstream ss;
   ss << "BAD OPCODE 0x" << setfill('0') << setw(4) << hex << opCode;

   pair<unsigned int, string> curIns;
   curIns.first = theAddress;
   curIns.second = ss.str();

   theDisassembly.push_back(curIns);

   // theAddress += 0x2;
}

