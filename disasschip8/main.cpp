#include <stdio.h>
#include "InstDecoder.h"
#include "Disassembler.h"

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

   unsigned char instruction[3];
   instruction[2] = 0; // null terminate string

   Disassembler da;

   while(true)
   {
      instruction[0] = fgetc(romFile);

      if (feof(romFile))
      {
         printf("End of file\n");
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

   fclose(romFile);

   printf("\nDisassembler Output:\n");

   da.printDisassembly();
 
}
