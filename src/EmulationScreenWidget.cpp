#include <QPainter>

#include "EmulationScreenWidget.h"

EmulationScreenWidget::EmulationScreenWidget(QWidget *parent):
   QWidget(parent)
{
   theRepaintTimer.setInterval(10); // 100 Hz
   connect(&theRepaintTimer, SIGNAL(timeout()),
           this, SLOT(forceRepaint()));

   theRepaintTimer.start();
}

void EmulationScreenWidget::paintEvent ( QPaintEvent * event )
{
   Q_UNUSED(event)

   QPainter p(this);
   p.setPen(Qt::blue);

   int xRes = theXRes;
   int yRes = theYRes;

   int pixelWidth = width() / xRes;
   int pixelHeight = height() / yRes;

   int xOffset = width() % xRes / 2;
   int yOffset = height() % yRes / 2;

   thePixelsLock.lockForRead();
   for(int y = 0; y < yRes; y++)
   {
      for(int x = 0; x < xRes; x++)
      {
         if (!thePixels[y][x])
         {
            p.fillRect(pixelWidth * x + xOffset, pixelHeight * y + yOffset, pixelWidth, pixelHeight, Qt::black);
         }
      }
   }
   thePixelsLock.unlock();

}

void EmulationScreenWidget::forceRepaint()
{
   repaint();
}
