#ifndef EMULATOR_H
#define EMULATOR_H

#include "InstDecoder.h"
#include "EmulationScreen.h"
#include <QVector>
#include <QSet>
#include <QStack>
#include <QMutex>
#include <QThread>
#include <QSemaphore>
#include <QDateTime>

class Emulator : public InstDecoder, public QThread
{
public:
   Emulator();

   void clearRomData();

   void loadRomData(unsigned char byte);

   void resetEmulator();

   void executeInstruction();

   void keyDown(unsigned char key);

   void keyUp(unsigned char key);

   void setEmulationScreen(EmulationScreen* screen);

   unsigned char getRegister(unsigned char reg);

   unsigned int getIP();

   unsigned int getIndexRegister();

   unsigned char getDelayTimer();

   void stopEmulator();

   QStack<unsigned int> getStack();

   void setBreakpoint(unsigned int addr);

   void clearBreakpoints();

   void setInstPerSecond(int ips);

   void enableSound(bool enable);

protected:

   QVector<unsigned char> theCpuRegisters;

   QStack<unsigned int> theCpuStack;

   QVector<unsigned char> theMemory;

   QVector<unsigned char> theRomData;

   QSet<unsigned char> theKeysDown;

   QSet<unsigned int> theBreakpoints;

   QMutex theKeysLock;

   unsigned int theIndexRegister;

   EmulationScreen* theScreen;

   bool theStopFlag;

   QDateTime theDelayTimerExpiration;

   int theInstructionPeriodMicroSecs;

   bool theSoundEnabled;

   void run();

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
