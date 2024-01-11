#ifndef ADDROWFORM_H
#define ADDROWFORM_H

#include "qpushbutton.h"
#include <QMainWindow>
#include <QComboBox>
#include <QSqlQuery>

namespace Ui {
class addRowForm;
}

class addRowForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit addRowForm(QWidget *parent = nullptr);
    ~addRowForm();

private slots:
    void addGenreRow();

    void deleteGenreRow(int index);

    void on_cancelButton_clicked();

    void on_okButton_clicked();

private:
    Ui::addRowForm *ui;
    QSqlDatabase db;
    QVector<QPushButton*> genreButton;
    QVector<QComboBox*> genreSelect;
};

#endif // ADDROWFORM_H
