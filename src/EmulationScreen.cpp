
#include <QtDebug>
#include <QString>
#include "EmulationScreen.h"

const int EmulationScreen::X_RES_STD = 64;
const int EmulationScreen::Y_RES_STD = 32;
const int EmulationScreen::X_RES_SCHIP = 128;
const int EmulationScreen::Y_RES_SCHIP = 64;

EmulationScreen::EmulationScreen():
   theXRes(X_RES_STD),
   theYRes(Y_RES_STD)
{
   thePixelsLock.lockForWrite();
   for(int y = 0; y < Y_RES_SCHIP; y++)
   {
      QVector<bool> curRowOfPixels;
      for(int x = 0; x < X_RES_SCHIP; x++)
      {
         curRowOfPixels.push_back(false);
      }
      thePixels.push_back(curRowOfPixels);
   }
   thePixelsLock.unlock();
}

void EmulationScreen::hiResModeEnable(bool enable)
{
   if (enable)
   {
      theXRes = X_RES_SCHIP;
      theYRes = Y_RES_SCHIP;
      qDebug() << "Entering High-Res Mode";
   }
   else
   {
      theXRes = X_RES_STD;
      theYRes = Y_RES_STD;
      qDebug() << "Entering Low-Res Mode";
   }
}

void EmulationScreen::setPixel(int x, int y, bool val)
{
   if ( (x >= theXRes) || (y >= theYRes) )
   {
      //qDebug() << "Pixel (" << x "," << y << ") drawn outside the drawable area";
   }

   thePixelsLock.lockForRead();
   thePixels[y % theYRes][x % theXRes] = val;
   thePixelsLock.unlock();
}

bool EmulationScreen::drawSprite(unsigned int x, unsigned int y, QVector<unsigned char> spriteData)
{
   bool pixelCleared = false;

   thePixelsLock.lockForRead();

   unsigned int row = y;
   for(int rowsProcessed = 0; rowsProcessed < spriteData.size(); rowsProcessed++)
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


         col = (col + 1) % theXRes;
      }

      row = (row + 1) % theYRes;
   }

   thePixelsLock.unlock();

   return pixelCleared;
}

bool EmulationScreen::drawSuperSprite(unsigned int x, unsigned int y, QVector<unsigned char> spriteData)
{
   if (spriteData.size() != 32)
   {
      qDebug() << "Invalid data for drawSuperSprite command.  Size = " << spriteData.size();
      return false;
   }

   bool pixelCleared = false;
   unsigned int row = y;
   int spriteByteCounter = 0;

   thePixelsLock.lockForRead();

   for(unsigned int rowsProcessed = 0; rowsProcessed < 16; rowsProcessed++)
   {
      unsigned int col = x;
      for(unsigned int colsProcessed = 0; colsProcessed < 16; colsProcessed++)
      {
         unsigned char mask = 0x1 << (7- (colsProcessed % 8));

         if ( (mask & spriteData[spriteByteCounter]) &&
              thePixels[row][col] )
         {
            // Bit is going to be cleared
            pixelCleared = true;
            thePixels[row][col] = false;
         }
         else
         {
            thePixels[row][col] = thePixels[row][col] || (mask & spriteData[spriteByteCounter]);
         }


         col = (col + 1) % theXRes;

         if (colsProcessed == 7)
         {
            // sprites are 2 bytes wide, move to the next byte halfway through drawing the row
            spriteByteCounter++;
         }
      }

      // the next line is always a next sprite byte
      spriteByteCounter++;

      row = (row + 1) % theYRes;
   }

   thePixelsLock.unlock();

   return pixelCleared;
}

void EmulationScreen::clearScreen()
{
   thePixelsLock.lockForRead();

   for(int y = 0; y < Y_RES_SCHIP; y++)
   {
      for(int x = 0; x < X_RES_SCHIP; x++)
      {
         thePixels[y][x] = false;
      }

   }

   thePixelsLock.unlock();
}

void EmulationScreen::shiftDown(int numLines)
{
   if (theXRes == X_RES_STD)
   {
      // Shift instruction received while in low-res mode.  Shift is 1/s of the pixels.  Not doing the half pixel
      // shift that David Winter talks about, but will complain if I see it

      if (numLines & 0x1)
      {
         qDebug() << "Vertical shift of " << numLines << " while in low-res mode results in a half-pixel shift, not implemented";
      }

      numLines /= 2;
   }

   thePixelsLock.lockForWrite();
   for(int y = 0; y < numLines; y++)
   {
      QVector<bool> curRowOfPixels;
      for(int x = 0; x < X_RES_SCHIP; x++)
      {
         curRowOfPixels.push_back(false);
      }

      // Insert a row at the beginning, pop one off the back
      thePixels.insert(0, curRowOfPixels);
      thePixels.pop_back();
   }
   thePixelsLock.unlock();

}

void EmulationScreen::shiftLeft()
{
   int numCols = 4;
   if (theXRes == X_RES_STD)
   {
      numCols /= 2;
   }

   thePixelsLock.lockForWrite();
   for(int y = 0; y < Y_RES_SCHIP; y++)
   {
      for(int x = 0; x < numCols; x++)
      {
         thePixels[y].remove(0);
         thePixels[y].append(false);
      }

   }

   thePixelsLock.unlock();
}

void EmulationScreen::shiftRight()
{
   int numCols = 4;
   if (theXRes == X_RES_STD)
   {
      numCols /= 2;
   }

   thePixelsLock.lockForWrite();
   for(int y = 0; y < Y_RES_SCHIP; y++)
   {
      for(int x = 0; x < numCols; x++)
      {
         thePixels[y].insert(0, false);
         thePixels[y].pop_back();
      }

   }
   thePixelsLock.unlock();
}


