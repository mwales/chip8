#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <QRunnable>
#include <QMutex>

int playAnnoyingTone(double durationSecs);

/**
 * The QSound class provided by Qt is obsolete I guess, and this seemed like the easiest
 * cross-platform library to create sound with
 */
class AnnoyingSound: public QRunnable
{
public:

   void setDuration(double durationSecs);

   void run();

private:

   double theDuration;

   static QMutex theSingleSoundLock;
};

#endif // AUDIO_PLAYER_H
