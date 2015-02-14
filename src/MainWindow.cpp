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
   //connect(ui->screenWidget->goB)
   connect(&theCpuDialog, SIGNAL(goPressed()),
           this, SLOT(runEmulator()));
   connect(&theCpuDialog, SIGNAL(pausePressed()),
           this, SLOT(pauseEmulator()));
   connect(&theCpuDialog, SIGNAL(resetPressed()),
           this, SLOT(resetEmulator()));
   connect(&theCpuDialog, SIGNAL(stepPressed()),
           this, SLOT(stepEmulator()));


   ui->screenWidget->setPixel(1, 1, true);

   theEmulator.setEmulationScreen(ui->screenWidget);

   // Not sure how standardized Chip-8 emulators are, but I'm using JChip8 as reference so I'll map my keys
   // the same as that emulator.

   //   Original Chip8            JChip8

   //      1  2  3  C            1  2  3  4
   //      4  5  6  D    --->    Q  W  E  R
   //      7  8  9  E            A  S  D  F
   //      A  0  B  F            Z  X  C  V

   theKeyMap.insert('x', 0x0);
   theKeyMap.insert('1', 0x1);
   theKeyMap.insert('2', 0x2);
   theKeyMap.insert('3', 0x3);
   theKeyMap.insert('q', 0x4);
   theKeyMap.insert('w', 0x5);
   theKeyMap.insert('e', 0x6);
   theKeyMap.insert('a', 0x7);
   theKeyMap.insert('s', 0x8);
   theKeyMap.insert('d', 0x9);
   theKeyMap.insert('z', 0xa);
   theKeyMap.insert('c', 0xb);
   theKeyMap.insert('4', 0xc);
   theKeyMap.insert('r', 0xd);
   theKeyMap.insert('f', 0xe);
   theKeyMap.insert('v', 0xf);
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::keyPressEvent ( QKeyEvent * event )
{
   qDebug() << "MW Key Pressed" << event->text();
   char key = event->text().at(0).toAscii();
   theEmulator.keyDown(theKeyMap[key]);

}

void MainWindow::keyReleaseEvent ( QKeyEvent * event )
{
   qDebug() << "MW Key Released" << event->text();
   char key = event->text().at(0).toAscii();
   theEmulator.keyUp(theKeyMap[key]);
}

void MainWindow::closeEvent(QCloseEvent * event)
{
   Q_UNUSED(event)

   pauseEmulator();
}

void MainWindow::loadRom()
{
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

   qDebug() << "Loading ROM" << dlg.selectedFiles().first();

   theEmulator.clearRomData();

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

   theEmulator.resetEmulator();

   if (ui->actionStart_on_Load->isChecked())
   {
      // Start emulator
      qDebug() << "Starting emulator on load";
      runEmulator();
   }
   else
   {
      qDebug() << "Emulator paused on load, run via CPU Viewer";
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

void MainWindow::pauseEmulator()
{
   qDebug() << "Pause Emulator";
   theEmulator.stopEmulator();
   updateCpuViewer();
}

void MainWindow::stepEmulator()
{
   theEmulator.executeInstruction();
   updateCpuViewer();
}

void MainWindow::runEmulator()
{
   qDebug() << "Run Emulator!";
   theEmulator.start();
}

void MainWindow::resetEmulator()
{
   theEmulator.resetEmulator();
}

void MainWindow::updateCpuViewer()
{
   for(unsigned char i = 0; i < 16; i++)
      theCpuDialog.setRegister(i, theEmulator.getRegister(i));

   theCpuDialog.setInstructionPointer(theEmulator.getIP());
   theCpuDialog.setIndexRegister(theEmulator.getIndexRegister());
   theCpuDialog.setStack(theEmulator.getStack());
   //theCpuDialog.setDelayTimer(theEmulator.get);
}
