#include "CpuViewer.h"
#include "ui_CpuViewer.h"

CpuViewer::CpuViewer(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::CpuViewer)
{
   ui->setupUi(this);

   theRegisters.append(ui->lineEdit);
   theRegisters.append(ui->lineEdit_2);
   theRegisters.append(ui->lineEdit_3);
   theRegisters.append(ui->lineEdit_4);
   theRegisters.append(ui->lineEdit_5);
   theRegisters.append(ui->lineEdit_6);
   theRegisters.append(ui->lineEdit_7);
   theRegisters.append(ui->lineEdit_8);
   theRegisters.append(ui->lineEdit_9);
   theRegisters.append(ui->lineEdit_10);
   theRegisters.append(ui->lineEdit_11);
   theRegisters.append(ui->lineEdit_12);
   theRegisters.append(ui->lineEdit_13);
   theRegisters.append(ui->lineEdit_14);
   theRegisters.append(ui->lineEdit_15);
   theRegisters.append(ui->lineEdit_16);

}

void CpuViewer::setRegister(int reg, int val)
{
   theRegisters[reg]->setText(numToHex(val, 1));
}

void CpuViewer::setInstructionPointer(unsigned int ip)
{
   ui->lineEdit_ip->setText(numToHex(ip, 2));
}

void CpuViewer::pushStack(unsigned int val)
{
   ui->callStack->addItem(numToHex(val, 2));
}

void CpuViewer::popStack()
{
   ui->callStack->takeItem(ui->callStack->count() - 1);
}

void CpuViewer::setDelayTimer(unsigned int val)
{
   ui->lineEdit_dt->setText(numToHex(val, 1));
}

void CpuViewer::setSoundTimer(unsigned int val)
{
   ui->lineEdit_st->setText(numToHex(val, 1));
}

QString CpuViewer::numToHex(int number, int numBytes)
{
   QString retVal = QString::number(number, 16);

   int numFrontPadding = numBytes * 2 - retVal.length();

   if (numFrontPadding > 0)
   {
      retVal.insert(0, QString("0").repeated(numFrontPadding));
   }

   return retVal;
}

CpuViewer::~CpuViewer()
{
   delete ui;
}
