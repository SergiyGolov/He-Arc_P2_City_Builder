#-------------------------------------------------
#
# Project created by QtCreator 2017-11-21T11:21:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = citybuilder
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


# Input
HEADERS += ConstantBuilding.h \
           GameManagementService.h \
           GameService.h \
           GraphicService.h \
           GuiView.h \
           LauncherService.h \
           LoadSaveService.h \
           MainWidget.h \
           MapTile.h \
           MapView.h \
           OursMaths.h \
           PickerElement.h \
           TickService.h \
           GameService.h \
           MapTile.h \
           Building.cpp \
           EffectBuilding.cpp \
           HouseBuilding.cpp \
    launcherservice.h
SOURCES += ConstantBuilding.cpp \
           GameManagementService.cpp \
           GraphicService.cpp \
           GuiView.cpp \
           LoadSaveService.cpp \
           main.cpp \
           MainWidget.cpp \
           MapTile.cpp \
           MapView.cpp \
           OursMaths.cpp \
           PickerElement.cpp \
           TickService.cpp \
           Building.h \
           EffectBuilding.h \
           HouseBuilding.h \
    LauncherService.cpp




FORMS +=
