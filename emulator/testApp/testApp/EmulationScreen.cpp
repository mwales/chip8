#include <QPainter>
#include <QtDebug>

#include "EmulationScreen.h"

const int X_RES = 64;
const int Y_RES = 32;

EmulationScreen::EmulationScreen(QWidget *parent) :
   QWidget(parent)
{
   for(int y = 0; y < Y_RES; y++)
   {
      QVector<bool> curRowOfPixels;
      for(int x = 0; x < X_RES; x++)
      {
         curRowOfPixels.append(false);
      }
      thePixels.append(curRowOfPixels);
   }
}


void EmulationScreen::paintEvent ( QPaintEvent * event )
{
   QPainter p(this);
   p.setPen(Qt::blue);

   int pixelWidth = width() / X_RES;
   int pixelHeight = height() / Y_RES;

   for(int y = 0; y < Y_RES; y++)
   {
      for(int x = 0; x < X_RES; x++)
      {
         if (thePixels[y][x])
         {
            p.fillRect(pixelWidth * x, pixelHeight * y, pixelWidth, pixelHeight, Qt::black);
         }
      }
   }


}

void EmulationScreen::setPixel(int x, int y, bool val)
{
   thePixels[y][x] = val;
}

void EmulationScreen::keyPressEvent ( QKeyEvent * event )
{
   qDebug() << "Key Pressed" << event->text();
}

void EmulationScreen::keyReleaseEvent ( QKeyEvent * event )
{
   qDebug() << "Key Released" << event->text();
}
