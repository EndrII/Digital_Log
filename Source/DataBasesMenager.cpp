#include "DataBasesMenager.h"

DataBasesMenager::DataBasesMenager(sqlDataBase *_bd, QWidget *ptr):
    QDialog(ptr)
{
    this->setModal(true);
    bd=_bd;
    QVBoxLayout *box=new QVBoxLayout();
    box->addWidget(new QLabel(ELanguage::getWord(DATA_BASES_LIST)));
    ListdataBases =new QTableView();
    ListdataBases->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ListdataBases->setModel(bd->getModel());
    bd->Query("show databases;");
    //ListdataBases->setHidden();
    box->addWidget(ListdataBases);
    QHBoxLayout *line=new QHBoxLayout();
    Cancle=new QPushButton(ELanguage::getWord(BUTTON_CANCLE));
    connect(Cancle,SIGNAL(clicked(bool)),SLOT(cancleClick(bool)));
    line->addWidget(Cancle);

    Delete=new QPushButton(ELanguage::getWord(BUTTON_DELETE));
    connect(Delete,SIGNAL(clicked(bool)),SLOT(deleteClick(bool)));
    line->addWidget(Delete);

    Create=new QPushButton(ELanguage::getWord(BUTTON_CREATE));
    connect(Create,SIGNAL(clicked(bool)),SLOT(createClick(bool)));
    line->addWidget(Create);

    Ok=new QPushButton(ELanguage::getWord(BUTTON_OK));
    connect(Ok,SIGNAL(clicked(bool)),SLOT(okClick(bool)));
    line->addWidget(Ok);
    box->addLayout(line);
    this->setLayout(box);
}
void DataBasesMenager::cancleClick(bool){
    this->close();
}
void DataBasesMenager::white(bool b){
    Ok->setEnabled(b);
    Create->setEnabled(b);
    Delete->setEnabled(b);
    Cancle->setEnabled(b);
}
void DataBasesMenager::deleteClick(bool){
    if(!QMessageBox::question(this,ELanguage::getWord(WARNING),
                             ELanguage::getWord(DELETE_DATABASE_MSG),
                              ELanguage::getWord(YES), ELanguage::getWord(NO))){
        white(false);
        for(QModelIndex index :ListdataBases->selectionModel()->selectedRows()){
            bd->Query("drop database "+ListdataBases->model()->data(index).toString());
        }
        bd->Query("show databases;");
        white(true);
    }
}
void DataBasesMenager::createClick(bool){
    white(false);
    bd->createDB(QInputDialog::getText(this,ELanguage::getWord(INPUT_BD_NAME),ELanguage::getWord(INPUT_BD_NAME_MORE)));
    bd->Query("show databases;");
    white(true);
}
void DataBasesMenager::okClick(bool){
    if(ListdataBases->selectionModel()->selectedRows().empty()){
        QMessageBox::information(this,ELanguage::getWord(WARNING),ELanguage::getWord(BD_NO_SELECTED));
        return ;
    }
    bd->openDB(ListdataBases->model()->data(ListdataBases->selectionModel()->selectedRows()[0]).toString());
    if(!bd->Query_no_update("describe prefix;")){
        QMessageBox::information(this,ELanguage::getWord(WARNING),ELanguage::getWord(BD_NO_ADUCATION));
    }else{
        this->close();
    }
    bd->Query("show databases;");
}
DataBasesMenager::~DataBasesMenager(){

}
