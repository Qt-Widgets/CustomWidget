TEMPLATE=lib
CONFIG += dll debug_and_release warn_on
QT  += charts widgets

DEFINES += XW_CUSTOMWIDGET_EXPORTS
QMAKE_CXXFLAGS += /MP /utf-8 /wd"4828"

SOURCES += *.cpp
HEADERS += *.h

INCLUDEPATH += ../third_party/opencv/include

RESOURCES += CustomWidget.qrc

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO  
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO  


CONFIG(debug,debug|release){
    TARGET = CustomWidgetD
    DESTDIR = ../runtimed 
}

CONFIG(release,debug|release){
    TARGET = CustomWidget
    DESTDIR = ../runtime
}