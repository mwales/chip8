#ifndef CPUVIEWER_H
#define CPUVIEWER_H

#include <QDialog>
#include <QLineEdit>
#include <QVector>
#include <QString>
#include <QStack>

namespace Ui {
class CpuViewer;
}

/**
 * Displays internal CPU / interpreter state information when the emulator is paused
 */
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

   void setStack(QStack<unsigned int> stack);

   void setDelayTimer(unsigned int val);

   void setIndexRegister(unsigned int val);

   QString numToHex(int number, int numBytes);

signals:

   void goPressed();

   void stepPressed();

   void resetPressed();

   void pausePressed();


private:
   Ui::CpuViewer *ui;

   /**
    * Allows me to access all the register QLineEdits via an array / index
    */
   QVector<QLineEdit*> theRegisters;
};

#endif // CPUVIEWER_H
