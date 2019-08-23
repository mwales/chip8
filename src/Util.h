#ifndef UTIL_H
#define UTIL_H

#include <iostream>

/**
 * Small little utility functions
 */
class Util
{
public:

   static std::string charToHex8(char val);

   static std::string toHex16(unsigned int val);

   static std::string charToNibble(char val);
};

#endif // UTIL_H
