#include "addepisodeform.h"
#include "qdatetime.h"
#include "qdebug.h"
#include "qsqlerror.h"
#include "ui_addepisodeform.h"
#include "src/SmtpMime"

AddEpisodeForm::AddEpisodeForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddEpisodeForm)
{
    ui->setupUi(this);
}

AddEpisodeForm::~AddEpisodeForm()
{
    delete ui;
}

void AddEpisodeForm::on_cancelButton_clicked()
{
    close();
}


void AddEpisodeForm::on_okButton_clicked()
{
    QString url = "";

    QString queryStr = "INSERT INTO `episodes`(`episode_id`, `anime_id`, `episode_number`, `video_url`, `translator_id`, `upload_moment`) VALUES ";
    queryStr += "(NULL, ";
    queryStr += QString::number(anime_id) + ", ";
    if(!ui->episodeNum->text().isEmpty()){
        queryStr += "" + ui->episodeNum->text() + ", ";
        ui->episodeNum->clear();
    } else {
        return;
    }
    queryStr += "'" + url + "', ";


    queryStr += "" + QString::number(ui->comboBox->currentIndex()+1) + ", ";

    queryStr += ":current_moment);    ";
    QSqlQuery query;
    query.prepare(queryStr);
    QDateTime moment = QDateTime::currentDateTime();
    query.bindValue(":current_moment", moment);
    if (query.exec()) {
        qDebug() << "Episode added\n";
        informUsers();
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    close();
}

void AddEpisodeForm::on_uplloadButton_clicked()
{

}


void AddEpisodeForm::setRecord(const int &record)
{
    anime_id = record;
    QSqlQuery query1;
    query1.prepare(QString("SELECT anime.anime_name FROM anime WHERE anime.anime_id = %1").arg(record));
    if (query1.exec()) {
        if (query1.next()) {
            ui->name->setText(query1.value(0).toString());
            ui->name->setReadOnly(1);
        } else {
            qDebug() << "SQL query failed\n";
        }
    } else {
        qDebug() << "Error executing query:" << query1.lastError().text();
    }

    QString queryStr = QString("SELECT translators.translator_id, translators.translator_name FROM translators ");
    QSqlQuery query;
    query.prepare(queryStr);
    if (query.exec()) {
        while(query.next()) {
            ui->comboBox->addItem(query.value(1).toString());
        }
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }
}

void AddEpisodeForm::informUsers()
{
    QSqlQuery query1;
    query1.prepare(QString("SELECT users.email, users.login, anime.anime_name "
                           "FROM lists "
                           "JOIN users ON users.user_id = lists.user_id "
                           "JOIN anime ON anime.anime_id = lists.anime_id "
                           "WHERE anime.anime_id = :anime_id AND lists.status_id = 1"));
    query1.bindValue(":anime_id", anime_id);

    if (query1.exec()) {
        while(query1.next()) {
            SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);
            smtp.setUser("llkairall1@gmail.com");
            smtp.setPassword("melyraoxueyovpol");
            MimeMessage message;

            message.setSender(new EmailAddress("llkairall1@gmail.com", "Kirixo"));
            message.addRecipient(new EmailAddress(query1.value(0).toString(), "Our User"));
            message.setSubject(QString("New episone of "+query1.value(2).toString()));

            MimeText text;
            text.setText(QString("Hi, %1.\nLets go to watch new episode of %2").arg(query1.value(1).toString()).arg(query1.value(2).toString()));
            message.addPart(&text);
            smtp.connectToHost();
            smtp.login();
            smtp.sendMail(message);
            smtp.quit();
        }
    } else {
        qDebug() << "Error executing query:" << query1.lastError().text();
    }

}
