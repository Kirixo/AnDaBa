#include "lb2_form.h"
#include "qsqlerror.h"
#include "ui_lb2_form.h"

LB2_form::LB2_form(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LB2_form)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("andaba");
    if(db.open()){

    } else {
        //qDebug() << "db error";
    }
}

LB2_form::~LB2_form()
{
    delete ui;
}

void LB2_form::on_doSQLButton_clicked()
{
    QString queryStr = ui->textEdit->toPlainText();
    QSqlQuery query(queryStr);
    if (query.exec()) {
        if (query.next()) {
            queryModel = new QSqlQueryModel();
            queryModel->setQuery(query);
            ui->tableView->setModel(queryModel);
        } else {
        }
        ui->status->setText("OK");
        ui->status->setStyleSheet("background-color: #81f968");
    } else {
        ui->status->setText("Error: "+query.lastError().text());
        ui->status->setStyleSheet("background-color: #ff6666");
    }

}


void LB2_form::on_exitButton_clicked()
{
    close();
}


void LB2_form::on_clearButton_clicked()
{
    ui->textEdit->clear();
}

