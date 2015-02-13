#-------------------------------------------------
#
# Project created by QtCreator 2015-02-10T22:26:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chip8emu
TEMPLATE = app


SOURCES += chip8emu.cpp\
        MainWindow.cpp \
    EmulationScreen.cpp \
    Emulator.cpp \
    InstDecoder.cpp \
    QSleeper.cpp \
    CpuViewer.cpp \
    EmulationScreenWidget.cpp

HEADERS  += MainWindow.h \
    EmulationScreen.h \
    Emulator.h \
    InstDecoder.h \
    QSleeper.h \
    CpuViewer.h \
    EmulationScreenWidget.h

FORMS    += MainWindow.ui \
    CpuViewer.ui
