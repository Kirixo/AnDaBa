#ifndef SORT_H
#define SORT_H

#include <QWidget>

namespace Ui {
class Sort;
}

class Sort : public QWidget
{
    Q_OBJECT

public:
    explicit Sort(QWidget *parent = nullptr);
    ~Sort();

private slots:

private:
    Ui::Sort *ui;
};

#endif // SORT_H
