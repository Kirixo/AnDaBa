#ifndef TITLE_H
#define TITLE_H

#include <QWidget>

#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "imgdownloader.h"
#include "titleinfo.h"


namespace Ui {
class title;
}

class Title : public QWidget
{
    Q_OBJECT

public:
    explicit Title(int id, QWidget *parent = nullptr);
    void showContent();
    void setImage(QString image);
    void setTitle(QString title);
    ~Title();
/*signals:
    void downloaded();*/
private slots:
    void downloadFinished();
    void showTitleInfo();

private:
    int title_id;
    QString image;
    QString title = "XD";
    imgDownloader downloader;
    TitleInfo *infoTab = nullptr;
    Ui::title *ui;
};

#endif // TITLE_H
