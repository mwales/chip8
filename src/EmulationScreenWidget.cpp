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
   QPainter p(this);
   p.setPen(Qt::blue);

   int pixelWidth = width() / X_RES;
   int pixelHeight = height() / Y_RES;

   int xOffset = width() % X_RES / 2;
   int yOffset = height() % Y_RES / 2;


   for(int y = 0; y < Y_RES; y++)
   {
      for(int x = 0; x < X_RES; x++)
      {
         if (!thePixels[y][x])
         {
            p.fillRect(pixelWidth * x + xOffset, pixelHeight * y + yOffset, pixelWidth, pixelHeight, Qt::black);
         }
      }
   }


}

void EmulationScreenWidget::forceRepaint()
{
   repaint();
}
