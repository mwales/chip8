#ifndef RECURSIVEDISASS_H
#define RECURSIVEDISASS_H

#include <vector>
#include <utility>
#include "Disassembler.h"

class RecursiveDisass : public Disassembler
{
public:
   RecursiveDisass();

   void loadRomByte(unsigned char romData);

   void decodeCodeSegment(unsigned char startAddress);

   void decodeCodeSegmentBody();

   void printDisassembly()

protected:

   bool theEndOfBlockDetected;

   vector< unsigned char > theRomData;

   vector< pair<unsigned int, bool> > theCodeBytes;

   vector< unsigned int > theBlockStartAddresses;



   // End of code block instructions that need to be detected
   void insReturnFromSub();
   void insJumpWithOffset(unsigned addr); // Destination is the start of a block, but we can't easily determine destination
   void insBad(unsigned opCode);

   // Instructions that will create start of blocks
   void insCall(unsigned addr);

   // Instructions that tell both the start and stop points of code blocks
   void insJump(unsigned addr);
   void insSkipNextIfRegEqConst(unsigned reg, unsigned val);
   void insSkipNextIfRegNotEqConst(unsigned reg, unsigned val);
   void insSkipNextIfRegEq(unsigned reg1, unsigned reg2);
   void insSkipNextIfRegNotEq(unsigned reg1, unsigned reg2);
   void insSkipNextIfKeyPressed(unsigned reg);
   void insSkipNextIfKeyNotPressed(unsigned reg);

};

#endif // RECURSIVEDISASS_H
