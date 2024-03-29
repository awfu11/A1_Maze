QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Controller/controller.cpp \
    Model/Cave/cave.cpp \
    Model/Maze/maze.cpp \
    Model/model.cpp \
    main.cpp \
    View/caveview.cpp \
    View/mazeview.cpp \
    View/view.cpp

HEADERS += \
    Controller/controller.h \
    Model/model.h \
    Model/Cave/cave.h \
    Model/Maze/maze.h \
    View/view.h

FORMS += \
    View/view.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
