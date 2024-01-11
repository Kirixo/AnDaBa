#ifndef EPISODESWINDOW_H
#define EPISODESWINDOW_H

#include <QWidget>
#include "episode.h"

namespace Ui {
class EpisodesWindow;
}

class EpisodesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EpisodesWindow(QWidget *parent = nullptr);
    ~EpisodesWindow();
    void setRecord(const int &record);

private:
    Ui::EpisodesWindow *ui;
    void saveChanges();
    QVector<episode*> eps;
};

#endif // EPISODESWINDOW_H
