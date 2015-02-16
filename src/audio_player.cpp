// Shameless theft because I don't care about audio a whole lot
// and who knew linux audio was such a cluster f...

/*
 *
 * ao_example.c
 *
 *     Written by Stan Seibert - July 2001
 *
 * Legal Terms:
 *
 *     This source file is released into the public domain.  It is
 *     distributed without any warranty; without even the implied
 *     warranty * of merchantability or fitness for a particular
 *     purpose.
 *
 * Function:
 *
 *     This program opens the default driver and plays a 440 Hz tone for
 *     one second.
 *
 * Compilation command line (for Linux systems):
 *
 *     gcc -o ao_example ao_example.c -lao -ldl -lm
 *
 */

#include <QtDebug>
#include <stdio.h>
#include <string.h>
#include <ao/ao.h>
#include <math.h>
#include "audio_player.h"

#define BUF_SIZE 4096

QMutex AnnoyingSound::theSingleSoundLock;

int playAnnoyingTone(double numSecs)
{
   ao_device *device;
   ao_sample_format format;
   int default_driver;
   char *buffer;
   int buf_size;
   int sample;
   float freq = 440.0;
   int i;

   /* -- Initialize -- */

   ao_initialize();

   /* -- Setup for default driver -- */

   default_driver = ao_default_driver_id();

        memset(&format, 0, sizeof(format));
   format.bits = 16;
   format.channels = 2;
   format.rate = 44100;
   format.byte_format = AO_FMT_LITTLE;

   /* -- Open driver -- */
   device = ao_open_live(default_driver, &format, NULL /* no options */);
   if (device == NULL) {
      fprintf(stderr, "Error opening device.\n");
      return 1;
   }

   /* -- Play some stuff -- */
   buf_size = format.bits/8 * format.channels * (int) (format.rate * numSecs);
   buffer = new char[buf_size]; //calloc(buf_size, sizeof(char));

   for (i = 0; i < (int) (format.rate * numSecs); i++) {
      sample = (int)(1.25 * 32768.0 *
         sin(2 * M_PI * freq * ((float) i/format.rate)));

      /* Put the same stuff in left and right channel */
      buffer[4*i] = buffer[4*i+2] = sample & 0xff;
      buffer[4*i+1] = buffer[4*i+3] = (sample >> 8) & 0xff;
   }
   ao_play(device, buffer, buf_size);

   /* -- Close and shutdown -- */
   ao_close(device);

   ao_shutdown();

   delete[] buffer;

  return (0);
}

void AnnoyingSound::setDuration(double durationSecs)
{
   theDuration = durationSecs;
}

void AnnoyingSound::run()
{
   if (theSingleSoundLock.tryLock() == false)
   {
      // Sound already playing, just exit
      qDebug() << "Aborting sound instruction, sound already playing";
   }
   else
   {
      playAnnoyingTone(theDuration);
      theSingleSoundLock.unlock();
   }
}


