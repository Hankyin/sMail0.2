#-------------------------------------------------
#
# Project created by QtCreator 2018-03-17T20:36:23
#
#-------------------------------------------------

QT       += core gui network sql webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sMail
TEMPLATE = app

CONFIG += link_pkgconfig
CONFIG += c++11
PKGCONFIG += xcb xcb-util dtkwidget

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    netProtocol/imap.cpp \
    netProtocol/pop.cpp \
    netProtocol/smtp.cpp \
    subControl/sender/cmdbar.cpp \
    subControl/sender/emojipicker.cpp \
    subControl/sender/itembutton.cpp \
    subControl/sender/lineedit.cpp \
    subControl/sender/maileditor.cpp \
    subControl/viewer/diritem.cpp \
    subControl/viewer/dirlistview.cpp \
    subControl/viewer/dirpage.cpp \
    subControl/viewer/liststack.cpp \
    subControl/viewer/mailitem.cpp \
    subControl/viewer/maillistview.cpp \
    subControl/viewer/mailpage.cpp \
    subControl/viewer/sidebar.cpp \
    subControl/viewer/sidebutton.cpp \
    subControl/viewer/useritem.cpp \
    subControl/viewer/userlistview.cpp \
    subControl/viewer/userpage.cpp \
    utils/flowlayout.cpp \
    utils/mime.cpp \
    utils/translator.cpp \
    accountmanager.cpp \
    database.cpp \
    mailsender.cpp \
    mailviewer.cpp \
    main.cpp \
    smail.cpp \
    subControl/viewer/mailbrowser.cpp


HEADERS += \
    netProtocol/imap.h \
    netProtocol/pop.h \
    netProtocol/smtp.h \
    subControl/sender/cmdbar.h \
    subControl/sender/emojipicker.h \
    subControl/sender/itembutton.h \
    subControl/sender/lineedit.h \
    subControl/sender/maileditor.h \
    subControl/viewer/diritem.h \
    subControl/viewer/dirlistview.h \
    subControl/viewer/dirpage.h \
    subControl/viewer/liststack.h \
    subControl/viewer/mailitem.h \
    subControl/viewer/maillistview.h \
    subControl/viewer/mailpage.h \
    subControl/viewer/sidebar.h \
    subControl/viewer/sidebutton.h \
    subControl/viewer/useritem.h \
    subControl/viewer/userlistview.h \
    subControl/viewer/userpage.h \
    utils/flowlayout.h \
    utils/mime.h \
    utils/translator.h \
    utils/type.h \
    accountmanager.h \
    database.h \
    mailsender.h \
    mailviewer.h \
    smail.h \
    subControl/viewer/mailbrowser.h


RESOURCES += \
    res.qrc

DISTFILES += \
    readme.md
