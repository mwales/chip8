#ifndef EMULATOR_H
#define EMULATOR_H

#include "InstDecoder.h"
#include <QVector>
#include <QSet>
#include <QStack>
#include <QMutex>

class Emulator : public InstDecoder
{
public:
   Emulator();

   void loadRomData(unsigned char byte);

   void resetEmulator();

   void executeInstruction();

   void keyDown(unsigned char key);

   void keyUp(unsigned char key);

protected:

   QVector<unsigned char> theCpuRegisters;

   QStack<unsigned int> theCpuStack;

   QVector<unsigned char> theMemory;

   QSet<unsigned char> theKeysDown;

   QMutex theKeysLock;

   unsigned int theIndexRegister;

   void loadFonts();

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

   void insBad(unsigned opCode);
};

#endif // EMULATOR_H
