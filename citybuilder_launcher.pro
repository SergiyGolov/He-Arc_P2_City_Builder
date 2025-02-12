#-------------------------------------------------
#
# Project created by QtCreator 2017-12-10T17:20:23
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = citybuilder_launcher
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
Launcher/main_launcher.cpp \
Launcher/Launcher.cpp \
general/RandomService.cpp \
general/OurMath.cpp \
general/PerlinNoise.cpp

HEADERS += \
Launcher/Launcher.h \
Launcher/LauncherService.h \
general/RandomService.h \
general/OurMath.h \
general/PerlinNoise.h

RESOURCES += \
Launcher/ressources/resources.qrc
