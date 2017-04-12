#include "PredmeMenager.h"
PredmeMenager::PredmeMenager(sqlDataBase *bd_,const QString &gr, QWidget *parent) : QDialog(parent)
{
    bd=bd_;
    this->setModal(true);
    group=gr;
    querL=bd->registration();
    modelL=new QSqlQueryModel();
    modelL->setQuery(*querL);
    querR=bd->registration();
    modelR=new QSqlQueryModel();
    modelR->setQuery(*querR);
    tableGlobal=new QTableView();
    tableGlobal->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableGlobal->setModel(modelL);
    tableLocal=new QTableView();
    tableLocal->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableLocal->setModel(modelR);

    remove=new QPushButton(ELanguage::getWord(BUTTON_PREDMET_REMOVE));
    connect(remove,SIGNAL(clicked(bool)),this,SLOT(del()));
    add=new QPushButton(ELanguage::getWord(BUTTON_CREATE));
    connect(add,SIGNAL(clicked(bool)),this,SLOT(add_()));
    toGroup=new QPushButton(ELanguage::getWord(PREDMET_TO_GROUP));
    connect(toGroup,SIGNAL(clicked(bool)),this,SLOT(toGr()));
    QHBoxLayout *box=new QHBoxLayout();
    QVBoxLayout *list=new QVBoxLayout();
    list->addWidget(add);
    list->addWidget(tableGlobal);
    box->addLayout(list);
    list=new QVBoxLayout();
    list->addWidget(toGroup);
    list->addWidget(remove);
    box->addLayout(list);

    box->addWidget(tableLocal);
    //box->addWidget(tableGroub);
    this->setLayout(box);
    createContextMenu();
    querL->exec("select Наименование from  предметы");
    modelL->setQuery(*querL);
    querR->exec("select предмет from P_"+group);
    modelR->setQuery(*querR);
}
void PredmeMenager::Renam(bool){
    for(QModelIndex index :tableGlobal->selectionModel()->selectedRows()){
        bd->Query_no_update("update предметы "
                            "set Наименование='"+QInputDialog::getText(this,ELanguage::getWord(BUTTON_RENAME),
                                                              ELanguage::getWord(RENAME_PREDMET_MSG))+
                            "' where Наименование='"+tableGlobal->model()->data(index).toString()+"'");
    }
    querL->exec("select Наименование from  предметы");
    modelL->setQuery(*querL);
    querR->exec("select предмет from P_"+group);
    modelR->setQuery(*querR);
}
void PredmeMenager::del(){
    for(QModelIndex index :tableLocal->selectionModel()->selectedRows()){
        bd->removePredmetGroup(group,tableLocal->model()->data(index).toString());
    }
    querR->exec("select предмет from P_"+group);
    modelR->setQuery(*querR);
}
void PredmeMenager::add_(){
    bd->createPredmet(QInputDialog::getText(this,ELanguage::getWord(NEW_PREDMET_TITLE),
                                          ELanguage::getWord(NEW_PREDMET_ABOUT)));
    querL->exec("select Наименование from  предметы");
    modelL->setQuery(*querL);
}
void PredmeMenager::toGr(){
    for(QModelIndex index :tableGlobal->selectionModel()->selectedRows()){
        bd->addPredmetGroup(group,tableGlobal->model()->data(index).toString());
    }
    querR->exec("select предмет from P_"+group);
    modelR->setQuery(*querR);
}
void PredmeMenager::createContextMenu(){
    deletePredmet=new QAction(ELanguage::getWord(BUTTON_DELETE));
    deletePredmet->setStatusTip(ELanguage::getWord(DELETE_PREDMET_MSG));
    connect(deletePredmet,SIGNAL(triggered(bool)),this,SLOT(dell(bool)));

    renamePredmet=new QAction(ELanguage::getWord(BUTTON_RENAME));
    renamePredmet->setStatusTip(ELanguage::getWord(RENAME_PREDMET_MSG));
    connect(renamePredmet,SIGNAL(triggered(bool)),this,SLOT(Renam(bool)));
}
void PredmeMenager::dell(bool){
    if(!QMessageBox::question(this,ELanguage::getWord(WARNING),
                             ELanguage::getWord(DELETE_PREDMET_MSG),
                              ELanguage::getWord(YES), ELanguage::getWord(NO))){
        for(QModelIndex index :tableGlobal->selectionModel()->selectedRows()){
            bd->deletePredmet(tableGlobal->model()->data(index).toString());
        }
        querL->exec("select Наименование from  предметы");
        modelL->setQuery(*querL);
        querR->exec("select предмет from P_"+group);
        modelR->setQuery(*querR);
    }
}
void PredmeMenager::contextMenuEvent(QContextMenuEvent *event){
    QMenu menu(this);
    menu.addAction(renamePredmet);
    menu.addAction(deletePredmet);
    menu.exec(event->globalPos());
}
PredmeMenager::~PredmeMenager(){
    delete querL;
    delete modelL;
    delete querR;
    delete modelR;
    delete deletePredmet;
    delete renamePredmet;
}

