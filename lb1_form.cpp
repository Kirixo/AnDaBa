#include "lb1_form.h"
#include "lb1_editform.h"
#include "ui_lb1_form.h"
#include "addepisodeform.h"

LB1_form::LB1_form(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LB1_form)
{
    ui->setupUi(this);

    connect(ui->actionExit, &QAction::triggered, this, &LB1_form::closeApplication);
    connect(ui->actionQueryEdit, &QAction::triggered, this, &LB1_form::openQueryEdit);
    connect(ui->actionAdd, &QAction::triggered, this, &LB1_form::addRow);
    connect(ui->actionUpdate, &QAction::triggered, this, &LB1_form::uptadeTable);
    connect(ui->actionDelete, &QAction::triggered, this, &LB1_form::deleteTable);
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("andaba");
    if(db.open()){
        qDebug() << "andaba opened";
        //showDataBase("titles");
        //QString var = QString("titles");
        connect(ui->actiontitles, &QAction::triggered, this, [=]() { showDataBase("anime"); });
        connect(ui->actiongenres, &QAction::triggered, this, [=]() { showDataBase("genres"); });
        connect(ui->actiontitle_genre, &QAction::triggered, this, [=]() { showDataBase("anime_genre"); });

        ui->tableView_2->hide();
        ui->tableView_3->hide();
//        tableModel1 = new QSqlTableModel();
//        tableModel1->setTable("titles");
//        tableModel1->select();
//        tableModel1->setEditStrategy(QSqlTableModel::OnManualSubmit);
//        ui->tableView_3->setModel(tableModel1);

//        tableModel = new QSqlTableModel();
//        tableModel->setTable("genres");
//        tableModel->select();
//        tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
//        ui->tableView->setModel(tableModel1);

//        QSqlQuery query( "SELECT titles.id, titles.name, genres.id, genres.name "
//                        "FROM title_genre "
//                        "JOIN titles ON title_genre.title_id = titles.id "
//                        "JOIN genres ON title_genre.genre_id = genres.id");
//        queryModel->setQuery(query);
//        ui->tableView_2->setModel(queryModel);



    } else {
        qDebug() << "db error";
    }

    connect(ui->tableView, &QTableView::doubleClicked, this, &LB1_form::editRecord);
    connect(ui->tableView, &QTableView::doubleClicked, this, &LB1_form::editRecord);
}

LB1_form::~LB1_form()
{
    delete tableModel;
    delete queryModel;
    delete ui;
}

void LB1_form::showDataBase(QString dbName){
    tableModel = new QSqlTableModel();
    tableModel->setTable(dbName);
    if(dbName == "anime_genre"){
        QSqlQuery query( "SELECT anime.anime_id, anime.anime_name, genres.genre_id, genres.genre_name\n"
                         "FROM anime_genre\n"
                         "JOIN anime ON anime_genre.anime_id = anime.anime_id\n"
                         "JOIN genres ON anime_genre.genre_id = genres.genre_id");
        queryModel->setQuery(query);
        ui->tableView->setModel(queryModel);

    } else {
        tableModel->select();
        //tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        ui->tableView->setModel(tableModel);
    }
}

void LB1_form::editRecord(const QModelIndex &index)
{
    if(tableModel->tableName() == "anime") {
        if(index.column() > 0) {
            return;
        }
        AddEpisodeForm *addEpisodeForm = new AddEpisodeForm(this);
        addEpisodeForm->setRecord(tableModel->data(index).toInt());
        addEpisodeForm->show();
        return;
    }
    if(tableModel->tableName() != "genres") {
        return;
    }
    QSqlRecord record = tableModel->record(index.row());

    LB1_editForm *editForm = new LB1_editForm(this);
    editForm->setRecord(record);

    connect(editForm, &LB1_editForm::dataSaved, this, &LB1_form::reloadData);

    editForm->show();
}

void LB1_form::reloadData()
{
    tableModel->select();
}

void LB1_form::openQueryEdit()
{
    queryForm = new LB2_form();
    queryForm->show();
}

void LB1_form::deleteTable()
{

}

void LB1_form::uptadeTable()
{

}

void LB1_form::addRow()
{
    addForm = new addRowForm();
    addForm->show();

}

void LB1_form::closeApplication(){
    close();
}
