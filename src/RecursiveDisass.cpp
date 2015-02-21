#include <stdio.h>

#include "RecursiveDisass.h"

#define ROM_START_ADDRESS  0x200

RecursiveDisass::RecursiveDisass()
{
}

void RecursiveDisass::loadRomByte(unsigned char romData)
{
   theRomData.push_back(romData);
}

void RecursiveDisass::decodeCodeSegment(unsigned int startAddress)
{
   //printf("%s - Address = 0x%04x\n", __PRETTY_FUNCTION__, startAddress);

   theEndOfBlockDetected = false;
   theAddress = startAddress;

   while(!theEndOfBlockDetected)
   {
      decodeCodeSegmentBody();
   }

   if (theDisassembly[startAddress].find("BEGINNING OF CODE BLOCK") == string::npos)
   {
      int numCharsInString = theDisassembly[startAddress].length();
      theDisassembly[startAddress].append(40 - numCharsInString, ' ');

      theDisassembly[startAddress].append("\t; == BEGINNING OF CODE BLOCK ==");
   }

   if (theBlockStartAddresses.size() > 0)
   {
      unsigned int nextAddress = theBlockStartAddresses.back();
      theBlockStartAddresses.pop_back();

      decodeCodeSegment(nextAddress);
   }
}

void RecursiveDisass::decodeCodeSegmentBody()
{
   //printf("%s - Address = 0x%04x\n", __PRETTY_FUNCTION__, theAddress);

   if ( ((theAddress - ROM_START_ADDRESS + 1) > theRomData.size() ) &&
        (theAddress >= ROM_START_ADDRESS) )
   {
      // Address not within the rom data
      printf("decoding segment body - end of block - address 0x%04x not within ROM\n", theAddress);
      theEndOfBlockDetected = true;
      return;
   }

   //if (theCodeBytes[theAddress])
   if (theDisassembly.find(theAddress - ROM_START_ADDRESS) != theDisassembly.end() )
   {
      // We have reached an instruction that has already been decoded
      printf("decoding segment body - end of block - address 0x%04x already decoded\n", theAddress);
      theEndOfBlockDetected = true;
      return;
   }

   //printf("%s - Address = 0x%04x\n", __PRETTY_FUNCTION__, startAddress);

   unsigned char opCode[2];
   opCode[0] = theRomData[theAddress - ROM_START_ADDRESS];
   opCode[1] = theRomData[theAddress - ROM_START_ADDRESS + 1];
   decodeInstruction(&opCode[0]);

}

void RecursiveDisass::addCodeBlock(unsigned int startAddress)
{
   if (theCodeBlockHistory.find(startAddress) == theCodeBlockHistory.end() )
   {
      // Block of code that has not been analyzed
      theBlockStartAddresses.push_back(startAddress);
      theCodeBlockHistory.insert(startAddress);
   }

}


// End of code block instructions that need to be detected
void RecursiveDisass::insReturnFromSub()
{
   theEndOfBlockDetected = true;
   Disassembler::insReturnFromSub();
}

void RecursiveDisass::insQuitEmulator()
{
   theEndOfBlockDetected = true;
   Disassembler::insQuitEmulator();
}

void RecursiveDisass::insJumpWithOffset(unsigned addr)
{
   theEndOfBlockDetected = true;
   Disassembler::insJumpWithOffset(addr);
}

void RecursiveDisass::insBad(unsigned opCode)
{
   theEndOfBlockDetected = true;
   Disassembler::insBad(opCode);
}


// Instructions that will create start of blocks
void RecursiveDisass::insCall(unsigned addr)
{
   addCodeBlock(addr);
   Disassembler::insCall(addr);
}

void RecursiveDisass::insSkipNextIfRegEqConst(unsigned reg, unsigned val)
{
   addCodeBlock(theAddress + 4);
   Disassembler::insSkipNextIfRegEqConst(reg, val);
}

void RecursiveDisass::insSkipNextIfRegNotEqConst(unsigned reg, unsigned val)
{
   addCodeBlock(theAddress + 4);
   Disassembler::insSkipNextIfRegNotEqConst(reg, val);
}

void RecursiveDisass::insSkipNextIfRegEq(unsigned reg1, unsigned reg2)
{
   addCodeBlock(theAddress + 4);
   Disassembler::insSkipNextIfRegEq(reg1, reg2);
}

void RecursiveDisass::insSkipNextIfRegNotEq(unsigned reg1, unsigned reg2)
{
   addCodeBlock(theAddress + 4);
   Disassembler::insSkipNextIfRegNotEq(reg1, reg2);
}

void RecursiveDisass::insSkipNextIfKeyPressed(unsigned reg)
{
   addCodeBlock(theAddress + 4);
   Disassembler::insSkipNextIfKeyPressed(reg);
}

void RecursiveDisass::insSkipNextIfKeyNotPressed(unsigned reg)
{
   addCodeBlock(theAddress + 4);
   Disassembler::insSkipNextIfKeyNotPressed(reg);
}


// Instructions that tell both the start and stop points of code blocks
void RecursiveDisass::insJump(unsigned addr)
{
   addCodeBlock(addr);

   theEndOfBlockDetected = true;
   Disassembler::insJump(addr);
}


void RecursiveDisass::printDisassembly()
{
   unsigned int curAddress = ROM_START_ADDRESS;
   while (curAddress < ROM_START_ADDRESS + theRomData.size())
   {
      if (theDisassembly.find(curAddress) != theDisassembly.end() )
      {
         // Print out instructions
         printf("0x%04x\t%s\n", curAddress, theDisassembly[curAddress].c_str());
         curAddress += 2;
      }
      else
      {
         // Print out data bytes
         unsigned char curDataByte = theRomData[curAddress - ROM_START_ADDRESS];

         // Binary representation (so we can see lines in the sprites)
         string spriteView;
         unsigned char spriteBits = curDataByte;
         for(int i = 0; i < 8; i++)
         {
            if (spriteBits & 0x1)
            {
               spriteView.insert(0, "#");
            }
            else
            {
               spriteView.insert(0, " ");
            }

            spriteBits >>= 1;
         }

         if ( (curDataByte >= '!') &&
              (curDataByte <= '~') )
         {
            // Print out printable ASCII characters
            printf("0x%04x\tDATA = 0x%02x\tGRAPHIC = %s\tASCII(%c)\n", curAddress, curDataByte, spriteView.c_str(), curDataByte);
         }
         else
         {
            // Print out data as hex only
            printf("0x%04x\tDATA = 0x%02x\tGRAPHIC = %s\n", curAddress, curDataByte, spriteView.c_str());
         }

         curAddress += 1;
      }
   }

}

