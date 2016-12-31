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
    limit=new QSpinBox();
    limit->setMinimum(0);
    table=new QTableView(this);
    qyer=bd->registration();
    model=new MySqlQueryColorModel();
    model->setQuery(*qyer);
    table->setModel(model);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //sort=new QComboBox();
    this->setMinimumSize(800,480);
    createContextMenu();
    QVBoxLayout *box=new QVBoxLayout();
    QHBoxLayout *list=new QHBoxLayout();
    list->addWidget(groups);
    list->addWidget(new QLabel(ELanguage::getWord(DATE_RANGE)+" "+ELanguage::getWord(BEGIN)+":"));
    list->addWidget(beginRange);
    list->addWidget(new QLabel(ELanguage::getWord(END)+":"));
    list->addWidget(endRange);
    list->addWidget(print_);
    list->addWidget(limit);
    box->addLayout(list);
    box->addWidget(table);
    this->setLayout(box);
    connect(bd,SIGNAL(ChangedBD()),this,SLOT(updateGroups()));
    connect(print_,SIGNAL(clicked(bool)),this,SLOT(PrintClick()));
    connect(limit,SIGNAL(valueChanged(int)),this,SLOT(limitChange(int)));
    connect(beginRange,SIGNAL(dateChanged(QDate)),SLOT(updateTable()));
    connect(endRange,SIGNAL(dateChanged(QDate)),SLOT(updateTable()));
    connect(groups,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
    //connect(table->selectionModel(),SIGNAL(currentColumnChanged(QModelIndex,QModelIndex)),SLOT(sortChanged(QModelIndex,QModelIndex)));
}
void StarastaMode::updateTable(const short &index,bool DESC){
    QString q="select ФИО as '"+ELanguage::getWord(NAME)+"'";
    listColumnHaider.clear();
    listColumnHaider.push_back("ФИО");
    QStringList temp=bd->getDateListP(beginRange->date(),endRange->date());
    for(QString i:temp){
        q+=","+i.replace('-','_')+ " as '"+QDate::fromString(i,"yyyy-MM-dd").toString("dd.MMMM.yyyy")+"'";
        listColumnHaider.push_back(i.replace('-','_'));
    }
    if(temp.size()==bd->getDatePCount()){
        q+=",sum as '"+ELanguage::getWord(SUMM)+"'";
        listColumnHaider.push_back("sum");
    }
    q+=" from "+groups->currentText()+" ORDER BY "+listColumnHaider[index]+((DESC)?" DESC ":"");
    ((MySqlQueryColorModel*)model)->setLimit(limit->value());
    qyer->exec(q);
    model->setQuery(*qyer);
}
void StarastaMode::updateGroups(){
    ComboWrite();
}
void StarastaMode::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Return||event->key()==Qt::Key_Enter){
        Enter();
    }
}
void StarastaMode::PrintClick(){
    ( new PrinterDialog(table,this))->exec();
}
void StarastaMode::limitChange(int i){
    ((MySqlQueryColorModel*)model)->setLimit(i);
    table->repaint();
    bd->setGroupLimit(groups->currentText(),i);
}
void StarastaMode::GroupListChanged(int){
    qyer->exec("call update_table('"+groups->currentText()+"','времяПропуски')");
    updateTable();
}
void StarastaMode::ComboWrite(){
    groups->clear();
    groups->addItems(bd->getGroupList());
    limit->setValue(bd->getGroupLimit(groups->currentText()));
}
void StarastaMode::createContextMenu(){
    sortUP=new QAction(ELanguage::getWord(SORT_UP));
    connect(sortUP,SIGNAL(triggered(bool)),this,SLOT(sortTableU()));
    sortDOWN=new QAction(ELanguage::getWord(SORT_DOWN));
    connect(sortDOWN,SIGNAL(triggered(bool)),this,SLOT(sortTableD()));
    enter=new  QAction(ELanguage::getWord(ENTER));
    connect(enter,SIGNAL(triggered(bool)),this,SLOT(Enter()));

}
void StarastaMode::Enter(){
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
void StarastaMode::sortTableU(){
    updateTable(table->selectionModel()->currentIndex().column());
}
void StarastaMode::sortTableD(){
    updateTable(table->selectionModel()->currentIndex().column(),true);
}
void StarastaMode::contextMenuEvent(QContextMenuEvent *event){
        QMenu menu(this);
        menu.addAction(sortUP);
        menu.addAction(sortDOWN);
        menu.addAction(enter);
        menu.exec(event->globalPos());
}
StarastaMode::~StarastaMode(){

}
