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

/**
 * Chip-8 Emulator class
 *
 * Running this thread runs the emulator at whatever instructions per second we are configured
 * to run at.
 */
class Emulator : public InstDecoder, public QThread
{
public:
   Emulator();


   void clearRomData();
   void loadRomData(unsigned char byte);

   // Connect to GUI keypress up/down events
   void keyDown(unsigned char key);
   void keyUp(unsigned char key);

   void setEmulationScreen(EmulationScreen* screen);

   // Accessors for the Chip-8 interpreter / CPU internal state
   unsigned char getRegister(unsigned char reg);
   unsigned int getIP();
   unsigned int getIndexRegister();
   unsigned char getDelayTimer();
   QStack<unsigned int> getStack();

   // Controls to play, pause, reset the emulator
   void stopEmulator();
   void resetEmulator();
   void executeInstruction();

   void setBreakpoint(unsigned int addr);
   void clearBreakpoints();

   void setInstPerSecond(int ips);

   void enableSound(bool enable);

protected:

   // Internal state variables for the interpreter
   QVector<unsigned char> theCpuRegisters;
   QStack<unsigned int> theCpuStack;
   QVector<unsigned char> theMemory;
   unsigned int theIndexRegister;
   QDateTime theDelayTimerExpiration;

   /**
    * ROM data stored seperately incase a program rewrites the code segment while it is running.
    * Otherwise, reset would have been implemented by setting IP to 0x200 again.
    */
   QVector<unsigned char> theRomData;

   /**
    * Stop the emulator when it is running and hits one of these addresses with the IP
    */
   QSet<unsigned int> theBreakpoints;

   // Key press event lock and list
   QSet<unsigned char> theKeysDown;
   QMutex theKeysLock;


   EmulationScreen* theScreen;

   bool theStopFlag;



   /**
    * How fast the emulator should process instructions
    */
   int theInstructionPeriodMicroSecs;

   bool theSoundEnabled;

   void run();

   /**
    * Loads the Chip-8 font sprites into memory below 0x200
    */
   void loadFonts();



   // Chip-8 instruction handlers

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
