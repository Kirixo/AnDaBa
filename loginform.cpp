#include "loginform.h"
#include "title.h"
#include "ui_loginform.h"

LoginForm::LoginForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("andaba");
    if(db.open()){
        qDebug() << "!!!!andaba opened";
    } else {
        qDebug() << "db error";
    }
}

LoginForm::~LoginForm()
{
    delete ui;
    delete catalog;
}


void LoginForm::on_sendButton_clicked()
{
    QString login = ui->loginInput->text();
    QString password = ui->passwordInput->text();

    QString queryString = "SELECT * FROM users WHERE login = :login AND password = :password";
    QSqlQuery query;
    query.prepare(queryString);

    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (query.exec()) {
        if (query.next()) {
            qDebug() << "Login successful\n";
            accountInfo = AccountInfo::GetInstance(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(),  NULL);
            createCatalog();
        } else {
            qDebug() << "Login failed\n";

        }
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

}

void LoginForm::createCatalog()
{
    catalog = new Catalog();
    catalog->show();
    this->close();
}


//void LoginForm::downloadFinished(QNetworkReply *reply)
//{
//    QPixmap pm;
//    pm.loadFromData(reply->readAll());
//    ui->label->setPixmap(pm);
//}

void LoginForm::on_regiterButton_clicked()
{
    QString login = ui->loginInput_reg->text();
    QString password = ui->passwordInput_reg->text();
    QString password2 = ui->passwordInput_reg2->text();

    if(password != password2) {
        qDebug() << "Passwords not equal";
        return;
    }

    QString email = ui->emailInput_reg->text();
    QDate birthday = ui->dateEdit->date();


    QString queryString = "SELECT * FROM users WHERE login = :login";
    QSqlQuery query;
    query.prepare(queryString);

    query.bindValue(":login", login);

    if (query.exec()) {
        if (query.next()) {
            qDebug() << "Someone already use your login\n";
            return;
        }
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }
    QSqlQuery query2;
    query2.prepare("SELECT * FROM users WHERE email = :email");

    query2.bindValue(":email", email);

    if (query2.exec()) {
        if (query2.next()) {
            qDebug() << "Someone already use your email\n";
            return;
        }
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    QSqlQuery registrationQuery;
    registrationQuery.prepare("INSERT INTO `users`(`user_id`, `login`, `password`, `email`, `avatar`, `date_birth`, `registration_moment`) VALUES "
                              "(NULL, :login, :password, :email, NULL, :birthday, :current_moment)");
    registrationQuery.bindValue(":login", login);
    registrationQuery.bindValue(":password", password);
    registrationQuery.bindValue(":email", email);
    registrationQuery.bindValue(":birthday", birthday);

    QDateTime moment = QDateTime::currentDateTime();

    registrationQuery.bindValue(":current_moment", moment);

    if (registrationQuery.exec()) {
        qDebug() << "Registration successful\n";
        ui->tabWidget->setCurrentIndex(0);
        ui->loginInput->setText(login);
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

}

