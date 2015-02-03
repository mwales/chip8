


#ifndef INSTDECODER_H
#define INSTDECODER_H

class InstDecoder
{

public:

   InstDecoder();

   /**
    * Decodes any instruction.  opCode is 2 byte array.
    * Returns false if decode fails (invalid operation)
    */
   bool decodeInstruction(unsigned char* opCode);


protected:

   unsigned theAddress;

   /**
    * Decodes instructions that the opCode contains a 12-bit address
    */
   bool decodeAddressBasedIns(unsigned int opCode);

   /**
    * Decodes instructions that have no parameters
    */
   bool decodeParamlessIns(unsigned int opCode);

   /**
    * Decodes instructions that have register and a constant in the opcode
   */
   bool decodeRegisterConstantIns(unsigned int opCode);

   /**
    * Decodes instructions that have two registers in the opcode
    */
   bool decodeTwoRegistersIns(unsigned int opCode);

   /**
    * Decodes instructions that have 1 register in the opcode
    */
   bool decodeOneRegisterIns(unsigned int opCode);

   /**
    * Decodes the sprite command, which has 2 registers and a 4-bit height value in the opcode
    */
   bool decodeSpriteIns(unsigned int opCode);
  
   /**
    * @section The exact instruction decoders will be left up to the children implementations
    */

   virtual void insClearScreen() = 0;

   virtual void insReturnFromSub() = 0;

   virtual void insJump(unsigned addr) = 0;
 
   virtual void insCall(unsigned addr) = 0;

   virtual void insSetIndexReg(unsigned addr) = 0;

   virtual void insJumpWithOffset(unsigned addr) = 0;

   virtual void insSkipNextIfRegEqConst(unsigned reg, unsigned val) = 0;

   virtual void insSkipNextIfRegNotEqConst(unsigned reg, unsigned val) = 0;

   virtual void insSetReg(unsigned reg, unsigned val) = 0;

   virtual void insAddReg(unsigned reg, unsigned val) = 0;

   virtual void insRandomNum(unsigned reg, unsigned mask) = 0;

   virtual void insSkipNextIfRegEq(unsigned reg1, unsigned reg2) = 0;

   virtual void insSkipNextIfRegNotEq(unsigned reg1, unsigned reg2) = 0;

   virtual void insSetRegToRegVal(unsigned regToSet, unsigned regVal) = 0;

   virtual void insOrReg(unsigned reg, unsigned otherReg) = 0;
  
   virtual void insAndReg(unsigned reg, unsigned otherReg) = 0;

   virtual void insXorReg(unsigned reg, unsigned otherReg) = 0;

   virtual void insAddRegs(unsigned reg, unsigned otherReg) = 0;

   virtual void insSubRegs(unsigned reg, unsigned otherReg) = 0;

   virtual void insSubRegsOtherOrder(unsigned reg, unsigned otherReg) = 0;

   virtual void insRightShift(unsigned reg) = 0;

   virtual void insLeftShift(unsigned reg) = 0;

   virtual void insSkipNextIfKeyPressed(unsigned reg) = 0;

   virtual void insSkipNextIfKeyNotPressed(unsigned reg) = 0;

   virtual void insWaitForKeyPress(unsigned reg) = 0;

   virtual void insSetRegToDelayTimer(unsigned reg) = 0;

   virtual void insSetDelayTimer(unsigned reg) = 0;

   virtual void insSetSoundTimer(unsigned reg) = 0;

   virtual void insAddRegToIndexReg(unsigned reg) = 0;

   virtual void insSetIndexToCharInReg(unsigned reg) = 0;

   virtual void insSetIndexMemoryToRegBcd(unsigned reg) = 0;

   virtual void insStoreRegsToIndexMemory(unsigned reg) = 0;

   virtual void insLoadRegsFromIndexMemory(unsigned reg) = 0;

   virtual void insDrawSprite(unsigned xReg, unsigned yReg, unsigned height) = 0;

   virtual void insBad(unsigned opCode) = 0;

};




#endif
