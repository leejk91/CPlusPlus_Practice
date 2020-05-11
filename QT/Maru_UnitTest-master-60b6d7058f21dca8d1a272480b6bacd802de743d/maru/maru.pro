#-------------------------------------------------
#
# Project created by QtCreator 2020-03-30T13:56:45
#
#-------------------------------------------------

QT       += core gui serialport

#static {
#    QTPLUGIN +=qtvirtualkeyboardplugin
#    QT +=svg
#}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets concurrent multimedia

TARGET = maru
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

INCLUDEPATH += inc \
               device_library \
               appdev

LIBS += -lasound

QMAKE_CXXFLAGS_WARN_OFF += -Wunused-parameter -Wunused-variable -Wformat-nonliteral
#QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter

SOURCES += \
        main.cpp \
        maindialog.cpp \
    repairtestkeybuttondialog.cpp \
    systemutil.cpp \
    repairtestlcddialog.cpp \
    repairtestsounddialog.cpp \
    SoundUtil.cpp \
    Settings.cpp \
    repairtestnetworkdialog.cpp \
    tof.c \
    repairtesttofdialog.cpp \
    marudoorlock.cpp \
    repairtestdoorlockdialog.cpp \
    Checksum.cpp \
    repairtestimageshowdialog.cpp \
    maruserial.cpp \
    repairtestserialdialog.cpp \
    device_library/lib_koces.c \
    device_library/lib_util.c \
    repairtestktcdialog.cpp \
    appdev/app_config.c \
    device_library/lib_ubcvan.c \
    testdialog.cpp

HEADERS += \
        maindialog.h \
    repairtestkeybuttondialog.h \
    systemutil.h \
    repairtestlcddialog.h \
    repairtestsounddialog.h \
    SoundUtil.h \
    Settings.h \
    repairtestnetworkdialog.h \
    tof.h \
    repairtesttofdialog.h \
    marudoorlock.h \
    repairtestdoorlockdialog.h \
    Checksum.h \
    repairtestimageshowdialog.h \
    maruserial.h \
    repairtestserialdialog.h \
    device_library/EMVtypedef.h \
    device_library/lib_koces.h \
    device_library/lib_util.h \
    device_library/typedef.h \
    inc/auth_koces.h \ 
    repairtestktcdialog.h \
    appdev/app_config.h \
    device_library/lib_ubcvan.h \
    testdialog.h

FORMS += \
        maindialog.ui \
    repairtestkeybuttondialog.ui \
    repairtestlcddialog.ui \
    repairtestsounddialog.ui \
    repairtestnetworkdialog.ui \
    repairtesttofdialog.ui \
    repairtestdoorlockdialog.ui \
    repairtestimageshowdialog.ui \
    repairtestserialdialog.ui \
    repairtestktcdialog.ui \
    testdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#message($$QT_ARCH)

contains(QT_ARCH, arm) {
    #message("arm")
    unix:!macx: LIBS += -L$$PWD/lib/bingo/ -lDKoces_v1.0.0.7 -lDCrypt_v3.0.10.5 -lDCli_v3.0.10.5
    INCLUDEPATH += $$PWD/lib/bingo
    DEPENDPATH += $$PWD/lib/bingo
}else {
    #message("ubuntu")
    unix:!macx: LIBS += -L$$PWD/lib/pc/ -lDKoces_v1.0.0.7 -lDCrypt_v3.0.10.5 -lDCli_v3.0.10.5
    INCLUDEPATH += $$PWD/lib/pc
    DEPENDPATH += $$PWD/lib/pc
}

#unix:!macx: LIBS += -L$$PWD/lib/pc/ -lDKoces_v1.0.0.7 -lDCrypt_v3.0.10.5 -lDCli_v3.0.10.5
#INCLUDEPATH += $$PWD/lib/pc
#DEPENDPATH += $$PWD/lib/pc

#unix:!macx: LIBS += -L$$PWD/lib/bingo/ -lDKoces_v1.0.0.7 -lDCrypt_v3.0.10.5 -lDCli_v3.0.10.5
#INCLUDEPATH += $$PWD/lib/bingo
#DEPENDPATH += $$PWD/lib/bingo
