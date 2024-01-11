#ifndef TITLEINFO_H
#define TITLEINFO_H

#include <QMainWindow>
#include "imgdownloader.h"
#include "accountinfo.h"
#include "episodeswindow.h"

namespace Ui {
class TitleInfo;
}

class TitleInfo : public QMainWindow
{
    Q_OBJECT

public:
    explicit TitleInfo(int title_id, QWidget *parent = nullptr);
    ~TitleInfo();
    int title_id;
private slots:
    void loadImage();

    void on_statusSelect_currentIndexChanged(int index);

    void on_goWatch_clicked();

    void on_scoreSelect_currentIndexChanged(int index);

    void on_whatchedEpisodesSet_editingFinished();

private:
    Ui::TitleInfo *ui;
    imgDownloader downloader;
    AccountInfo *accountInfo;
    EpisodesWindow *episodesWindow;
};

#endif // TITLEINFO_H
