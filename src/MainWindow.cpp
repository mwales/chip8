#include <QtDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

#include "MainWindow.h"
#include "ui_MainWindow.h"

const int X_RES = 64;
const int Y_RES = 32;
const int PIXEL_SIZE = 10;

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   theCpuDialog(this),
   ui(new Ui::MainWindow)
{

   ui->setupUi(this);

   connect(ui->actionLoadRom, SIGNAL(triggered()),
           this, SLOT(loadRom()));
   connect(ui->actionAboutEmulator, SIGNAL(triggered()),
           this, SLOT(aboutApplication()));
   connect(ui->actionAboutQt, SIGNAL(triggered()),
           this, SLOT(aboutQt()));
   connect(ui->actionCPU_Viewer, SIGNAL(triggered()),
           &theCpuDialog, SLOT(show()));


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

void MainWindow::loadRom()
{
   qDebug() << "Load Rom";
   QFileDialog dlg(this, "Choose ROM");
   QStringList loadFilters;
   loadFilters << "Chip-8 ROM (*.rom *.ROM)";
   loadFilters << "Any File (* *.*)";

   dlg.setNameFilters(loadFilters);
   dlg.setFileMode(QFileDialog::ExistingFile);
   dlg.setModal(true);


   dlg.exec();

   if (dlg.selectedFiles().empty())
      return;

   unsigned char instruction;

   FILE* romFile = fopen(dlg.selectedFiles().first().toStdString().c_str(), "r");

   while(true)
   {
      instruction = fgetc(romFile);

      if (feof(romFile))
      {

         break;
      }

      // Decode instruction
      theEmulator.loadRomData(instruction);

   }


}

void MainWindow::aboutApplication()
{
   QMessageBox::about(this, "About Emulator", "Chip-8 Emulator\nWritten by Michael Wales\nmwales3@gmail.com\n");
}

void MainWindow::aboutQt()
{
   QMessageBox::aboutQt(this, "About Qt");
}
