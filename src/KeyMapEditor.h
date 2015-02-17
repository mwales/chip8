#ifndef KEYMAPEDITOR_H
#define KEYMAPEDITOR_H

#include <QDialog>
#include <QMap>
#include <QLineEdit>

namespace Ui {
class KeyMapEditor;
}

/**
 * Dialog used to change keyboard key bindings (tank game has inverted controls that need to be
 * reconfigured).
 */
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
