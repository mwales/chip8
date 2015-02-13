#ifndef EMULATIONSCREENWIDGET_H
#define EMULATIONSCREENWIDGET_H

#include <QWidget>
#include "EmulationScreen.h"

class EmulationScreenWidget :  public QWidget, public EmulationScreen
{
   Q_OBJECT

public:
   EmulationScreenWidget(QWidget *parent = 0);

   bool drawSprite(unsigned int x, unsigned int y, vector<unsigned char> spriteData);

   void clearScreen();

protected:
   void paintEvent ( QPaintEvent * event );
};

#endif // EMULATIONSCREENWIDGET_H
