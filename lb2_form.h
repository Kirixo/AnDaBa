#ifndef LB2_FORM_H
#define LB2_FORM_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStyle>

namespace Ui {
class LB2_form;
}

class LB2_form : public QMainWindow
{
    Q_OBJECT

public:
    explicit LB2_form(QWidget *parent = nullptr);
    ~LB2_form();

private slots:
    void on_clearButton_clicked();

    void on_doSQLButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::LB2_form *ui;
    QSqlDatabase db;
    QSqlQueryModel* queryModel;
};

#endif // LB2_FORM_H
