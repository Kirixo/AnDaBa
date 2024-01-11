#ifndef CATALOG_H
#define CATALOG_H

#include "title.h"
#include <QSqlQuery>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSql>
#include <QVector>
#include <QPushButton>
#include "profile.h"
#include "accountinfo.h"
#include "sort.h"

namespace Ui {
class Catalog;
}

class Catalog : public QMainWindow
{
    Q_OBJECT

public:
    explicit Catalog(QWidget *parent = nullptr);
    ~Catalog();


private slots:
    void on_searchButton_clicked();

    void on_pushButton_clicked();

    void profile_open();

    void profile_close();

    void filter_open();

    void filter_close();

public slots:
    void filter_by_status(const int& status_id);
    void filter_by_genre(const int& genre_id);

    void sort_by_alphabet();
    void sort_by_score();
    void sort_by_aired();

private:
    Ui::Catalog *ui;
    QSqlDatabase db;
    QVector<Title*> title;
    void fillCatalog();
    void addTitle(Title* title);
    QAction *profileAction;

    Sort *filterWindow;
    Profile *profileWindow;
    AccountInfo *accountInfo;
    QString lastQuery;
    QString sort;
};

#endif // CATALOG_H
