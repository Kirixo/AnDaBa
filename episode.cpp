#include "episode.h"
#include "ui_episode.h"

episode::episode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::episode)
{
    ui->setupUi(this);
}

episode::~episode()
{
    delete ui;
}
