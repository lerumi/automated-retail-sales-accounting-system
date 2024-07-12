QT       += core gui
QT += core gui sql
QT += printsupport
QT += core
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(src/QZXing.pri)

CONFIG += c++17
#CONFIG += staticlib
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DataBaseManager.cpp \
    authoriz.cpp \
    customer_window.cpp \
    customerstab.cpp \
    main.cpp \
    mainwindow.cpp \
    orderstab.cpp \
    productstab.cpp \
    providerstab.cpp \
    shipmentstab.cpp \
    shop.cpp \
    sotrudnik.cpp \
    storagestab.cpp \
    workertab.cpp

HEADERS += \
    DataBaseManager.h \
    Report.h \
    authoriz.h \
    customer_window.h \
    customerstab.h \
    mainwindow.h \
    orderstab.h \
    productstab.h \
    providerstab.h \
    shipmentstab.h \
    shop.h \
    sotrudnik.h \
    storagestab.h \
    workertab.h

FORMS += \
    authoriz.ui \
    customer_window.ui \
    customerstab.ui \
    mainwindow.ui \
    orderstab.ui \
    productstab.ui \
    providerstab.ui \
    shipmentstab.ui \
    shop.ui \
    sotrudnik.ui \
    storagestab.ui \
    workertab.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
