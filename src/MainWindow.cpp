#include <QtDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QInputDialog>

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "KeyMapEditor.h"

#ifdef OLED_DISPLAY
#include "EmulationScreenOled.h"
#endif

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
   connect(ui->actionSet_Breakpoint, SIGNAL(triggered()),
           this, SLOT(addBreakpoint()));
   connect(ui->actionClear_Breakpoints, SIGNAL(triggered()),
           this, SLOT(clearBreakpoints()));
   connect(ui->actionEdit_Keymap, SIGNAL(triggered()),
           this, SLOT(editKeyMap()));

   connect(ui->actionEnable_Sound, SIGNAL(triggered()),
           this, SLOT(enableSound()));
   enableSound();

   theEmulator.setEmulationScreen(ui->screenWidget);

#ifdef OLED_DISPLAY
   theEmulator.setEmulationScreen(new EmulationScreenOled());
#endif

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

   // Setup speed selection maps, signal maps, and connections
   theSpeedSelectors.insert(100, ui->action100_IPS);
   theSpeedSelectors.insert(200, ui->action200_IPS);
   theSpeedSelectors.insert(500, ui->action500_IPS);
   theSpeedSelectors.insert(1000, ui->action1_KIPS);
   theSpeedSelectors.insert(2000, ui->action2_KIPS);
   updateIps(500);

   theIpsMapper.setMapping(ui->action100_IPS, 100);
   theIpsMapper.setMapping(ui->action200_IPS, 200);
   theIpsMapper.setMapping(ui->action500_IPS, 500);
   theIpsMapper.setMapping(ui->action1_KIPS, 1000);
   theIpsMapper.setMapping(ui->action2_KIPS, 2000);

   connect(ui->action100_IPS, SIGNAL(triggered()),
           &theIpsMapper, SLOT(map()));
   connect(ui->action200_IPS, SIGNAL(triggered()),
           &theIpsMapper, SLOT(map()));
   connect(ui->action500_IPS, SIGNAL(triggered()),
           &theIpsMapper, SLOT(map()));
   connect(ui->action1_KIPS, SIGNAL(triggered()),
           &theIpsMapper, SLOT(map()));
   connect(ui->action2_KIPS, SIGNAL(triggered()),
           &theIpsMapper, SLOT(map()));
   connect(&theIpsMapper, SIGNAL(mapped(int)),
           this, SLOT(updateIps(int)));

}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::keyPressEvent ( QKeyEvent * event )
{
   if (event->text().size() > 0)
   {
      char key = event->text().at(0).toLatin1();

      if (theKeyMap.contains(key))
         theEmulator.keyDown(theKeyMap[key]);
   }

}

void MainWindow::keyReleaseEvent ( QKeyEvent * event )
{
   if (event->text().size() > 0)
   {
      char key = event->text().at(0).toLatin1();
      theEmulator.keyUp(theKeyMap[key]);
   }
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

   theEmulator.stopEmulator();
   theEmulator.resetEmulator();

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
   theEmulator.stopEmulator();
   theEmulator.resetEmulator();
   updateCpuViewer();
}

void MainWindow::updateCpuViewer()
{
   for(unsigned char i = 0; i < 16; i++)
      theCpuDialog.setRegister(i, theEmulator.getRegister(i));

   theCpuDialog.setInstructionPointer(theEmulator.getIP());
   theCpuDialog.setIndexRegister(theEmulator.getIndexRegister());
   theCpuDialog.setStack(theEmulator.getStack());
   theCpuDialog.setDelayTimer(theEmulator.getDelayTimer());
}

void MainWindow::addBreakpoint()
{
   if (theEmulator.isRunning())
   {
      QMessageBox::warning(this, "Processor Running", "Stop the emulator before configuring breakpoints");
      return;
   }

   QString breakpoint = QInputDialog::getText(this, "Enter breakpoint address", "0x");
   bool success;
   unsigned int addr = breakpoint.toUInt(&success, 16);

   if (success)
   {
      theEmulator.setBreakpoint(addr);
   }
   else
   {
      QMessageBox::warning(this, "Invalid Address", "Invalid address specified");
   }
}

void MainWindow::clearBreakpoints()
{
   theEmulator.clearBreakpoints();
}

void MainWindow::updateIps(int ips)
{
   if (theSpeedSelectors.contains(ips))
   {
      foreach(int possibleSpeed, theSpeedSelectors.keys())
      {
         if (ips == possibleSpeed)
         {
            theSpeedSelectors[possibleSpeed]->setChecked(true);
         }
         else
         {
            theSpeedSelectors[possibleSpeed]->setChecked(false);
         }
      }

      theEmulator.setInstPerSecond(ips);
   }
}

void MainWindow::enableSound()
{
   theEmulator.enableSound(ui->actionEnable_Sound->isChecked());
}

void MainWindow::editKeyMap()
{
   KeyMapEditor kme(theKeyMap, this);
   if(kme.exec())
   {
      // user clicked ok, update key map

      // validate all the user selections before making changes
      for(int i = 0; i <= 0xf; i++)
      {
         if (kme.getKeyBinding(i) == 0)
         {
            qDebug() << "New key map rejected due to error";
            return;
         }
      }

      theKeyMap.clear();

      for(int i = 0; i <= 0xf; i++)
      {
         theKeyMap.insert(kme.getKeyBinding(i), i);
      }
   }

   qDebug() << "Key Map updated";
}
