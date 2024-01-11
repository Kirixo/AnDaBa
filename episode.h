#ifndef EPISODE_H
#define EPISODE_H

#include <QWidget>

namespace Ui {
class episode;
}

class episode : public QWidget
{
    Q_OBJECT

public:
    explicit episode(QWidget *parent = nullptr);
    ~episode();

private:
    Ui::episode *ui;
};

#endif // EPISODE_H
