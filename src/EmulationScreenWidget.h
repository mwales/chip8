#ifndef EMULATIONSCREENWIDGET_H
#define EMULATIONSCREENWIDGET_H

#include <QWidget>
#include <QTimer>
#include "EmulationScreen.h"

/**
 * Qt implementation of the Chip-8 display
 *
 * @note Tried to have the forceRepaint only called when the emulator modified the screen, but
 * the inter-thread communication was causing crashes.  Now the repaint is called by a timer at
 * 100Hz by a timer in the GUI thread.
 */
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
