#ifndef EMULATIONSCREENOLED_H
#define EMULATIONSCREENOLED_H

#include <QObject>
#include <QTimer>
#include "EmulationScreen.h"
#include "Adafruit_SSD1306/Adafruit_SSD1306.h"



class EmulationScreenOled : public QObject, public EmulationScreen
{

   Q_OBJECT

public:
   EmulationScreenOled(QObject* parent = 0);

private slots:

   void forceRepaint();

protected:

   QTimer theRepaintTimer;

   Adafruit::Adafruit_SSD1306 theOledScreen;
};

#endif // EMULATIONSCREENOLED_H



