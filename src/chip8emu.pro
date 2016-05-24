#-------------------------------------------------
#
# Project created by QtCreator 2015-02-10T22:26:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chip8emu
TEMPLATE = app

# You can pass this option (and OLED_DISPLAY) from the command line, like the following:
# qmake CONFIG+=NO_SOUND CONFIG+= OLED_DISPLAY

CONFIG(NO_SOUND) {
   DEFINES+=NO_SOUND
}
else {
   LIBS += -lao
}

#CONFIG+= OLED_DISPLAY

CONFIG(OLED_DISPLAY) {
   DEFINES+=OLED_DISPLAY
   LIBS += -L$$_PRO_FILE_PWD_/Adafruit_SSD1306/build -lSSD1306 \
           -L$$_PRO_FILE_PWD_/Adafruit_SSD1306/build/Adafruit-GFX-Library -lAdafruit_GFX \
           -L$$_PRO_FILE_PWD_/Adafruit_SSD1306/build/ArduinoForLinux -larduino
   INCLUDEPATH += Adafruit_SSD1306 \
                  Adafruit_SSD1306/Adafruit-GFX-Library
   QMAKE_CXXFLAGS += --std=c++11
   SOURCES += EmulationScreenOled.cpp 
   HEADERS += EmulationScreenOled.h
}


SOURCES += chip8emu.cpp\
        MainWindow.cpp \
    EmulationScreen.cpp \
    Emulator.cpp \
    InstDecoder.cpp \
    QSleeper.cpp \
    CpuViewer.cpp \
    EmulationScreenWidget.cpp \
    audio_player.cpp \
    KeyMapEditor.cpp

HEADERS  += MainWindow.h \
    EmulationScreen.h \
    Emulator.h \
    InstDecoder.h \
    QSleeper.h \
    CpuViewer.h \
    EmulationScreenWidget.h \
    audio_player.h \
    KeyMapEditor.h

FORMS    += MainWindow.ui \
    CpuViewer.ui \
    KeyMapEditor.ui

RESOURCES +=
