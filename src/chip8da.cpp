#include <stdio.h>
#include <string.h>
#include "InstDecoder.h"
#include "RecursiveDisass.h"

void linearDisassemble(FILE* romFile, bool hideAddress)
{
   unsigned int address = 0x200;

   unsigned char instruction[3];
   instruction[2] = 0; // null terminate string

   Disassembler da;
   da.printAddresses(!hideAddress);

   while(true)
   {
      instruction[0] = fgetc(romFile);

      if (feof(romFile))
      {
         printf("; End of file\n");
         break;
      }

      instruction[1] = fgetc(romFile);

      if (feof(romFile))
      {
         printf("End of file.  Ended with an odd number of bytes!\n");
         printf("i[0] = 0x%02x, i[1] = 0x%02x\n", instruction[0], instruction[1]);
         break;
      }

      // Decode instruction
      bool success = da.decodeInstruction( instruction);

      if (!success)
      {
         printf("Error at address=0x%04x, opcode=0x%02x%02x\n", address, instruction[0], instruction[1]);
      }

      address += 2;
   }

   da.printDisassembly();
}

void recursiveDisassemble(FILE* romFile, bool hideAddress)
{
   unsigned int address = 0x200;

   unsigned char instruction;

   RecursiveDisass da;
   da.printAddresses(!hideAddress);

   while(true)
   {
      instruction = fgetc(romFile);

      if (feof(romFile))
      {
         printf("End of file\n");
         break;
      }

      // Decode instruction
      da.loadRomByte(instruction);

   }

   da.decodeCodeSegment(address);

   da.printDisassembly();
}

int main(int argc, char** argv)
{
   if (argc < 3)
   {
      printf("Usage:  disasschip8 <mode> <-h> romfile\n");
      printf("  Modes:  -l is linear disassemble (disassembles everything as code)\n");
      printf("          -r is recursive disassemble (disassemble only what it thinks is code)\n");
      printf("  -h = hides address field, useful for generating code that can be reassembled\n");
      return -1;
   }

   FILE* romFile = fopen(argv[argc - 1], "r");

   if (romFile == NULL)
   {
      printf("Error opening file:  %s\n", argv[1]);
      return -1;
   }

   bool hideAddress = false;
   if (strcmp(argv[2], "-h") == 0)
   {
      hideAddress = true;
   }

   printf("; Setting used by the chipper assembler\n");
   printf("option schip11\n");
   printf("option binary\n");
   printf("align off\n\n");

   if (strcmp(argv[1],"-r") == 0)
   {
      printf("; Recursive Disassembly\n");
      recursiveDisassemble(romFile, hideAddress);
   }
   else
   {
      printf("; Linear Disassembly\n");
      linearDisassemble(romFile, hideAddress);
   }

   fclose(romFile);
}
