#include "QSleeper.h"

void QSleeper::sleepSecs(unsigned long secs)
{
   sleep(secs);
}

void QSleeper::sleepMicroSecs(unsigned long usecs)
{
   usleep(usecs);
}

void QSleeper::sleepMilliSecs(unsigned long msecs)
{
   msleep(msecs);
}
