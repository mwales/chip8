#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <QRunnable>
#include <QMutex>

int playAnnoyingTone(double durationSecs);

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
