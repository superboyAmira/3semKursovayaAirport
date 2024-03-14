QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# win32: LIBS += libpqdll.lib
# INCLUDEPATH += .
# LIBS += C:/Program Files/PostgreSQL/12/lib/libpq.lib
# LIBS += libpq.dll libiconv-2.dll libintl-9.dll libssl-3-x64.dll libcrypto-3-x64.dll
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminmenu.cpp \
    auth.cpp \
    customermenu.cpp \
    databases.cpp \
    guardmenu.cpp \
    login.cpp \
    main.cpp \
    passmenu.cpp \
    registration.cpp \
    route.cpp \
    staffmenu.cpp \
    ticket.cpp

HEADERS += \
    adminmenu.h \
    airport.h \
    auth.h \
    customermenu.h \
    databases.h \
    guardmenu.h \
    login.h \
    passmenu.h \
    registration.h \
    route.h \
    staffmenu.h \
    ticket.h

FORMS += \
    adminmenu.ui \
    auth.ui \
    customermenu.ui \
    databases.ui \
    guardmenu.ui \
    login.ui \
    passmenu.ui \
    registration.ui \
    route.ui \
    staffmenu.ui \
    ticket.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    resources/airport_700.jpg

RESOURCES += \
    image-samples.qrc \
    image_src.qrc
