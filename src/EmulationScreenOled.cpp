#include "EmulationScreenOled.h"

EmulationScreenOled::EmulationScreenOled(QObject* parent):
   QObject(parent)
{
   theRepaintTimer.setInterval(10); // 100 Hz
   connect(&theRepaintTimer, SIGNAL(timeout()),
           this, SLOT(forceRepaint()));

   theRepaintTimer.start();

   theOledScreen.begin();
}

void EmulationScreenOled::forceRepaint ()
{
   int xRes = theXRes;
   int yRes = theYRes;

   int pixelWidth = theOledScreen.width() / xRes;
   int pixelHeight = theOledScreen.height() / yRes;

   thePixelsLock.lockForRead();
   for(int y = 0; y < yRes; y++)
   {
      for(int x = 0; x < xRes; x++)
      {
         if (!thePixels[y][x])
         {
            theOledScreen.fillRect(x,y,pixelWidth,pixelHeight, WHITE);
         }
      }
   }
   thePixelsLock.unlock();

}
