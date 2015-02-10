#include "RecursiveDisass.h"

#define ROM_START_ADDRESS  0x200

RecursiveDisass::RecursiveDisass()
{
}

void RecursiveDisass::loadRomByte(unsigned char romData)
{
   theRomData.push_back(romData);
}

void RecursiveDisass::decodeCodeSegment(unsigned char startAddress)
{
   theEndOfBlockDetected = false;

   while(!theEndOfBlockDetected)
   {
      decodeCodeSegmentBody();
   }

   theDisassembly[startAddress].insert(0, "== BEGINNING OF CODE BLOCK ==");

   if (theBlockStartAddresses.size() > 0)
   {
      unsigned int nextAddress = theBlockStartAddresses.back();
      theBlockStartAddresses.pop();

      decodeCodeSegment(nextAddress);
   }
}

void RecursiveDisass::decodeCodeSegmentBody()
{
   if ( ((theAddress - ROM_START_ADDRESS + 1) > theRomData.size() ) &&
        (theAddress >= ROM_START_ADDRESS) )
   {
      // Address not within the rom data
      theEndOfBlockDetected = true;
      return;
   }

   if (theCodeBytes[theAddress])
   {
      // We have reached an instruction that has already been decoded
      theEndOfBlockDetected = true;
      return;
   }

   char opCode[2];
   opCode[0] = theRomData[theAddress - ROM_START_ADDRESS];
   opCode[1] = theRomData[theAddress - ROM_START_ADDRESS + 1];
   decodeInstruction(opCode);

   theCodeBytes[theAddress] = true;
   theCodeBytes[theAddress + 1] = true;
}



bool RecursiveDisass::theEndOfBlockDetected()
{

}



// End of code block instructions that need to be detected
void RecursiveDisass::insReturnFromSub()
{
   theEndOfBlockDetected = true;
   Disassembler::insReturnFromSub();
}

void RecursiveDisass::insJumpWithOffset(unsigned addr)
{
   theEndOfBlockDetected = true;
   Disassembler::insJumpWithOffset();
}

void RecursiveDisass::insBad(unsigned opCode)
{
   theEndOfBlockDetected = true;
   Disassembler::insBad();
}


// Instructions that will create start of blocks
void RecursiveDisass::insCall(unsigned addr)
{
   theBlockStartAddresses(addr);
}


// Instructions that tell both the start and stop points of code blocks
void RecursiveDisass::insJump(unsigned addr)
{
   theBlockStartAddresses(addr);

   theEndOfBlockDetected = true;
   Disassembler::insJump();
}

void RecursiveDisass::insSkipNextIfRegEqConst(unsigned reg, unsigned val)
{
   theBlockStartAddresses(theAddress + 4);

   theEndOfBlockDetected = true;
   Disassembler::insSkipNextIfRegEqConst();
}

void RecursiveDisass::insSkipNextIfRegNotEqConst(unsigned reg, unsigned val)
{
   theBlockStartAddresses(theAddress + 4);

   theEndOfBlockDetected = true;
   Disassembler::insSkipNextIfRegNotEqConst();
}

void RecursiveDisass::insSkipNextIfRegEq(unsigned reg1, unsigned reg2)
{
   theBlockStartAddresses(theAddress + 4);

   theEndOfBlockDetected = true;
   Disassembler::insSkipNextIfRegEq();
}

void RecursiveDisass::insSkipNextIfRegNotEq(unsigned reg1, unsigned reg2)
{
   theBlockStartAddresses(theAddress + 4);

   theEndOfBlockDetected = true;
   Disassembler::insSkipNextIfRegNotEq();
}

void RecursiveDisass::insSkipNextIfKeyPressed(unsigned reg)
{
   theBlockStartAddresses(theAddress + 4);

   theEndOfBlockDetected = true;
   Disassembler::insSkipNextIfKeyPressed();
}

void RecursiveDisass::insSkipNextIfKeyNotPressed(unsigned reg)
{
   theBlockStartAddresses(theAddress + 4);

   theEndOfBlockDetected = true;
   Disassembler::insSkipNextIfKeyNotPressed();
}

void RecursiveDisass::printDisassembly()
{
   unsigned int curAddress = ROM_START_ADDRESS;
   while (curAddress < ROM_START_ADDRESS + theRomData.size())
   {
      if (theCodeBytes[curAddress])
      {
         // Print out instructions
         printf("0x%04x\t%s\n", (*curIns).first, (*curIns).second.c_str());
      }
   }

   vector< pair<unsigned int, string> >::iterator curIns = theDisassembly.begin();
   while(curIns != theDisassembly.end())
   {
      printf("0x%04x\t%s\n", (*curIns).first, (*curIns).second.c_str());
      curIns++;
   }


}

