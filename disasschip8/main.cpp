#include <stdio.h>
#include "InstDecoder.h"
#include "RecursiveDisass.h"

int main(int argc, char** argv)
{
   if (argc < 2)
   {
      printf("Usage:  disasschip8 romfile\n");
      return -1;
   }

   FILE* romFile = fopen(argv[1], "r");

   if (romFile == NULL)
   {
      printf("Error opening file:  %s\n", argv[1]);
      return -1;
   }

   unsigned int address = 0x200;

   unsigned char instruction;

   RecursiveDisass da;

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

   fclose(romFile);

   printf("\nDisassembler Output:\n");

   da.decodeCodeSegment(0x200);

   da.printDisassembly();
 
}
