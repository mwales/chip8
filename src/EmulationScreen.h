#ifndef EMULATIONSCREEN_H
#define EMULATIONSCREEN_H

#include <vector>

using namespace std;

/**
 * Emulation screen is an abstract class using only standard C++ stuff.  The emulator needs this class to help with
 * the XOR function when drawing a sprite and how it can set the vF register.
 *
 * Keeping this as an abstract class,  I can implement the actual drawing in any framework from Qt, ncurses, or SDL.
 */
class EmulationScreen
{

public:
   explicit EmulationScreen();

   void setPixel(int x, int y, bool val);

   /**
    * Draws a sprite on the screen at the provided coordinates
    *
    * @param x
    * @param y
    * @param spriteData Sprite data, should be max 15 in length
    * @return True if one of the pixels on the screen was cleared (pixel was set, and the sprite tried to set the pixel
    *   again
    */
   virtual bool drawSprite(unsigned int x, unsigned int y, vector<unsigned char> spriteData);

   virtual void clearScreen();

protected:

   static const int X_RES;
   static const int Y_RES;

   vector< vector< bool > > thePixels;

};

#endif // EMULATIONSCREEN_H
