#include <QtDebug>
#include "MainWindow.h"
#include "ui_MainWindow.h"

const int X_RES = 64;
const int Y_RES = 32;
const int PIXEL_SIZE = 10;

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{

   ui->setupUi(this);


   ui->theScreen->setPixel(1, 1, true);

}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::keyPressEvent ( QKeyEvent * event )
{
   qDebug() << "MW Key Pressed" << event->text();
}

void MainWindow::keyReleaseEvent ( QKeyEvent * event )
{
   qDebug() << "MW Key Released" << event->text();
}
