#include "sort.h"
#include "ui_sort.h"
#include "catalog.h"

Sort::Sort(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sort)
{
    ui->setupUi(this);
    this->hide();
    connect(ui->alphabet, &QPushButton::clicked, parent, [=]() { dynamic_cast<Catalog*>(parent)->sort_by_alphabet(); });
    connect(ui->score, &QPushButton::clicked, parent, [=]() { dynamic_cast<Catalog*>(parent)->sort_by_score(); });
    connect(ui->aired, &QPushButton::clicked, parent, [=]() { dynamic_cast<Catalog*>(parent)->sort_by_aired(); });
}

Sort::~Sort()
{
    delete ui;
}

