#include "Util.h"

#include <sstream>
#include <iomanip>

std::string Util::charToHex8(char val)
{
   unsigned int intVal = val & 0xff;

   std::ostringstream oss;
   oss << "0x" << std::setfill('0') << std::setw(2) << std::hex << intVal;
   return oss.str();
}

std::string Util::toHex16(unsigned int val)
{
   std::ostringstream oss;
   oss << "0x" << std::setfill('0') << std::setw(4) << std::hex << val;
   return oss.str();
}

std::string Util::charToNibble(char val)
{
   std::ostringstream oss;
   oss << std::hex << (val & 0xf);
   return oss.str();
}
