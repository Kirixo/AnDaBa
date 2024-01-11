#include "title.h"
#include "imgdownloader.h"
#include "ui_title.h"



Title::Title(int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::title)
{
    ui->setupUi(this);
    title_id = id;

    QSizePolicy policy = ui->img->sizePolicy();
    policy.setVerticalStretch(5);
    ui->img->setSizePolicy(policy);
    QSizePolicy policy2 = ui->titletext->sizePolicy();
    policy2.setVerticalStretch(5);
    ui->titletext->setSizePolicy(policy2);
    connect(ui->layout, SIGNAL(clicked()), SLOT(showTitleInfo()));
}

void Title::showContent()
{
    ui->titletext->setText(title);
    downloader.downloadImage(QUrl(image));
    connect(&downloader, &imgDownloader::imgDownloaded, this, &Title::downloadFinished);
}

void Title::setImage(QString image)
{
    this->image = image;
}

void Title::setTitle(QString title)
{
    this->title = title;
}

void Title::downloadFinished()
{
    QPixmap pm = downloader.getDownloadedData();
    ui->img->setPixmap(pm);
//    QNetworkAccessManager *nam = qobject_cast<QNetworkAccessManager*>(sender());
//    if (nam) {
//        nam->deleteLater();
//    }
}

void Title::showTitleInfo()
{
    if (infoTab != nullptr) {
        // Освобождение памяти от предыдущего объекта
        //infoTab->close();
        //qDebug() << "must delete" << infoTab->title_id;
        infoTab->deleteLater();
    }

    infoTab = new TitleInfo(title_id);
    infoTab->show();

    qDebug() << "OH, rly clicked\n" << title;
}

Title::~Title()
{
    qDebug() << "destroyed";
    delete infoTab;
    delete ui;
}
