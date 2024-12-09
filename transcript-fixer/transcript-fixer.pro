QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia opengl printsupport

CONFIG += c++17

QMAKE_CXXFLAGS += -Wall -Wextra

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mediacontrol.cpp \
    src/transcriptionmanager.cpp \
    src/utils.cpp \
    src/qcustomplot.cpp \
    src/waveform.cpp

HEADERS += \
    include/mainwindow.h \
    include/mediacontrol.h \
    include/params.h \
    include/transcriptionmanager.h \
    include/utils.h \
    include/qcustomplot.h \
    include/waveform.h

FORMS += \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Enable parallel compilation (optional)
QMAKE_PARALLEL_COMPILE = -j8

RESOURCES += \
    ./resources.qrc
