#ifndef EMULATIONSCREEN_H
#define EMULATIONSCREEN_H

#include <QWidget>
#include <QVector>
#include <QKeyEvent>




class EmulationScreen : public QWidget
{
   Q_OBJECT
public:
   explicit EmulationScreen(QWidget *parent = 0);

   void setPixel(int x, int y, bool val);



protected:
   void paintEvent ( QPaintEvent * event );

   void keyPressEvent ( QKeyEvent * event );

   void keyReleaseEvent ( QKeyEvent * event );

signals:

public slots:

private:

   QVector< QVector< bool > > thePixels;

};

#endif // EMULATIONSCREEN_H
