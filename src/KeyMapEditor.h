#ifndef KEYMAPEDITOR_H
#define KEYMAPEDITOR_H

#include <QDialog>
#include <QMap>
#include <QLineEdit>

namespace Ui {
class KeyMapEditor;
}

class KeyMapEditor : public QDialog
{
   Q_OBJECT

public:
   explicit KeyMapEditor(QWidget *parent = 0);

   explicit KeyMapEditor(QMap<char, unsigned char> currentKeyMap, QWidget *parent = 0);

   ~KeyMapEditor();

   char getKeyBinding(unsigned char);

protected:

   void createControlMap();

   QMap<unsigned char, QLineEdit*> theControlMap;

private:
   Ui::KeyMapEditor *ui;
};

#endif // KEYMAPEDITOR_H
