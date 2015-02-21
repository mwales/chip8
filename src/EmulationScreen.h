#ifndef EMULATIONSCREEN_H
#define EMULATIONSCREEN_H

#include <QVector>
#include <QReadWriteLock>

/**
 * Emulation screen is an abstract class.  Now implemented using Qt because I needed a mutex facility that was cross
 * platofrm.  The emulator needs this class to help with the XOR function when drawing a sprite and how it can set the
 * vF register.
 *
 * Keeping this as an abstract class,  I can implement the actual drawing in any framework from Qt, ncurses, or SDL.
 */
class EmulationScreen
{

public:
   explicit EmulationScreen();

   void setPixel(int x, int y, bool val);

   void hiResModeEnable(bool enable);

   /**
    * Draws a sprite on the screen at the provided coordinates
    *
    * @param x
    * @param y
    * @param spriteData Sprite data, should be max 15 in length
    * @return True if one of the pixels on the screen was cleared (pixel was set, and the sprite tried to set the pixel
    *   again
    */
   virtual bool drawSprite(unsigned int x, unsigned int y, QVector<unsigned char> spriteData);

   virtual bool drawSuperSprite(unsigned int x, unsigned int y, QVector<unsigned char> spriteData);

   virtual void clearScreen();

   void shiftDown(int numLines);

   void shiftLeft();

   void shiftRight();



protected:

   static const int X_RES_STD;
   static const int Y_RES_STD;
   static const int X_RES_SCHIP;
   static const int Y_RES_SCHIP;

   int theXRes;
   int theYRes;

   QVector< QVector< bool > > thePixels;

   // When changing the structure of the pixels vector, lock first
   QReadWriteLock thePixelsLock;

};

#endif // EMULATIONSCREEN_H
