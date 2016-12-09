/****************************************************************************
**
** Copyright (C) 2016 A.N.Yankovich.
** Contact: https://vk.com/id223266985
**
** $QT_BEGIN_LICENSE:GPLv3$
** This file is distributed under the GPLv3 license.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "starastamode.h"

StarastaMode::StarastaMode(sqlDataBase *bd_, QWidget *parent) : QWidget(parent)
{
    bd=bd_;
    groups=new QComboBox();
    beginRange=new QDateEdit();
    beginRange->setDisplayFormat("dd.MM.yyyy");
    endRange=new QDateEdit(QDate::currentDate());
    endRange->setDisplayFormat("dd.MM.yyyy");
    print_=new QPushButton(ELanguage::getWord(PRINT));
    print_->setShortcut(Qt::Key_Print);
    change=new QPushButton(ELanguage::getWord(LIMIT));
    table=new QTableView(this);
    qyer=bd->registration();
    model=new QSqlQueryModel();
    model->setQuery(*qyer);
    table->setModel(model);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->setMinimumSize(800,480);
    change->setEnabled(false);
    createContextMenu();
    QVBoxLayout *box=new QVBoxLayout();
    QHBoxLayout *list=new QHBoxLayout();
    list->addWidget(groups);
    list->addWidget(new QLabel(ELanguage::getWord(DATE_RANGE)+" "+ELanguage::getWord(BEGIN)+":"));
    list->addWidget(beginRange);
    list->addWidget(new QLabel(ELanguage::getWord(END)+":"));
    list->addWidget(endRange);
    list->addWidget(print_);
    list->addWidget(change);
    box->addLayout(list);
    box->addWidget(table);
    this->setLayout(box);
    connect(bd,SIGNAL(ChangedBD()),this,SLOT(updateGroups()));
    connect(print_,SIGNAL(clicked(bool)),this,SLOT(PrintClick()));
    connect(change,SIGNAL(clicked(bool)),this,SLOT(changeClick()));
    connect(beginRange,SIGNAL(dateChanged(QDate)),SLOT(updateTable()));
    connect(endRange,SIGNAL(dateChanged(QDate)),SLOT(updateTable()));
    connect(groups,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
}
void StarastaMode::updateTable(){
    QString q="select ФИО as '"+ELanguage::getWord(NAME)+"'";
    for(QString i:bd->getDateListP(beginRange->date(),endRange->date())){
        q+=","+i.replace('-','_')+ " as '"+QDate::fromString(i,"yyyy-MM-dd").toString("dd.MMMM.yyyy")+"'";
    }
    q+=",sum as '"+ELanguage::getWord(SUMM)+"' from "+groups->currentText();
    qyer->exec(q);

    model->setQuery(*qyer);
}
void StarastaMode::updateGroups(){
    ComboWrite();
}
void StarastaMode::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Return||event->key()==Qt::Key_Enter){
        bool ok;
        short indexRow=table->currentIndex().row();
        short indexColumn=table->currentIndex().column();
        QString tempName=model->data(model->index(indexRow,0)).toString();
        int tempData=QInputDialog::getInt(this,ELanguage::getWord(ENTER_VALUE),ELanguage::getWord(ENTER_FROM)+
                                          " "+tempName,
                                          0,0,56,1,&ok);
        if(ok){
            QString temp_data;
            temp_data=QDate::fromString(model->headerData(indexColumn,Qt::Horizontal).toString(),"dd.MMMM.yyyy").toString("yyyy_MM_dd");
            qyer->exec("UPDATE "+groups->currentText()+" SET "+temp_data+"="+QString::number(tempData)+" "
                       "WHERE ФИО='"+tempName+"'");
            bd->sumCount(groups->currentText());
            updateTable();
            if(++indexRow>=model->rowCount()){
                indexColumn++;
                indexRow=0;
            }
            table->selectionModel()->setCurrentIndex(model->index(indexRow,indexColumn),QItemSelectionModel::Select);
        }
    }
}
void StarastaMode::PrintClick(){
    ( new PrinterDialog(table,this))->exec();
}
void StarastaMode::changeClick(){
}
void StarastaMode::GroupListChanged(int){
    qyer->exec("call update_table('"+groups->currentText()+"','времяПропуски')");
    updateTable();
}
void StarastaMode::ComboWrite(){
    groups->clear();
    groups->addItems(bd->getGroupList());
}
void StarastaMode::createContextMenu(){
}


void StarastaMode::contextMenuEvent(QContextMenuEvent *event){
    if(change->isEnabled()){
        QMenu menu(this);
        menu.exec(event->globalPos());
    }
}
StarastaMode::~StarastaMode(){

}
