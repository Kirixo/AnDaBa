#include "titlewidget.h"

TitleWidget::TitleWidget(QWidget *parent)
    : QWidget{parent}
{

}

void TitleWidget::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}
