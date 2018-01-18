#-------------------------------------------------
#
# Project created by QtCreator 2017-11-21T11:21:28
#
#-------------------------------------------------

QT       += core gui multimedia

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
HEADERS += \
	Client/ConstantBuilding.h \
	Client/GameManagementService.h \
	Client/GraphicService.h \
	Client/GuiView.h \
	Client/LoadSaveService.h \
	Client/MainWidget.h \
	Client/MapTile.h \
        Client/MapView.h \
	Client/PickerElement.h \
	Client/TickService.h \
	Client/MapTile.h \
	Client/Building.h \
	Client/TopView.h \
	Client/BuildingManagementService.h \
        Client/Services.h \
	Client/AudioService.h \
        Client/ClickableLabel.h \
        general/RandomService.h \
        general/PerlinNoise.h \
        general/OurMath.h
	
SOURCES += \
	Client/ConstantBuilding.cpp \
	Client/GameManagementService.cpp \
	Client/GraphicService.cpp \
	Client/GuiView.cpp \
	Client/LoadSaveService.cpp \
	Client/main.cpp \
	Client/MainWidget.cpp \
	Client/MapTile.cpp \
        Client/MapView.cpp \
	Client/PickerElement.cpp \
	Client/TickService.cpp \
	Client/Building.cpp \
	Client/TopView.cpp \
        Client/BuildingManagementService.cpp \
	Client/AudioService.cpp \
        Client/ClickableLabel.cpp \
        general/RandomService.cpp \
        general/PerlinNoise.cpp \
        general/OurMath.cpp

RESOURCES += \
	Client/images.qrc \
	Client/resources.qrc
