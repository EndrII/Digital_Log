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
    times=new QComboBox();
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
    list->addWidget(times);
    list->addWidget(print_);
    list->addWidget(change);
    box->addLayout(list);
    box->addWidget(table);
    this->setLayout(box);
    connect(bd,SIGNAL(ChangedBD()),this,SLOT(updateGroups()));
    connect(print_,SIGNAL(clicked(bool)),this,SLOT(PrintClick()));
    connect(change,SIGNAL(clicked(bool)),this,SLOT(changeClick()));
    connect(times,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
    connect(groups,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
}
void StarastaMode::updateGroups(){
    ComboWrite();
}
void StarastaMode::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Return){
        bool ok;
        short indexRow=table->currentIndex().row();
        QString tempName=model->data(model->index(indexRow,0)).toString();
        int tempData=QInputDialog::getInt(this,ELanguage::getWord(ENTER_VALUE),ELanguage::getWord(ENTER_FROM)+
                                          " "+tempName,
                                          0,0,56,1,&ok);
        if(ok){
            QString temp_data;
            qyer->exec("select Даты from времяПропуски");
            qyer->seek(qyer->size()-1);
            temp_data=qyer->value(0).toString().replace('-','_');
            qyer->exec("UPDATE "+groups->currentText()+" SET "+temp_data+"="+QString::number(tempData)+" "
                       "WHERE ФИО='"+tempName+"'");
            qyer->exec("call showGroup('"+groups->currentText()+"')");
            model->setQuery(*qyer);
            table->selectRow(indexRow+1);
        }
    }
}
void StarastaMode::PrintClick(){
}
void StarastaMode::changeClick(){
}
void StarastaMode::GroupListChanged(int){
    qyer->exec("call showGroup('"+groups->currentText()+"')");
    model->setQuery(*qyer);
}
void StarastaMode::ComboWrite(){
    times->clear();
    groups->clear();

    times->addItems(bd->getDateListU());
    groups->addItem("*");
    groups->addItems(bd->getGroupList());
}

void StarastaMode::Editing(){

}
void StarastaMode::createContextMenu(){
    clearFilter=new QAction("Очистить фильтры");
    clearFilter->setStatusTip("Удалит все фильтры из таблицы");
    connect(clearFilter,SIGNAL(triggered(bool)),this,SLOT(clearFilterClick(bool)));

    alfavit=new QAction("По алфавиту");
    alfavit->setStatusTip("Отсортирует таблицу по алфавиту");
    connect(alfavit,SIGNAL(triggered(bool)),this,SLOT(alfavitClick(bool)));

    maxtomin=new QAction("По убыванию");
    maxtomin->setStatusTip("Отсортирует таблицу по убыванию");
    connect(maxtomin,SIGNAL(triggered(bool)),this,SLOT(maxtominClick(bool)));

    mintomax=new QAction("По возрастанию");
    mintomax->setStatusTip("Отсортирует таблицу по возрастанию");
    connect(mintomax,SIGNAL(triggered(bool)),this,SLOT(mintomaxClick(bool)));

    curentTime=new QAction("Выбор временного интервала");
    curentTime->setStatusTip("Выбрать за какой месяц отображать данные");
    connect(curentTime,SIGNAL(triggered(bool)),this,SLOT(curentTimeClick(bool)));

    PrintHTML=new QAction("Сохронить отчёт в HTML");
    PrintHTML->setStatusTip("Сохронить отчёт в HTML");
    connect(PrintHTML,SIGNAL(triggered(bool)),this,SLOT(ClickPrintHTML(bool)));

    PrintPDF=new QAction("Сохронить отчёт в PDF");
    PrintPDF->setStatusTip("Сохронить отчёт в PDF");
    connect(PrintPDF,SIGNAL(triggered(bool)),this,SLOT(ClickPrintPDF(bool)));

    onlySumm=new QAction("Только сумма");
    onlySumm->setStatusTip("Покажет только общее количество пропусков за всё время");
    connect(onlySumm,SIGNAL(triggered(bool)),this,SLOT(ClickOnlySumm(bool)));

}
void StarastaMode::ClickOnlySumm(bool){
}
void StarastaMode::ClickPrintPDF(bool){
}
void StarastaMode::ClickPrintHTML(bool){
}
void StarastaMode::clearFilterClick(bool){
}
void StarastaMode::alfavitClick(bool){
}
void StarastaMode::maxtominClick(bool){
}
void StarastaMode::mintomaxClick(bool){
}
void StarastaMode::curentTimeClick(bool){
}

void StarastaMode::contextMenuEvent(QContextMenuEvent *event){
    if(change->isEnabled()){
        QMenu menu(this);
        menu.addAction(PrintHTML);
        menu.addAction(PrintPDF);
        menu.addAction(alfavit);
        menu.addAction(maxtomin);
        menu.addAction(mintomax);
        menu.addAction(curentTime);
        menu.addAction(onlySumm);
        menu.addAction(clearFilter);
        menu.exec(event->globalPos());
    }
}
StarastaMode::~StarastaMode(){

}
