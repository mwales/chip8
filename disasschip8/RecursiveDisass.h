#ifndef RECURSIVEDISASS_H
#define RECURSIVEDISASS_H

#include <vector>
#include <utility>
#include <set>
#include "Disassembler.h"

class RecursiveDisass : public Disassembler
{
public:
   RecursiveDisass();

   void loadRomByte(unsigned char romData);

   void decodeCodeSegment(unsigned int startAddress);

   void decodeCodeSegmentBody();

   void printDisassembly();

protected:

   bool theEndOfBlockDetected;

   vector< unsigned char > theRomData;

   vector< unsigned int > theBlockStartAddresses;

   void addCodeBlock(unsigned int startAddress);

   set< unsigned int > theCodeBlockHistory;

   // End of code block instructions that need to be detected
   void insReturnFromSub();
   void insJumpWithOffset(unsigned addr); // Destination is the start of a block, but we can't easily determine destination
   void insBad(unsigned opCode);

   // Instructions that will create start of blocks
   void insCall(unsigned addr);

   // Branch instructions.  These typically have a sequence of:
   //   test
   //   jmp 0xbeef   <- We process this as part of the code block
   //   more code    <- Add this to list of code blocks (current address + 2 (opcodes) x 2 (opcode size)
   void insSkipNextIfRegEqConst(unsigned reg, unsigned val);
   void insSkipNextIfRegNotEqConst(unsigned reg, unsigned val);
   void insSkipNextIfRegEq(unsigned reg1, unsigned reg2);
   void insSkipNextIfRegNotEq(unsigned reg1, unsigned reg2);
   void insSkipNextIfKeyPressed(unsigned reg);
   void insSkipNextIfKeyNotPressed(unsigned reg);

   // Instructions that tell both the start and stop points of code blocks
   void insJump(unsigned addr);

};

#endif // RECURSIVEDISASS_H
