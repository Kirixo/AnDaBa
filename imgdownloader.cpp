#include "imgdownloader.h"

imgDownloader::imgDownloader(QObject *parent)
    : QObject{parent}
{

}

void imgDownloader::downloadImage(QUrl imgURL)
{
    connect(&nam, &QNetworkAccessManager::finished, this, &imgDownloader::downloadFinished);
    QNetworkRequest request(imgURL);
    nam.get(request);

}

QPixmap imgDownloader::getDownloadedData() const
{
    return downloadedData;
}

void imgDownloader::downloadFinished(QNetworkReply *pReply)
{
    downloadedData.loadFromData(pReply->readAll());
    emit imgDownloaded();
}
