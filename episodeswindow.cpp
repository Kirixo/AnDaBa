#include "episodeswindow.h"
#include "ui_episodeswindow.h"

EpisodesWindow::EpisodesWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EpisodesWindow)
{
    ui->setupUi(this);
    eps.resize(3);
    eps[0] = new episode();
    //eps[0]->setTitle(query.value(1).toString());
    //eps[0]->setImage(query.value(3).toString());
    //eps[0]->showContent();
    ui->scrollAreaWidgetContents->layout()->addWidget(eps[0]);
}

EpisodesWindow::~EpisodesWindow()
{
    delete ui;
}

