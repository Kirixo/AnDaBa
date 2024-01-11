#include "addrowform.h"
#include "qdebug.h"
#include "qsqlerror.h"
#include "ui_addrowform.h"

addRowForm::addRowForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::addRowForm)
{
    ui->setupUi(this);

    addGenreRow();
}

addRowForm::~addRowForm()
{
    delete ui;
}

void addRowForm::addGenreRow()
{
    int index = genreButton.size();
    genreButton.push_back(new QPushButton());
    connect(genreButton[index], &QPushButton::clicked, this, &addRowForm::addGenreRow);
    genreButton[index]->setText("Add genre");
    if(index > 0) {
        genreButton[index-1]->setText("Delete genre");
        disconnect(genreButton[index-1], &QPushButton::clicked, 0, 0);
        connect(genreButton[index-1], &QPushButton::clicked, this, [=](){ deleteGenreRow(index-1); });
    }
    genreSelect.push_back(new QComboBox());
    QSqlQuery query("SELECT * FROM genres");
    if (query.exec()) {
        while(query.next()){
            genreSelect[index]->addItem(query.value(1).toString());
        }
    } else {
        qDebug() << query.lastError().text();
    }
    QFormLayout* formLayout = qobject_cast<QFormLayout*>(ui->scrollAreaWidgetContents->layout());
    formLayout->addRow(genreButton[index], genreSelect[index]);
}

void addRowForm::deleteGenreRow(int index)
{
    delete genreButton[index];
    delete genreSelect[index];
}

void addRowForm::on_cancelButton_clicked()
{
    close();
}


void addRowForm::on_okButton_clicked()
{
    QString queryStr = "INSERT INTO `anime` (`anime_id`, `anime_name`, `description`, `image`, `aired_start`, `aired_end`, `total_episodes`) VALUES ";
    queryStr += "(NULL, ";
    if(!ui->name->text().isEmpty()){
        queryStr += "'" + ui->name->text() + "', ";
        ui->name->clear();
    } else {
        return;
    }
    queryStr += "'" + ui->description->toPlainText() + "', ";
    queryStr += "'" + ui->image->text() + "', ";
    queryStr += "'" + QString::number(ui->aired_start->date().year()) + "-" +
                QString::number(ui->aired_start->date().month()) + "-" +
                QString::number(ui->aired_start->date().day()) + "', ";
    queryStr += "'" + QString::number(ui->aired_end->date().year()) + "-" +
                QString::number(ui->aired_end->date().month()) + "-" +
                QString::number(ui->aired_end->date().day()) + "', ";
    //queryStr += "NULL, ";
    queryStr += "'" + QString::number(ui->total_episodes->value()) + "');    ";
    QSqlQuery query(queryStr);

    QSqlQuery counterQuery("SELECT MAX(id) FROM titles");
    counterQuery.next();
    int id = counterQuery.value(0).toInt();
    QString str;
    for(auto&& genre : genreSelect) {
        str += "INSERT INTO `anime_genre` (`anime_id`, `genre_id`) VALUES ";
        str += "(" + QString::number(id) + ", " + QString::number(genre->currentIndex()+1) + "); ";
    }
    QSqlQuery query1;
    query1.prepare(str);
    if (query1.exec()) {
        qDebug() << "OK" << str;
        qDebug() << query1.lastError().text();
    } else {
        qDebug() << query1.lastError().text() << '\n' << str;
    }
    close();
}

