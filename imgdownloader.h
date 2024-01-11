#ifndef IMGDOWNLOADER_H
#define IMGDOWNLOADER_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class imgDownloader : public QObject
{
    Q_OBJECT
public:
    explicit imgDownloader(QObject *parent = nullptr);
    void downloadImage(QUrl imgURL);
    QPixmap getDownloadedData() const;

    signals:
        void imgDownloaded();

private slots:
    void downloadFinished(QNetworkReply *pReply);

private:
    QUrl imgURL;
    QPixmap downloadedData;
    QNetworkAccessManager nam;
};

#endif // IMGDOWNLOADER_H
