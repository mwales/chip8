#include <QPainter>

#include "EmulationScreenWidget.h"

EmulationScreenWidget::EmulationScreenWidget(QWidget *parent):
   QWidget(parent)
{

}

bool EmulationScreenWidget::drawSprite(unsigned int x, unsigned int y, vector<unsigned char> spriteData)
{
   EmulationScreen::drawSprite(x,y,spriteData);
   repaint();
}

void EmulationScreenWidget::clearScreen()
{
   EmulationScreen::clearScreen();
   repaint();
}

void EmulationScreenWidget::paintEvent ( QPaintEvent * event )
{
   QPainter p(this);
   p.setPen(Qt::blue);

   int pixelWidth = width() / X_RES;
   int pixelHeight = height() / Y_RES;

   int xOffset = 0; //width() % X_RES / 2;
   int yOffset = 0; //height() % Y_RES / 2;


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
