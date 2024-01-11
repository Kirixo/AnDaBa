#ifndef LB1_EDITFORM_H
#define LB1_EDITFORM_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlQuery>

namespace Ui {
class LB1_editForm;
}

class LB1_editForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit LB1_editForm(QWidget *parent = nullptr);
    ~LB1_editForm();
    void setRecord(const QSqlRecord &record);

    QSqlQueryModel* queryModel;

signals:
    void dataSaved();

private slots:
    void saveChanges();

private:
    Ui::LB1_editForm *ui;
};

#endif // LB1_EDITFORM_H
