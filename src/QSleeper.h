#ifndef QSLEEPER_H
#define QSLEEPER_H

#include <QThread>

/**
 * @brief Wrapper to allow access to the QThread protected sleep functions.  These functions were
 * later made public in Qt5.
 */
class QSleeper : public QThread
{
   Q_OBJECT
public:

   static void sleepSecs(unsigned long secs);

   static void sleepMicroSecs(unsigned long usecs);

   static void sleepMilliSecs(unsigned long msecs);

};

#endif // QSLEEPER_H
