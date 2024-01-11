#include "catalog.h"
#include "qsqlerror.h"
#include "ui_catalog.h"

Catalog::Catalog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Catalog)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("andaba");
    if(db.open()){
        qDebug() << "andaba opened";
        fillCatalog();
    } else {
        qDebug() << "db error";
    }
    sort = " ORDER BY anime.anime_name ASC";
    profileAction = new QAction("Profile");
    connect(profileAction, &QAction::triggered, this, &Catalog::profile_open);
    ui->toolBar->addAction(profileAction);

    profileWindow = new Profile(this);
    filterWindow = new Sort(this);
    accountInfo = AccountInfo::GetInstance();
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Catalog::filter_open);
}

Catalog::~Catalog()
{
    delete ui;
    for (auto var : title) {
        delete var;
    }
}

void Catalog::fillCatalog()
{
    QString queryString = "SELECT * FROM anime";
    QSqlQuery query;
    query.prepare(queryString);
    if(query.exec()){
        qDebug() << query.size();
        title.resize(query.size());
        for (int i = 0; query.next(); ++i) {
            title[i] = new Title(query.value(0).toInt());
            title[i]->setTitle(query.value(1).toString());
            title[i]->setImage(query.value(3).toString());
            title[i]->showContent();
            addTitle(title[i]);
        }
        lastQuery = query.lastQuery();
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }
}

void Catalog::addTitle(Title *title)
{
    ui->grid->layout()->addWidget(title);
}

void Catalog::on_searchButton_clicked()
{
    QString searchInput = ui->searchInput->text();
    QString queryString = QString("SELECT * FROM anime WHERE anime_name LIKE '%%1%'").arg(searchInput);
    QSqlQuery query;
    for (int i = 0; i < title.size(); ++i) {
        delete title[i];
    }
    query.prepare(queryString);
    if(query.exec()){
        qDebug() << query.size();
        title.resize(query.size());
        for (int i = 0; query.next(); ++i) {
            title[i] = new Title(query.value(0).toInt());
            title[i]->setTitle(query.value(1).toString());
            title[i]->setImage(query.value(3).toString());
            title[i]->showContent();
            addTitle(title[i]);
        }
        lastQuery = query.lastQuery();
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }
    qDebug() << query.lastQuery();
}


void Catalog::on_pushButton_clicked()
{

}

void Catalog::filter_open()
{
    disconnect(ui->pushButton_2, &QPushButton::clicked, 0, 0);
    //filterWindow->reloadStats();
    filterWindow->show();
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Catalog::filter_close);
}

void Catalog::filter_close()
{
    disconnect(ui->pushButton_2, &QPushButton::clicked, 0, 0);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Catalog::filter_open);
    filterWindow->hide();
}

void Catalog::profile_open()
{
    disconnect(profileAction, &QAction::triggered, 0, 0);
    profileWindow->reloadStats();
    profileWindow->show();
    connect(profileAction, &QAction::triggered, this, &Catalog::profile_close);
}

void Catalog::profile_close()
{
    disconnect(profileAction, &QAction::triggered, 0, 0);
    connect(profileAction, &QAction::triggered, this, &Catalog::profile_open);
    profileWindow->hide();
}

void Catalog::filter_by_status(const int &status_id)
{
    QString queryString = QString("SELECT * FROM anime "
                                  "JOIN lists ON anime.anime_id = lists.anime_id "
                                  "JOIN users ON users.user_id = lists.user_id "
                                  "WHERE users.user_id = %1 AND lists.status_id = %2").arg(accountInfo->user_id()).arg(status_id);
    QSqlQuery query;
    for (int i = 0; i < title.size(); ++i) {
        delete title[i];
    }
    query.prepare(queryString);
    if(query.exec()){
        qDebug() << query.size();
        title.resize(query.size());
        for (int i = 0; query.next(); ++i) {
            title[i] = new Title(query.value(0).toInt());
            title[i]->setTitle(query.value(1).toString());
            title[i]->setImage(query.value(3).toString());
            title[i]->showContent();
            addTitle(title[i]);
        }
        lastQuery = query.lastQuery();
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }
    qDebug() << query.lastQuery();
}


void Catalog::filter_by_genre(const int &genre_id)
{

    QString queryString = QString("SELECT * FROM anime "
                                  "JOIN anime_genre ON anime.anime_id = anime_genre.anime_id "
                                  "WHERE anime_genre.genre_id = %1").arg(genre_id);
    QSqlQuery query;
    for (int i = 0; i < title.size(); ++i) {
        delete title[i];
    }
    query.prepare(queryString);
    if(query.exec()){
        qDebug() << query.size();
        title.resize(query.size());
        for (int i = 0; query.next(); ++i) {
            title[i] = new Title(query.value(0).toInt());
            title[i]->setTitle(query.value(1).toString());
            title[i]->setImage(query.value(3).toString());
            title[i]->showContent();
            addTitle(title[i]);
        }
        lastQuery = query.lastQuery();
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }
    qDebug() << query.lastQuery();
}

void Catalog::sort_by_alphabet()
{
    sort = " ORDER BY anime.anime_name ASC";
    QSqlQuery query;
    for (int i = 0; i < title.size(); ++i) {
        delete title[i];
    }
    query.prepare(lastQuery+sort);
    if(query.exec()){
        qDebug() << query.size();
        title.resize(query.size());
        for (int i = 0; query.next(); ++i) {
            title[i] = new Title(query.value(0).toInt());
            title[i]->setTitle(query.value(1).toString());
            title[i]->setImage(query.value(3).toString());
            title[i]->showContent();
            addTitle(title[i]);
        }
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }
}

void Catalog::sort_by_score()
{

//    sort = " ORDER BY anime.anime_name ASC";
//    QSqlQuery query;
//    for (int i = 0; i < title.size(); ++i) {
//        delete title[i];
//    }
//    query.prepare(lastQuery+sort);
//    if(query.exec()){
//        qDebug() << query.size();
//        title.resize(query.size());
//        for (int i = 0; query.next(); ++i) {
//            title[i] = new Title(query.value(0).toInt());
//            title[i]->setTitle(query.value(1).toString());
//            title[i]->setImage(query.value(3).toString());
//            title[i]->showContent();
//            addTitle(title[i]);
//        }
//    } else {
//        qDebug() << "Error executing query:" << query.lastError().text();
//    }
}

void Catalog::sort_by_aired()
{
    qDebug() << "sort by aired";
    qDebug() << lastQuery;
    sort = " ORDER BY anime.aired_start ASC";
    QSqlQuery query;
    for (int i = 0; i < title.size(); ++i) {
        delete title[i];
    }
    query.prepare(lastQuery+sort);
    if(query.exec()){
        qDebug() << query.size();
        title.resize(query.size());
        for (int i = 0; query.next(); ++i) {
            title[i] = new Title(query.value(0).toInt());
            title[i]->setTitle(query.value(1).toString());
            title[i]->setImage(query.value(3).toString());
            title[i]->showContent();
            addTitle(title[i]);
        }
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }
}
