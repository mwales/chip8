#include <stdio.h>

#include "InstDecoder.h"

#define MASK_PREFIX               0xf000
#define MASK_ADDRESS              0x0fff
#define MASK_DEST_REG             0x0f00
#define MASK_CONSTANTS            0x00ff
#define MASK_SRC_REG              0x00f0

#define PREFIX_NO_PARAMS          0x0000
#define PREFIX_JUMP               0x1000
#define PREFIX_CALL               0x2000
#define PREFIX_SKIP_NEXT_EQ_CONST 0x3000
#define PREFIX_SKIP_NEXT_NE_CONST 0x4000
#define PREFIX_SKIP_NEXT_EQ_REG   0x5000
#define PREFIX_SET_REG            0x6000
#define PREFIX_ADD_REG            0x7000
#define PREFIX_ARITHMETIC         0x8000
#define PREFIX_SKIP_NEXT_NE_REG   0x9000
#define PREFIX_SET_INDEX          0xa000
#define PREFIX_JUMP_OFFSET        0xb000
#define PREFIX_RANDOM             0xc000
#define PREFIX_SPRITE_DRAW        0xd000
#define PREFIX_KEY_PRESS          0xe000
#define PREFIX_SINGLE_REG         0xf000

#define OPCODE_CLEAR_SCREEN       0x00e0
#define OPCODE_RETURN             0x00ee


InstDecoder::InstDecoder()
{
   theAddress = 0x200 - 0x2; // Start address for all ROMs
}

bool InstDecoder::decodeInstruction(unsigned char* opCode)
{
   unsigned int word = opCode[0] * 0x100 + opCode[1];
   unsigned int prefix = word & MASK_PREFIX;

   bool retCode = false;

   theAddress += 2;

   //printf("word = 0x%04x, prefix=0x%04x\n", word, prefix);

   switch(prefix)
   {
      case PREFIX_JUMP:
      case PREFIX_CALL:
      case PREFIX_SET_INDEX:
      case PREFIX_JUMP_OFFSET:
         // Operations of the form ?NNN
         //  NNN = address
         retCode = decodeAddressBasedIns(word);
         break;

      case PREFIX_SKIP_NEXT_EQ_CONST:
      case PREFIX_SKIP_NEXT_NE_CONST:
      case PREFIX_SET_REG:
      case PREFIX_ADD_REG:
      case PREFIX_RANDOM:
         // Operations of the form ?XNN
         //  X = register
         //  NN = constant
         retCode = decodeRegisterConstantIns(word);
         break;

      case PREFIX_SKIP_NEXT_EQ_REG:
      case PREFIX_SKIP_NEXT_NE_REG:
      case PREFIX_ARITHMETIC: // There are 2 exceptions (shift operators) here, that will be handled later
         // Operations of the form ?XY?
         //  X = register
         //  Y = register
         retCode = decodeTwoRegistersIns(word);
         break;

      case PREFIX_KEY_PRESS:
      case PREFIX_SINGLE_REG:
         // Operations of the form ?X??
         //  X = register
         retCode = decodeOneRegisterIns(word);
         break;

      case PREFIX_SPRITE_DRAW:
         // Sprite draw operation
         retCode = decodeSpriteIns(word);
         break;

      case PREFIX_NO_PARAMS:
         // Op-code only operations, no parameters, etc
         retCode = decodeParamlessIns(word);
         break;
   }

   if (!retCode)
   {
      insBad(word);
   }

   return retCode;
}


bool InstDecoder::decodeAddressBasedIns(unsigned int opCode)
{
   unsigned int addr = opCode & MASK_ADDRESS;

   switch(opCode & MASK_PREFIX)
   {
      case PREFIX_JUMP:
         insJump(addr);
         return true;

      case PREFIX_CALL:
         insCall(addr);
         return true;

      case PREFIX_SET_INDEX:
         insSetIndexReg(addr);
         return true;

      case PREFIX_JUMP_OFFSET:
         insJumpWithOffset(addr);
         return true;
   }

   return false;
}

bool InstDecoder::decodeParamlessIns(unsigned int opCode)
{
   switch(opCode)
   {
      case OPCODE_CLEAR_SCREEN:
         insClearScreen();
         return true;

      case OPCODE_RETURN:
         insReturnFromSub();
         return true;
   }

   return false;
}

bool InstDecoder::decodeRegisterConstantIns(unsigned int opCode)
{
   unsigned regNum =  (opCode & MASK_DEST_REG) >> 8;
   unsigned value = opCode & MASK_CONSTANTS;

   //printf("decodeRegisterConstantIns opCode=0x%04x, regNum = %ud, value = %ud\n", opCode, regNum, value);

   switch(opCode & MASK_PREFIX)
   {
      case PREFIX_SKIP_NEXT_EQ_CONST:
         insSkipNextIfRegEqConst(regNum, value);
         return true;

      case PREFIX_SKIP_NEXT_NE_CONST:
         insSkipNextIfRegNotEqConst(regNum, value);
         return true;

      case PREFIX_SET_REG:
         insSetReg(regNum, value);
         return true;

      case PREFIX_ADD_REG:
         insAddReg(regNum, value);
         return true;

      case PREFIX_RANDOM:
         insRandomNum(regNum, value);
         return true;
   }

   return false;
}

bool InstDecoder::decodeTwoRegistersIns(unsigned int opCode)
{
   unsigned int dest = (opCode & 0x0f00) >> 8;
   unsigned int src = (opCode & 0x00f0) >> 4;

   unsigned int noParmOpCode = opCode & 0xf00f;

   //printf("decodingTwoRegisterIns.  opcode = 0x%04x, noParmOpCode=0x%04x\n", opCode, noParmOpCode);

   switch(noParmOpCode)
   {
      case 0x5000:
         insSkipNextIfRegEq(dest, src);
         return true;

      case 0x9000:
         insSkipNextIfRegNotEq(dest, src);
         return true;

      case 0x8000:
         insSetRegToRegVal(dest, src);
         return true;

      case 0x8001:
         insOrReg(dest, src);
         return true;

      case 0x8002:
         insAndReg(dest, src);
         return true;

      case 0x8003:
         insXorReg(dest, src);
         return true;

      case 0x8004:
         insAddRegs(dest, src);
         return true;

      case 0x8005:
         insSubRegs(dest, src);
         return true;

      case 0x8007:
         insSubRegsOtherOrder(dest, src);
         return true;

      case 0x8006:
         insRightShift(dest);
         return true;

      case 0x800e:
         insLeftShift(dest);
         return true;
   }

   return false;
}

bool InstDecoder::decodeOneRegisterIns(unsigned int opCode)
{
   unsigned int reg = (opCode & MASK_DEST_REG) >> 8;

   unsigned int noParmOpCode = opCode & 0xf0ff;

   //printf("decodeOneRegisterIns.  opcode = 0x%04x, noParmOpCode=0x%04x\n", opCode, noParmOpCode);

   switch(noParmOpCode)
   {
      case 0xe09e:
         insSkipNextIfKeyPressed(reg);
         return true;

      case 0xe0a1:
         insSkipNextIfKeyNotPressed(reg);
         return true;

      case 0xf00a:
         insWaitForKeyPress(reg);
         return true;

      case 0xf007:
         insSetRegToDelayTimer(reg);
         return true;

      case 0xf015:
         insSetDelayTimer(reg);
         return true;

      case 0xf018:
         insSetSoundTimer(reg);
         return true;

      case 0xf01e:
         insAddRegToIndexReg(reg);
         return true;

      case 0xf029:
         insSetIndexToCharInReg(reg);
         return true;

      case 0xf033:
         insSetIndexMemoryToRegBcd(reg);
         return true;

      case 0xf055:
         insStoreRegsToIndexMemory(reg);
         return true;

      case 0xf065:
         insLoadRegsFromIndexMemory(reg);
         return true;

   }

   return false;
}

bool InstDecoder::decodeSpriteIns(unsigned int opCode)
{
   unsigned int xCoordReg = (opCode & 0x0f00) >> 8;
   unsigned int yCoordReg = (opCode & 0x00f0) >> 4;
   unsigned int heightVal = (opCode & 0x000f);

   insDrawSprite(xCoordReg, yCoordReg, heightVal);

   return true;
}  

