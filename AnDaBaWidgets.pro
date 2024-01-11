QT       += core gui sql network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    accountinfo.cpp \
    addepisodeform.cpp \
    addrowform.cpp \
    catalog.cpp \
    episode.cpp \
    episodeswindow.cpp \
    imgdownloader.cpp \
    lb1_editform.cpp \
    lb1_form.cpp \
    lb2_form.cpp \
    main.cpp \
    loginform.cpp \
    profile.cpp \
    someclass.cpp \
    sort.cpp \
    src/emailaddress.cpp \
    src/mimeattachment.cpp \
    src/mimecontentformatter.cpp \
    src/mimefile.cpp \
    src/mimehtml.cpp \
    src/mimeinlinefile.cpp \
    src/mimemessage.cpp \
    src/mimemultipart.cpp \
    src/mimepart.cpp \
    src/mimetext.cpp \
    src/quotedprintable.cpp \
    src/smtpclient.cpp \
    title.cpp \
    titleinfo.cpp \
    titlewidget.cpp

#LIBS += -LC:/Qt/Tools/OpenSSLv3/Win_x64/lib/libssl.lib
#LIBS += -LC:/Qt/Tools/OpenSSLv3/Win_x64/lib/libcrypto.lib

HEADERS += \
    accountinfo.h \
    addepisodeform.h \
    addrowform.h \
    catalog.h \
    episode.h \
    episodeswindow.h \
    imgdownloader.h \
    lb1_editform.h \
    lb1_form.h \
    lb2_form.h \
    loginform.h \
    profile.h \
    someclass.h \
    sort.h \
    src/SmtpMime \
    src/emailaddress.h \
    src/mimeattachment.h \
    src/mimecontentformatter.h \
    src/mimefile.h \
    src/mimehtml.h \
    src/mimeinlinefile.h \
    src/mimemessage.h \
    src/mimemultipart.h \
    src/mimepart.h \
    src/mimetext.h \
    src/quotedprintable.h \
    src/smtpclient.h \
    src/smtpexports.h \
    title.h \
    titleinfo.h \
    titlewidget.h

FORMS += \
    addepisodeform.ui \
    addrowform.ui \
    catalog.ui \
    episode.ui \
    episodeswindow.ui \
    lb1_editform.ui \
    lb1_form.ui \
    lb2_form.ui \
    loginform.ui \
    profile.ui \
    sort.ui \
    title.ui \
    titleinfo.ui

TRANSLATIONS += \
    AnDaBaWidgets_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
