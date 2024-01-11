#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QMouseEvent>

class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event);

signals:
    void clicked();

};

#endif // TITLEWIDGET_H
