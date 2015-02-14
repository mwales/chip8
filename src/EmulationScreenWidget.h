#ifndef EMULATIONSCREENWIDGET_H
#define EMULATIONSCREENWIDGET_H

#include <QWidget>
#include <QTimer>
#include "EmulationScreen.h"

class EmulationScreenWidget :  public QWidget, public EmulationScreen
{
   Q_OBJECT

public:
   EmulationScreenWidget(QWidget *parent = 0);

private slots:

   void forceRepaint();

protected:
   void paintEvent ( QPaintEvent * event );

   QTimer theRepaintTimer;
};

#endif // EMULATIONSCREENWIDGET_H
