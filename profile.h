#ifndef PROFILE_H
#define PROFILE_H

#include <QWidget>
#include "accountinfo.h"
#include "qlabel.h"
#include "qpushbutton.h"

namespace Ui {
class Profile;
}

class Profile : public QWidget
{
    Q_OBJECT

public:
    explicit Profile(QWidget *parent = nullptr);
    ~Profile();

    void reloadStats();

private slots:
    void on_pushButton_6_clicked();

    void on_pushButton_12_clicked();

private:
    Ui::Profile *ui;
    QVector<QPushButton*> genreButton;
    QVector<QLabel*> genreLabe;
    AccountInfo *accountInfo;
};

#endif // PROFILE_H
