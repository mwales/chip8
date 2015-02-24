#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <string>
#include <vector>
#include <map>

#include "InstDecoder.h"

using namespace std;

class Disassembler : public InstDecoder
{
public:
   Disassembler();

   virtual void printDisassembly();

   void printAddresses(bool printAddr);

protected:

   map <unsigned int, string> theDisassembly;

   bool thePrintAddressOption;

   void insBad(unsigned opCode);

   void insClearScreen();

   void insReturnFromSub();

   void insJump(unsigned addr);

   void insCall(unsigned addr);

   void insSetIndexReg(unsigned addr);

   void insJumpWithOffset(unsigned addr);

   void insSkipNextIfRegEqConst(unsigned reg, unsigned val);

   void insSkipNextIfRegNotEqConst(unsigned reg, unsigned val);

   void insSetReg(unsigned reg, unsigned val);

   void insAddReg(unsigned reg, unsigned val);

   void insRandomNum(unsigned reg, unsigned mask);

   void insSkipNextIfRegEq(unsigned reg1, unsigned reg2);

   void insSkipNextIfRegNotEq(unsigned reg1, unsigned reg2);

   void insSetRegToRegVal(unsigned regToSet, unsigned regVal);

   void insOrReg(unsigned reg, unsigned otherReg);

   void insAndReg(unsigned reg, unsigned otherReg);

   void insXorReg(unsigned reg, unsigned otherReg);

   void insAddRegs(unsigned reg, unsigned otherReg);

   void insSubRegs(unsigned reg, unsigned otherReg);

   void insSubRegsOtherOrder(unsigned reg, unsigned otherReg);

   void insRightShift(unsigned reg);

   void insLeftShift(unsigned reg);

   void insSkipNextIfKeyPressed(unsigned reg);

   void insSkipNextIfKeyNotPressed(unsigned reg);

   void insWaitForKeyPress(unsigned reg);

   void insSetRegToDelayTimer(unsigned reg);

   void insSetDelayTimer(unsigned reg);

   void insSetSoundTimer(unsigned reg);

   void insAddRegToIndexReg(unsigned reg);

   void insSetIndexToCharInReg(unsigned reg);

   void insSetIndexMemoryToRegBcd(unsigned reg);

   void insStoreRegsToIndexMemory(unsigned reg);

   void insLoadRegsFromIndexMemory(unsigned reg);

   void insDrawSprite(unsigned xReg, unsigned yReg, unsigned height);

   // Super Chip-8 Instructions
   void insScrollDown(unsigned char numLines);

   void insScrollLeft();

   void insScrollRight();

   void insQuitEmulator();

   void insEnterLowResMode();

   void insEnterHighResMode();

   void insSaveHp48Flags(unsigned char reg);

   void insLoadHp48Flags(unsigned char reg);

   void insSetIndexToHiResCharInReg(unsigned char reg);
};

#endif // DISASSEMBLER_H
