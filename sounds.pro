#-------------------------------------------------
#
# Project created by QtCreator 2011-06-03T11:21:59
#
#-------------------------------------------------

QT       += core gui

TARGET = sounds
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    WavDecoder.cpp \
    FilterInterface.cpp \
    Complex.cpp \
    AutoCorrelationFilter.cpp \
    AutoCorrelationSetupDialog.cpp \
    DisplayWindow.cpp \
    SoundWindow.cpp \
    FFT.cpp \
    CombFilter.cpp

HEADERS  += MainWindow.h \
    WavDecoder.h \
    FilterInterface.h \
    FilterData.h \
    ComplexArray.h \
    Complex.h \
    AutoCorrelationFilter.h \
    AutoCorrelationSetupDialog.h \
    DisplayWindow.h \
    SoundWindow.h \
    FFT.h \
    CombFilter.h

FORMS    += MainWindow.ui

QMAKE_CXXFLAGS += -fopenmp
LIBS += -lgomp
