#ifndef CPUVIEWER_H
#define CPUVIEWER_H

#include <QDialog>
#include <QLineEdit>
#include <QVector>
#include <QString>

namespace Ui {
class CpuViewer;
}

class CpuViewer : public QDialog
{
   Q_OBJECT

public:
   explicit CpuViewer(QWidget *parent = 0);
   ~CpuViewer();

   void setRegister(int reg, int val);

   void setInstructionPointer(unsigned int ip);

   void pushStack(unsigned int val);

   void popStack();

   void setDelayTimer(unsigned int val);

   void setSoundTimer(unsigned int val);

   QString numToHex(int number, int numBytes);

private:
   Ui::CpuViewer *ui;

   QVector<QLineEdit*> theRegisters;
};

#endif // CPUVIEWER_H
