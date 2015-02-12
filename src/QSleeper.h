#ifndef QSLEEPER_H
#define QSLEEPER_H

#include <QThread>

class QSleeper : public QThread
{
   Q_OBJECT
public:

   static void sleepSecs(unsigned long secs);

   static void sleepMicroSecs(unsigned long usecs);

   static void sleepMilliSecs(unsigned long msecs);

};

#endif // QSLEEPER_H
