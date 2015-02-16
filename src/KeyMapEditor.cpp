#include <QtDebug>
#include "KeyMapEditor.h"
#include "ui_KeyMapEditor.h"

KeyMapEditor::KeyMapEditor(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::KeyMapEditor)
{
   ui->setupUi(this);

   createControlMap();
}

KeyMapEditor::KeyMapEditor(QMap<char, unsigned char> currentKeyMap, QWidget *parent) :
   QDialog(parent),
   ui(new Ui::KeyMapEditor)
{
   ui->setupUi(this);

   createControlMap();

   foreach(unsigned char chip8Key, currentKeyMap)
   {
      char pcKey = currentKeyMap.key(chip8Key);
      QChar pcKeyQChar(pcKey);

      theControlMap[chip8Key]->setText(pcKeyQChar);
   }
}

KeyMapEditor::~KeyMapEditor()
{
   delete ui;
}

void KeyMapEditor::createControlMap()
{
   theControlMap.insert(0x0, ui->lineEdit_0);
   theControlMap.insert(0x1, ui->lineEdit_1);
   theControlMap.insert(0x2, ui->lineEdit_2);
   theControlMap.insert(0x3, ui->lineEdit_3);
   theControlMap.insert(0x4, ui->lineEdit_4);
   theControlMap.insert(0x5, ui->lineEdit_5);
   theControlMap.insert(0x6, ui->lineEdit_6);
   theControlMap.insert(0x7, ui->lineEdit_7);
   theControlMap.insert(0x8, ui->lineEdit_8);
   theControlMap.insert(0x9, ui->lineEdit_9);
   theControlMap.insert(0xa, ui->lineEdit_A);
   theControlMap.insert(0xb, ui->lineEdit_B);
   theControlMap.insert(0xc, ui->lineEdit_C);
   theControlMap.insert(0xd, ui->lineEdit_D);
   theControlMap.insert(0xe, ui->lineEdit_E);
   theControlMap.insert(0xf, ui->lineEdit_F);
}

char KeyMapEditor::getKeyBinding(unsigned char chip8Key)
{
   if (!theControlMap.contains(chip8Key))
   {
      qDebug() << "Invalid parameter" << __PRETTY_FUNCTION__ << "chip8Key" << chip8Key;
      return 0;
   }

   QString textBoxContents = theControlMap[chip8Key]->text();

   if (textBoxContents.length() == 0)
   {
      qDebug() << "User failed to make a selection for" << chip8Key;
      return 0;
   }

   return textBoxContents.at(0).toLatin1();
}

