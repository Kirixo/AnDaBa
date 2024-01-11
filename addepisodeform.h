#ifndef ADDEPISODEFORM_H
#define ADDEPISODEFORM_H

#include <QMainWindow>
#include <QSqlQuery>

namespace Ui {
class AddEpisodeForm;
}

class AddEpisodeForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddEpisodeForm(QWidget *parent = nullptr);
    ~AddEpisodeForm();
    void setRecord(const int &record);
    void informUsers();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

    void on_uplloadButton_clicked();

private:
    Ui::AddEpisodeForm *ui;
    int anime_id;
};

#endif // ADDEPISODEFORM_H
