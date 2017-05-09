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

    table->setModel(model);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    showCritikal=new QCheckBox(ELanguage::getWord(SHOW_CRITICAL));showCritikal->setCheckState(Qt::Checked);
    showWarning=new QCheckBox(ELanguage::getWord(SHOW_WARNING));showWarning->setCheckState(Qt::Checked);
    showNormal=new QCheckBox(ELanguage::getWord(SHOW_NORMAL));showNormal->setCheckState(Qt::Checked);
    //sort=new QComboBox();
    this->setMinimumSize(800,480);
    createContextMenu();
    QVBoxLayout *box=new QVBoxLayout();
    QHBoxLayout *list=new QHBoxLayout();
    QVBoxLayout *stack=new QVBoxLayout();
    stack->addWidget(showNormal);
    stack->addWidget(showWarning);
    stack->addWidget(showCritikal);
    list->addLayout(stack);
    stack=new QVBoxLayout();
    stack->addWidget(new QLabel(ELanguage::getWord(BUTTON_GROUP)));
    stack->addWidget(groups);
    list->addLayout(stack);
    stack=new QVBoxLayout();
    stack->addWidget(new QLabel(ELanguage::getWord(DATE_RANGE)));
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(BEGIN)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(beginRange);
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(END)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(endRange);
    list->addLayout(stack);
    stack=new QVBoxLayout();
    stack->addWidget(new QLabel(ELanguage::getWord(LIMIT_TITLE)));
    stack->addWidget(limit);
    stack->addWidget(print_);
    list->addLayout(stack);
    box->addLayout(list);

    box->addWidget(table);
    bar=new QProgressBar();
    bar->setMinimum(0);
    bar->setVisible(false);
    this->setLayout(box);
    connect(bd,SIGNAL(ChangedBD()),this,SLOT(updateGroups()));
    connect(print_,SIGNAL(clicked(bool)),this,SLOT(PrintClick()));
    connect(limit,SIGNAL(valueChanged(int)),this,SLOT(limitChange(int)));
    connect(beginRange,SIGNAL(dateChanged(QDate)),SLOT(updateTable()));
    connect(endRange,SIGNAL(dateChanged(QDate)),SLOT(updateTable()));
    connect(groups,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
    connect(showNormal,SIGNAL(stateChanged(int)),this,SLOT(updateTable()));
    connect(showWarning,SIGNAL(stateChanged(int)),this,SLOT(updateTable()));
    connect(showCritikal,SIGNAL(stateChanged(int)),this,SLOT(updateTable()));
}
void StarastaMode::updateTable(const short &index,bool DESC){
    disconnect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this
            ,SLOT(Enter(QModelIndex,QModelIndex,QVector<int>)));
    QString q=QString("select s.name,point,_value from (dates  d , students s) LEFT JOIN attendance a ON (d.id=a._date and a.student=s.id) where"
              " d.point>'%0' and d.point<'%1' and d.date_group=1 and s._group=(select id from groups where name='%2')").
            arg(beginRange->date().toString("yyyy-MM-dd")).
            arg(endRange->date().toString("yyyy-MM-dd")).
            arg(groups->currentText());
    qDebug()<<q;
    bd->transformQuery(q,"point","name","_value",model);
    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this
            ,SLOT(Enter(QModelIndex,QModelIndex,QVector<int>)));
}
void StarastaMode::updateGroups(){
    ComboWrite();
}
void StarastaMode::keyPressEvent(QKeyEvent *event){
}
void StarastaMode::PrintClick(){
    ( new PrinterDialog(table,this))->exec();
}
void StarastaMode::limitChange(int i){
    model->setLimit(i);
    table->repaint();
    bd->setGroupLimit(groups->currentText(),i);
}
void StarastaMode::GroupListChanged(int){
    updateTable();
    limit->setValue(bd->getGroupLimit(groups->currentText()));
    model->setLimit(limit->value());
    table->repaint();

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

}
void StarastaMode::Enter(QModelIndex mod, QModelIndex, QVector<int> role){
    Write_Line_Params temp;
    temp.InToTable="attendance";
    temp.columnInTo="student";
    temp.rowInTo="_date";
    temp.valueInTo="_value";
    temp.DescRowTable="students";
    temp.DescRowTableColumn="name";
    temp.DescColumnTable="dates";
    temp.DescColumnTableColumn="point";
    temp.dateType="1";
    temp.valueRow=model->headerData(mod.row(),Qt::Vertical).toString();
    temp.valueColumn=model->headerData(mod.column(),Qt::Horizontal).toString();
    temp.value=model->data(mod,Qt::DisplayRole).toString();
    qDebug()<<"WriteLine="<<bd->writeLine(temp);
}
void StarastaMode::filterUpdate(){

}
void StarastaMode::saveStateChanged(int i){
    save->setEnabled(i);
    save->setText(ELanguage::getWord(BUTTON_SAVE)+"("+QString::number(i)+")");
    bar->setVisible(i);
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
        menu.exec(event->globalPos());
}
StarastaMode::~StarastaMode(){

}
