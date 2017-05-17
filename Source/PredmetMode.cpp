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
#include "PredmetMode.h"

PredmetMode::PredmetMode(sqlDataBase *bd_, QWidget *parent) : QWidget(parent)
{
    bd=bd_;
    groups=new QComboBox();
    predmets=new QComboBox();
    times=new QComboBox();
    print_=new QPushButton(ELanguage::getWord(PRINT));
    print_->setShortcut(Qt::Key_Print);
    showCritikal=new QCheckBox(ELanguage::getWord(SHOW_CRITICAL));showCritikal->setCheckState(Qt::Checked);
    showWarning=new QCheckBox(ELanguage::getWord(SHOW_WARNING));showWarning->setCheckState(Qt::Checked);
    showNormal=new QCheckBox(ELanguage::getWord(SHOW_NORMAL));showNormal->setCheckState(Qt::Checked);
    limit_LC=new QSpinBox();
    limit_PC=new QSpinBox();
    limit_KRC=new QSpinBox();
    limit_RGRC=new QSpinBox();
    limit_LC->setMinimum(0);
    limit_PC->setMinimum(0);
    limit_KRC->setMinimum(0);
    limit_RGRC->setMinimum(0);
    table=new QTableView(this);
    prefix=new QComboBox();
    prefix->addItem(ELanguage::getWord(LABS_V));
    prefix->addItem(ELanguage::getWord(PRACT_V));
    prefix->addItem(ELanguage::getWord(KONTR_V));
    prefix->addItem(ELanguage::getWord(RGR_V));
    prefix->addItem(ELanguage::getWord(LABS_Z));
    prefix->addItem(ELanguage::getWord(KONTR_Z));
    prefix->addItem(ELanguage::getWord(RGR_Z));
    beginRange=new QDateEdit();
    beginRange->setDisplayFormat("dd.MM.yyyy");
    endRange=new QDateEdit(QDate::currentDate());
    endRange->setDisplayFormat("dd.MM.yyyy");
    beginRangeL=new QLabel(ELanguage::getWord(DATE_RANGE)+" "+ELanguage::getWord(BEGIN)+":");
    endRangeL=new QLabel(ELanguage::getWord(END)+":");
    qyer=bd->registration();
    model=new MySqlQueryColorModelU();
    table->setModel(model);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->setMinimumSize(800,480);
    createContextMenu();
    QVBoxLayout *box=new QVBoxLayout();
    QHBoxLayout *list=new QHBoxLayout();
    QVBoxLayout *stack=new QVBoxLayout();
    stack->addWidget(new QLabel(ELanguage::getWord(FILTERS)));
    stack->addWidget(showNormal);
    stack->addWidget(showWarning);
    stack->addWidget(showCritikal);
    list->addLayout(stack);

    stack=new QVBoxLayout();
    stack->addWidget(new QLabel(ELanguage::getWord(SELECTOR)));
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(BUTTON_GROUP)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(groups);
    list->addLayout(stack);

    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(PREDMET_MOD)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(predmets);
    list->addLayout(stack);


    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(DATE_MODE)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(times);
    list->addLayout(stack);

    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(WORK)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(prefix);
    list->addLayout(stack);

    stack=new QVBoxLayout();
    stack->addWidget(new QLabel(ELanguage::getWord(DATE_RANGE)));
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(BEGIN)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(beginRange);
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(END)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(endRange);
    stack->addWidget(print_);
    list->addLayout(stack);

    stack=new QVBoxLayout();
    stack->addWidget(new QLabel(ELanguage::getWord(PLAN_WORK_NOW)));
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(LC_WORKS)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(limit_LC);
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(PC_WORKS)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(limit_PC);
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(KRC_WORKS)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(limit_KRC);
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(RGRC_WORKS)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(limit_RGRC);
    list->addLayout(stack);
    box->addLayout(list);
    box->addWidget(table);
    this->setLayout(box);
    connect(bd,SIGNAL(ChangedBD()),this,SLOT(updateGroups()));
    connect(print_,SIGNAL(clicked(bool)),this,SLOT(PrintClick()));
    connect(limit_LC,SIGNAL(valueChanged(int)),this,SLOT(changed_limit_LC(int)));
    connect(limit_PC,SIGNAL(valueChanged(int)),this,SLOT(changed_limit_PC(int)));
    connect(limit_KRC,SIGNAL(valueChanged(int)),this,SLOT(changed_limit_KRC(int)));
    connect(limit_RGRC,SIGNAL(valueChanged(int)),this,SLOT(changed_limit_RGRC(int)));
    connect(times,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
    connect(predmets,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
    connect(groups,SIGNAL(currentIndexChanged(int)),this,SLOT(ComboWrite(int)));
    connect(prefix,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
    connect(beginRange,SIGNAL(dateChanged(QDate)),SLOT(updateTable()));
    connect(endRange,SIGNAL(dateChanged(QDate)),SLOT(updateTable()));
    connect(showNormal,SIGNAL(stateChanged(int)),this,SLOT(filterUpdate()));
    connect(showWarning,SIGNAL(stateChanged(int)),this,SLOT(filterUpdate()));
    connect(showCritikal,SIGNAL(stateChanged(int)),this,SLOT(filterUpdate()));
}
void PredmetMode::updateGroups(){
    groups->clear();
    groups->addItems(bd->getGroupList());
    ComboWrite(0);
}
void PredmetMode::PrintClick(){
    ( new PrinterDialog(table,this))->exec();
}
void PredmetMode::GroupListChanged(int){
    updateLimits();
    if(times->currentText()=="*"){
        updateTable(true);
        prefix->setEnabled(true);
        beginRange->setEnabled(true);
        beginRangeL->setEnabled(true);
        endRange->setEnabled(true);
        endRangeL->setEnabled(true);
    }else{
        updateTable(false);
        prefix->setEnabled(false);
        beginRange->setEnabled(false);
        beginRangeL->setEnabled(false);
        endRange->setEnabled(false);
        endRangeL->setEnabled(false);
    }
}
void PredmetMode::ComboWrite(int){
    predmets->clear();
    times->clear();
    times->addItem("*");
    times->addItems(bd->getDateListU());
    predmets->addItems(bd->getGroupPredmetsList(groups->currentText()));
    updateLimits();
    }
void PredmetMode::updateTable(){
    updateTable(true);
}
void PredmetMode::updateTable(bool isAll){
    if(isAll){
        model->setSummColumn(true);
        disconnect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this
                ,SLOT(Enter(QModelIndex,QModelIndex,QVector<int>)));
        QString q=QString("select s.name as 'row',t.point as 'column',_value from (work_groups  d , students s, subjects j, dates t)"
                          " LEFT JOIN performance a ON (t.id=a._date and d.id=a.work_group and a.student=s.id and a.subject=j.id) where "
                  "j.name='%0' and s._group=(select id from groups where name='%1') and d.name='%2' and t.point>'%3' and t.point<'%4'").
                arg(predmets->currentText()).
                arg(groups->currentText()).
                arg(prefix->currentText()).
                arg(beginRange->date().toString("yyyy-MM-dd")).
                arg(endRange->date().toString("yyyy-MM-dd"));
        bd->transformQuery(q,"column","row","_value",model);
        table->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);
        model->recalc();
        connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this
                ,SLOT(Enter(QModelIndex,QModelIndex,QVector<int>)));
        switch(prefix->currentIndex()){
        case 0:model->setLimit(limit_LC->value(),LC_,true);break;
        case 1:model->setLimit(limit_PC->value(),LC_,true);break;
        case 2:model->setLimit(limit_KRC->value(),LC_,true);break;
        case 3:model->setLimit(limit_RGRC->value(),LC_,true);break;
        case 4:model->setLimit(limit_LC->value(),LC_,true);break;
        case 5:model->setLimit(limit_KRC->value(),LC_,true);break;
        case 6:model->setLimit(limit_RGRC->value(),LC_,true);break;
        default: model->setLimit();
        }
    }else{
        model->setSummColumn(false);
        disconnect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this
                ,SLOT(Enter(QModelIndex,QModelIndex,QVector<int>)));
        QString q=QString("select s.name as 'row',d.name as 'column',_value from (work_groups  d , students s, subjects j, dates t)"
                          " LEFT JOIN performance a ON (t.id=a._date and d.id=a.work_group and a.student=s.id and a.subject=j.id) where "
                  "j.name='%0' and s._group=(select id from groups where name='%1') and t.point<='%2'").
                arg(predmets->currentText()).
                arg(groups->currentText()).
                arg(times->currentText());
        qDebug()<<q;
        bd->transformQuery(q,"column","row","_value",model);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        model->recalc();
        connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this
                ,SLOT(Enter(QModelIndex,QModelIndex,QVector<int>)));
        model->setLimit(limit_LC->value(),LC_);
        model->setLimit(limit_PC->value(),PC_);
        model->setLimit(limit_KRC->value(),KRC_);
        model->setLimit(limit_RGRC->value(),RGRC_);
    }
}
void PredmetMode::updateLimits(){
    limit_LC->setValue(bd->getGroupLimit(groups->currentText(),_LC_,predmets->currentText()));
    limit_PC->setValue(bd->getGroupLimit(groups->currentText(),_PC_,predmets->currentText()));
    limit_KRC->setValue(bd->getGroupLimit(groups->currentText(),_KRC_,predmets->currentText()));
    limit_RGRC->setValue(bd->getGroupLimit(groups->currentText(),_RGRC_,predmets->currentText()));
}
void PredmetMode::Enter(QModelIndex mod,QModelIndex,QVector<int>){
    Write_Line_ParamsU temp;
    temp.student=model->headerData(mod.row(),Qt::Vertical).toString();
    temp.subject=predmets->currentText();
    temp.value=model->data(mod,Qt::DisplayRole).toString();
    if(times->currentText()=="*"){
        temp.work_group=prefix->currentText();
        temp.date=model->headerData(mod.column(),Qt::Horizontal).toString();
    }
    else{
        temp.date=times->currentText();
        temp.work_group=model->headerData(mod.column(),Qt::Horizontal).toString();
    }
}
void PredmetMode::sortTableU(){
    table->sortByColumn(table->selectionModel()->currentIndex().column(),Qt::AscendingOrder);
}
void PredmetMode::sortTableD(){
    table->sortByColumn(table->selectionModel()->currentIndex().column(),Qt::DescendingOrder);
}
void PredmetMode::filterUpdate(){
    for(int i=0;i<model->rowCount();i++){
        switch (model->checkLimit(model->index(i,model->columnCount()-1))) {
        case filter::normal:
            if(showNormal->isChecked())
                table->showRow(i);
            else
                table->hideRow(i);
            break;
        case filter::warning:
            if(showWarning->isChecked())
                table->showRow(i);
            else
                table->hideRow(i);
            break;
        case filter::critical:
            if(showCritikal->isChecked())
                table->showRow(i);
            else
                table->hideRow(i);
            break;
        default:
            if(showNormal->isChecked())
                table->showRow(i);
            else
                table->hideRow(i);
            break;
        }
    }
}
void PredmetMode::createContextMenu(){
    sortUP=new QAction(ELanguage::getWord(SORT_UP));
    connect(sortUP,SIGNAL(triggered(bool)),this,SLOT(sortTableU()));
    sortDOWN=new QAction(ELanguage::getWord(SORT_DOWN));
    connect(sortDOWN,SIGNAL(triggered(bool)),this,SLOT(sortTableD()));
}
void PredmetMode::changed_limit_LC(int i){
    ((MySqlQueryColorModelU*)model)->setLimit(i,LC_,times->currentText()=="*");
    table->repaint();
}
void PredmetMode::changed_limit_PC(int i){
    ((MySqlQueryColorModelU*)model)->setLimit(i,PC_,times->currentText()=="*");
    table->repaint();
}
void PredmetMode::changed_limit_KRC(int i){
    ((MySqlQueryColorModelU*)model)->setLimit(i,KRC_,times->currentText()=="*");
    table->repaint();
}
void PredmetMode::changed_limit_RGRC(int i){
    ((MySqlQueryColorModelU*)model)->setLimit(i,RGRC_,times->currentText()=="*");
    table->repaint();
}
void PredmetMode::contextMenuEvent(QContextMenuEvent *event){
    QMenu menu(this);
    if(!times->currentIndex()){
        menu.addAction(sortUP);
        menu.addAction(sortDOWN);
    }else{
        menu.addAction(enter);
    }
    menu.exec(event->globalPos());
}
PredmetMode::~PredmetMode(){

}
