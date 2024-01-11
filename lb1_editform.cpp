#include "lb1_editform.h"
#include "qdebug.h"
#include "qsqlerror.h"
#include "qsqlrecord.h"
#include "ui_lb1_editform.h"

LB1_editForm::LB1_editForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LB1_editForm)
{
    ui->setupUi(this);
}

LB1_editForm::~LB1_editForm()
{
    delete queryModel;
    delete ui;
}

void LB1_editForm::setRecord(const QSqlRecord &record)
{
    QSqlQuery query1(QString("SELECT genres.genre_name FROM genres WHERE genres.genre_id = %1").arg(record.value(0).toString()));

    if (query1.exec()) {
        if (query1.next()) {
            ui->lineEdit->setText(query1.value(0).toString());
            ui->lineEdit->setReadOnly(1);
        } else {
            qDebug() << "SQL query failed\n";
        }
    } else {
        qDebug() << "Error executing query:" << query1.lastError().text();
    }
    QString queryStr = QString("SELECT * FROM anime "
                               "WHERE anime.anime_id IN (SELECT anime_genre.anime_id FROM anime_genre "
                                                    "WHERE anime_genre.genre_id = %1)"
                               ).arg(record.value(0).toString());

    QSqlQuery query(queryStr);
    queryModel = new QSqlQueryModel();
    queryModel->setQuery(query);
    ui->tableView->setModel(queryModel);
}

void LB1_editForm::saveChanges()
{
    emit dataSaved();
}
