#ifndef LOGINFORM_H
#define LOGINFORM_H

#include "catalog.h"
#include "title.h"
#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "accountinfo.h"


QT_BEGIN_NAMESPACE
namespace Ui { class LoginForm; }
QT_END_NAMESPACE

class LoginForm : public QMainWindow
{
    Q_OBJECT

public:
    LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

private slots:
    void on_sendButton_clicked();
    //void downloadFinished(QNetworkReply *reply);

    void on_regiterButton_clicked();

private:
    Ui::LoginForm *ui;
    QSqlDatabase db;
    Catalog *catalog;
    AccountInfo *accountInfo;
    void createCatalog();
};
#endif // LOGINFORM_H
