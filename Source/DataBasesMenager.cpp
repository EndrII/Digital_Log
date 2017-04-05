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
    model= new QStandardItemModel(0,1);
    model->setHeaderData(0,Qt::Horizontal,ELanguage::getWord(EDATA_BASES));
    ListdataBases->setModel(model);
    showDataBases();
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
void DataBasesMenager::showDataBases(){
    bd->Query("show databases;");
    while(bd->getQuery()->next()){
        if(bd->getQuery()->value(0).toString()!="information_schema"&&
                bd->getQuery()->value(0).toString()!="mysql"&&
                bd->getQuery()->value(0).toString()!="performance_schema"&&
                bd->getQuery()->value(0).toString()!="sys"){
            model->insertRow(0);
            model->setData(model->index(0,0),bd->getQuery()->value(0),Qt::EditRole);
        }

    }

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
            model->removeRow(index.row());
        }
        white(true);
    }
}
void DataBasesMenager::createClick(bool){
    white(false);
    QString temp= QInputDialog::getText(this,ELanguage::getWord(INPUT_BD_NAME),ELanguage::getWord(INPUT_BD_NAME_MORE));
    if(bd->createDB(temp)){
        model->insertRow(0);
        model->setData(model->index(0,0),temp,Qt::EditRole);
    }
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
    showDataBases();
}
DataBasesMenager::~DataBasesMenager(){

}
