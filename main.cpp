#include "lb1_form.h"
#include "loginform.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "AnDaBaWidgets_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    LoginForm w;
    w.show();
    LB1_form form;
    form.show();


//    qDebug() << QSslSocket::sslLibraryBuildVersionString();
//    qDebug() << QSslSocket::sslLibraryVersionString();
//    qDebug() << QSslSocket::supportsSsl();

    return a.exec();
}
