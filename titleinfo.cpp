#include "titleinfo.h"
#include "qdebug.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "ui_titleinfo.h"

TitleInfo::TitleInfo(int id, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TitleInfo)
{
    ui->setupUi(this);
    title_id = id;
    QString queryString = "SELECT * FROM anime WHERE anime_id = :id";
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":id", title_id);
    if(query.exec()){
        query.next();
        ui->title->setText(query.value(1).toString());
        ui->textBrowser->setText(query.value(2).toString());
        downloader.downloadImage(QUrl(query.value(3).toString()));
        connect(&downloader, &imgDownloader::imgDownloaded, this, &TitleInfo::loadImage);
        qDebug() << "????" << query.value(4) << query.value(4).toString();
        const QString month[] = {"January", "February", "March", "April", "May", "June", "July",
                           "August", "September", "October", "November", "December"};
        const QDate date = query.value(4).toDateTime().date();
        QString aired_start = date.isNull() ? "?" :
                                  QString("%1 %2 %3").arg(date.day()).arg(month[date.month()-1]).arg(date.year());
        const QDate date2 = query.value(5).toDateTime().date();
        QString aired_end = date2.isNull() ? "?" :
                                  QString("%1 %2 %3").arg(date2.day()).arg(month[date2.month()-1]).arg(date2.year());

        QString aired = QString("%1 to %2").arg(aired_start).arg(aired_end);
        ui->aired->setText(aired);
        ui->episodes->setText(query.value(7).toString());

    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    QString queryGenresStr = "SELECT genres.genre_name FROM anime_genre "
                          "JOIN genres ON genres.genre_id = anime_genre.genre_id "
                          "WHERE anime_genre.anime_id = :id";
    QSqlQuery queryGenres;
    queryGenres.prepare(queryGenresStr);
    queryGenres.bindValue(":id", title_id);
    if(queryGenres.exec()){
        QString genres = "";
        while (queryGenres.next()) {
            genres+= queryGenres.value(0).toString()+", ";
        }
        ui->genres->setText(genres);
    } else {
        qDebug() << "Error executing query:" << queryGenres.lastError().text();
    }

    accountInfo = AccountInfo::GetInstance();

    QSqlQuery listQuery;
    listQuery.prepare("SELECT `status_id`, `score`, `watched_eps` FROM lists WHERE anime_id = :anime_id AND user_id = :user_id");
    listQuery.bindValue(":anime_id", title_id);
    listQuery.bindValue(":user_id", accountInfo->user_id());
    if(listQuery.exec()){
        if(listQuery.next()) {
            ui->statusSelect->setCurrentIndex(listQuery.value(0).isNull() ? 5 : listQuery.value(0).toInt() - 1);
            ui->scoreSelect->setCurrentIndex(listQuery.value(1).toInt());
            ui->whatchedEpisodesSet->setValue(listQuery.value(2).toInt());
        }
    } else {
        qDebug() << "Error executing query0:" << listQuery.lastError().text();
    }
}

TitleInfo::~TitleInfo()
{
    delete ui;
    qDebug() << "destroyed" << title_id;
}

void TitleInfo::loadImage()
{
    QPixmap pm = downloader.getDownloadedData();
    ui->img->setPixmap(pm);
}

void TitleInfo::on_statusSelect_currentIndexChanged(int index)
{
    QString queryString = "SELECT * FROM lists WHERE anime_id = :anime_id AND user_id = :user_id";
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":anime_id", title_id);
    query.bindValue(":user_id", accountInfo->user_id());
    if(query.exec()){
        if(query.next()) {
            QSqlQuery updateInListQuery;
            updateInListQuery.prepare("UPDATE `lists` SET `status_id`= :status_id,`last_modify_moment`= :current_moment "
                                        "WHERE anime_id = :anime_id AND user_id = :user_id");
            updateInListQuery.bindValue(":anime_id", title_id);
            updateInListQuery.bindValue(":user_id", accountInfo->user_id());
            updateInListQuery.bindValue(":status_id", (index != 5) ? index+1 : QVariant());

            QDateTime moment = QDateTime::currentDateTime();

            updateInListQuery.bindValue(":current_moment", moment);

            if (updateInListQuery.exec()) {
                qDebug() << "Status changed\n";
            } else {
                qDebug() << "Error executing updateInListQuery:" << updateInListQuery.lastError().text();
                return;
            }
        } else {
            QSqlQuery insertInListQuery;
            insertInListQuery.prepare("INSERT INTO `lists`(`record_id`, `anime_id`, `user_id`, `status_id`, `score`, `watched_eps`, `last_modify_moment`) VALUES "
                                      "(NULL, :anime_id, :user_id, :status_id, NULL, NULL, :current_moment)");
            insertInListQuery.bindValue(":anime_id", title_id);
            insertInListQuery.bindValue(":user_id", accountInfo->user_id());
            insertInListQuery.bindValue(":status_id", index+1);

            QDateTime moment = QDateTime::currentDateTime();

            insertInListQuery.bindValue(":current_moment", moment);

            if (insertInListQuery.exec()) {
                qDebug() << "Status setted\n";
            } else {
                qDebug() << "Error executing insertInListQuery:" << insertInListQuery.lastError().text();
                return;
            }
        }
    } else {
        qDebug() << "Error executing query0:" << query.lastError().text();
    }
}


void TitleInfo::on_goWatch_clicked()
{
    episodesWindow = new EpisodesWindow();
    episodesWindow->show();
}


void TitleInfo::on_scoreSelect_currentIndexChanged(int index)
{
    QString queryString = "SELECT * FROM lists WHERE anime_id = :anime_id AND user_id = :user_id";
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":anime_id", title_id);
    query.bindValue(":user_id", accountInfo->user_id());
    if(query.exec()){
        if(query.next()) {
            QSqlQuery updateInListQuery;
            updateInListQuery.prepare("UPDATE `lists` SET `score`= :score,`last_modify_moment`= :current_moment "
                                      "WHERE anime_id = :anime_id AND user_id = :user_id");
            updateInListQuery.bindValue(":anime_id", title_id);
            updateInListQuery.bindValue(":user_id", accountInfo->user_id());
            updateInListQuery.bindValue(":score", (index != 0) ? index : QVariant());

            QDateTime moment = QDateTime::currentDateTime();

            updateInListQuery.bindValue(":current_moment", moment);

            if (updateInListQuery.exec()) {
                qDebug() << "Status changed\n";
            } else {
                qDebug() << "Error executing updateInListQuery:" << updateInListQuery.lastError().text();
                return;
            }
        } else {
//            QSqlQuery insertInListQuery;
//            insertInListQuery.prepare("INSERT INTO `lists`(`record_id`, `anime_id`, `user_id`, `status_id`, `score`, `watched_eps`, `last_modify_moment`) VALUES "
//                                      "(NULL, :anime_id, :user_id, :status_id, NULL, NULL, :current_moment)");
//            insertInListQuery.bindValue(":anime_id", title_id);
//            insertInListQuery.bindValue(":user_id", accountInfo->user_id());
//            insertInListQuery.bindValue(":status_id", index+1);

//            QDateTime moment = QDateTime::currentDateTime();

//            insertInListQuery.bindValue(":current_moment", moment);

//            if (insertInListQuery.exec()) {
//                qDebug() << "Status setted\n";
//            } else {
//                qDebug() << "Error executing insertInListQuery:" << insertInListQuery.lastError().text();
//                return;
//            }
        }
    } else {
        qDebug() << "Error executing query0:" << query.lastError().text();
    }
}


void TitleInfo::on_whatchedEpisodesSet_editingFinished()
{
    QString queryString = "SELECT * FROM lists WHERE anime_id = :anime_id AND user_id = :user_id";
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":anime_id", title_id);
    query.bindValue(":user_id", accountInfo->user_id());
    if(query.exec()){
        if(query.next()) {
            QSqlQuery updateInListQuery;
            updateInListQuery.prepare("UPDATE `lists` SET `watched_eps`= :watched,`last_modify_moment`= :current_moment "
                                      "WHERE anime_id = :anime_id AND user_id = :user_id");
            updateInListQuery.bindValue(":anime_id", title_id);
            updateInListQuery.bindValue(":user_id", accountInfo->user_id());
            updateInListQuery.bindValue(":watched", ui->whatchedEpisodesSet->value());

            QDateTime moment = QDateTime::currentDateTime();

            updateInListQuery.bindValue(":current_moment", moment);

            if (updateInListQuery.exec()) {
                qDebug() << "Status changed\n";
            } else {
                qDebug() << "Error executing updateInListQuery:" << updateInListQuery.lastError().text();
                return;
            }
        } else {
//            QSqlQuery insertInListQuery;
//            insertInListQuery.prepare("INSERT INTO `lists`(`record_id`, `anime_id`, `user_id`, `status_id`, `score`, `watched_eps`, `last_modify_moment`) VALUES "
//                                      "(NULL, :anime_id, :user_id, :status_id, NULL, NULL, :current_moment)");
//            insertInListQuery.bindValue(":anime_id", title_id);
//            insertInListQuery.bindValue(":user_id", accountInfo->user_id());
//            insertInListQuery.bindValue(":status_id", index+1);

//            QDateTime moment = QDateTime::currentDateTime();

//            insertInListQuery.bindValue(":current_moment", moment);

//            if (insertInListQuery.exec()) {
//                qDebug() << "Status setted\n";
//            } else {
//                qDebug() << "Error executing insertInListQuery:" << insertInListQuery.lastError().text();
//                return;
//            }
        }
    } else {
        qDebug() << "Error executing query0:" << query.lastError().text();
    }
}

