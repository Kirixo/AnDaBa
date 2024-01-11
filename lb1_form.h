#ifndef LB1_FORM_H
#define LB1_FORM_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include "lb2_form.h"
#include "addrowform.h"

namespace Ui {
class LB1_form;
}

class LB1_form : public QMainWindow
{
    Q_OBJECT

public:
    explicit LB1_form(QWidget *parent = nullptr);
    ~LB1_form();

private slots:
    void closeApplication();
    void showDataBase(QString dbName);
    void editRecord(const QModelIndex &index);
    void reloadData();
    void openQueryEdit();
    void deleteTable();
    void uptadeTable();
    void addRow();

private:
    Ui::LB1_form *ui;
    QSqlDatabase db;
    QSqlTableModel* tableModel;
    QSqlTableModel* tableModel1;
    QSqlQueryModel* queryModel = new QSqlQueryModel();
    LB2_form* queryForm;
    addRowForm* addForm;
};

#endif // LB1_FORM_H
