#include "profile.h"
#include "qdebug.h"
#include "qpainter.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "qsqltablemodel.h"
#include "qtableview.h"
#include "qtextdocument.h"
#include "ui_profile.h"
#include "qprinter.h"
#include "catalog.h"
#include <QPrintDialog>

Profile::Profile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Profile)
{
    ui->setupUi(this);
    this->setGeometry(5, 50, 700, 400);
    this->hide();

    accountInfo = AccountInfo::GetInstance();
    reloadStats();

    connect(ui->pushButton, &QPushButton::clicked, parent, [=]() { dynamic_cast<Catalog*>(parent)->filter_by_status(1); });
    connect(ui->pushButton_2, &QPushButton::clicked, parent, [=]() { dynamic_cast<Catalog*>(parent)->filter_by_status(2); });
    connect(ui->pushButton_3, &QPushButton::clicked, parent, [=]() { dynamic_cast<Catalog*>(parent)->filter_by_status(3); });
    connect(ui->pushButton_4, &QPushButton::clicked, parent, [=]() { dynamic_cast<Catalog*>(parent)->filter_by_status(4); });
    connect(ui->pushButton_5, &QPushButton::clicked, parent, [=]() { dynamic_cast<Catalog*>(parent)->filter_by_status(5); });

    QSqlQuery maxGenre("SELECT MAX(genres.genre_id) FROM genres");
    genreButton.resize(19);
    genreLabe.resize(19);

    QSqlQuery query;
    query.prepare("SELECT genres.genre_id, COUNT(lists.anime_id), genres.genre_name "
                    "FROM genres "
                    "LEFT OUTER JOIN anime_genre ON anime_genre.genre_id = genres.genre_id "
                    "LEFT OUTER JOIN anime ON anime_genre.anime_id = anime.anime_id "
                    "LEFT OUTER JOIN lists ON lists.anime_id = anime.anime_id AND lists.user_id = 2 "
                    "LEFT OUTER JOIN users ON lists.user_id = users.user_id "
                    "GROUP BY genres.genre_id;");

    query.bindValue(":user_id", accountInfo->user_id());

    if (query.exec()) {
        while (query.next()) {
            //qDebug() << query.value(0).toInt() << " " << query.value(1).toInt() << "" << query.value(2).toString();
            genreButton[query.value(0).toInt()] = new QPushButton(query.value(2).toString());
            connect(genreButton[query.value(0).toInt()], &QPushButton::clicked, parent, [=]() { dynamic_cast<Catalog*>(parent)->filter_by_genre(query.value(0).toInt()); });
            genreLabe[query.value(0).toInt()] = new QLabel(query.value(1).toString());
            dynamic_cast<QFormLayout*>(ui->formLayout_1->layout())->addRow(genreButton[query.value(0).toInt()], genreLabe[query.value(0).toInt()]);
            qDebug() << query.value(0).toInt() << ' ' << query.value(1).toInt();
        }
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

}

Profile::~Profile()
{
    delete ui;
}

void Profile::reloadStats()
{
    ui->username->setText(QString(accountInfo->login()+"'s Profile"));
    QVector<uint> statuses(5, 0);

    QSqlQuery query;
    query.prepare("SELECT status_id, COUNT(*) "
                    "FROM lists "
                    "WHERE user_id = :user_id "
                    "GROUP BY user_id, status_id;");

    query.bindValue(":user_id", accountInfo->user_id());

    if (query.exec()) {
        while (query.next()) {
            if(query.value(0).isNull()) {
                continue;
            }
            statuses[query.value(0).toInt()-1] += query.value(1).toInt();
            qDebug() << query.value(0).toInt() << ' ' << query.value(1).toInt();
        }
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    ui->watchingLabe->setText(QString::number(statuses[0]));
    ui->completedLabe->setText(QString::number(statuses[1]));
    ui->onholdLabe->setText(QString::number(statuses[2]));
    ui->droppedLabe->setText(QString::number(statuses[3]));
    ui->planLabe->setText(QString::number(statuses[4]));


}

void Profile::on_pushButton_6_clicked()
{
    QSqlTableModel *tableModel = new QSqlTableModel();
    tableModel->setTable("lists");
    QSqlQuery query(QString("SELECT statuses.status_name AS Status, anime.anime_name AS Tiltle, lists.score AS Score, lists.watched_eps AS 'Watched<\br>episodes' "
                            "FROM lists "
                            "JOIN anime ON anime.anime_id = lists.anime_id "
                            "JOIN statuses ON statuses.status_id = lists.status_id "
                            "WHERE user_id = %1 "
                            "ORDER BY lists.status_id").arg(accountInfo->user_id()));
    QSqlQueryModel *queryModel = new QSqlQueryModel();
    queryModel->setQuery(query);
    QTableView *tableView = new QTableView();
    tableView->setModel(queryModel);

    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = tableView->model()->rowCount();
    const int columnCount = tableView->model()->columnCount();

    out <<  "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
        <<  QString("<title>%1</title>\n").arg("User's lists")
        <<  QString("</head>\n"
                   "<body bgcolor=#ffffff link=#5000A0>\n"
                   "<h1 align='center'>%1's lists</h1>\n"
                   "<table border=1 cellspacing=0 cellpadding=2>\n").arg(accountInfo->login());
    out << "<thead><tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; column++)
        if (!tableView->isColumnHidden(column))
            out << QString("<th>%1</th>").arg(tableView->model()->headerData(column, Qt::Horizontal).toString());
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++) {
        out << "<tr>";
        for (int column = 0; column < columnCount; column++) {
            if (!tableView->isColumnHidden(column)) {
                QString data = tableView->model()->data(tableView->model()->index(row, column)).toString().simplified();
                out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }
    out <<  "</table>\n"
           "</body>\n"
           "</html>\n";

    QTextDocument *document = new QTextDocument();
    document->setHtml(strStream);

    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, NULL);
    if (dialog->exec() == QDialog::Accepted) {
        document->print(&printer);
    }

    delete document;
}


void Profile::on_pushButton_12_clicked()
{
    QSqlTableModel *tableModel = new QSqlTableModel();
    tableModel->setTable("lists");
    QSqlQuery query(QString("SELECT anime.anime_name AS Tiltle, anime.aired_start AS Premiere "
                            "FROM anime "
                            "WHERE ISNULL(anime.aired_start) OR anime.aired_start > CURDATE() "
                            "ORDER BY anime.aired_start"));
    QSqlQueryModel *queryModel = new QSqlQueryModel();
    queryModel->setQuery(query);
    QTableView *tableView = new QTableView();
    tableView->setModel(queryModel);

    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = tableView->model()->rowCount();
    const int columnCount = tableView->model()->columnCount();

    out <<  "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
        <<  QString("<title>%1</title>\n").arg("User's lists")
        <<  QString("</head>\n"
                   "<body bgcolor=#ffffff link=#5000A0>\n"
                   "<h1 align='center'>Anouncements</h1>\n"
                   "<table border=1 cellspacing=0 cellpadding=2>\n");
    out << "<thead><tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; column++)
        if (!tableView->isColumnHidden(column))
            out << QString("<th>%1</th>").arg(tableView->model()->headerData(column, Qt::Horizontal).toString());
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++) {
        out << "<tr>";
        for (int column = 0; column < columnCount; column++) {
            if (!tableView->isColumnHidden(column)) {
                QString data = tableView->model()->data(tableView->model()->index(row, column)).toString().simplified();
                out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }
    out <<  "</table>\n"
           "</body>\n"
           "</html>\n";

    QTextDocument *document = new QTextDocument();
    document->setHtml(strStream);

    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, NULL);
    if (dialog->exec() == QDialog::Accepted) {
        document->print(&printer);
    }

    delete document;
}

