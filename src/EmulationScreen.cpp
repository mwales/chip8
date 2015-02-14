

#include "EmulationScreen.h"

const int EmulationScreen::X_RES = 64;
const int EmulationScreen::Y_RES = 32;

EmulationScreen::EmulationScreen()
{
   for(int y = 0; y < Y_RES; y++)
   {
      vector<bool> curRowOfPixels;
      for(int x = 0; x < X_RES; x++)
      {
         curRowOfPixels.push_back(false);
      }
      thePixels.push_back(curRowOfPixels);
   }
}

void EmulationScreen::setPixel(int x, int y, bool val)
{
   thePixels[y][x] = val;
}

bool EmulationScreen::drawSprite(unsigned int x, unsigned int y, vector<unsigned char> spriteData)
{
   bool pixelCleared = false;

   unsigned int row = y;
   for(unsigned int rowsProcessed = 0; rowsProcessed < spriteData.size(); rowsProcessed++)
   {
      unsigned int col = x;
      for(unsigned int colsProcessed = 0; colsProcessed < 8; colsProcessed++)
      {
         unsigned char mask = 0x1 << (7-colsProcessed);

         if ( (mask & spriteData[rowsProcessed]) &&
              thePixels[row][col] )
         {
            // Bit is going to be cleared
            pixelCleared = true;
            thePixels[row][col] = false;
         }
         else
         {
            thePixels[row][col] = thePixels[row][col] || (mask & spriteData[rowsProcessed]);
         }


         col = (col + 1) % X_RES;
      }

      row = (row + 1) % Y_RES;
   }

   return pixelCleared;
}

void EmulationScreen::clearScreen()
{
   for(int y = 0; y < Y_RES; y++)
   {
      for(int x = 0; x < X_RES; x++)
      {
         thePixels[y][x] = false;
      }

   }
}


