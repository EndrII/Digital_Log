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
//    save=new QPushButton(ELanguage::getWord(BUTTON_SAVE));
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
 //   stack->addWidget(save);
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
  //  box->addWidget(bar);
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
  //  connect((MySqlQueryColorModel*)model,SIGNAL(tableSaveProgress(int)),bar,SLOT(setValue(int)));
   // connect(save,SIGNAL(clicked(bool)),(MySqlQueryColorModel*)model,SLOT(save()));
   // connect((MySqlQueryColorModel*)model,SIGNAL(SavesChanged(int)),SLOT(saveStateChanged(int)));
   // connect((MySqlQueryColorModel*)model,SIGNAL(startSave(int)),bar,SLOT(setMaximum(int)));
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

    q+=" from "+groups->currentText();
    if(!showNormal->isChecked()){
        q+=" where sum >= "+QString::number(limit->value()/2);
    }
    if(!showWarning->isChecked()){
        q+=" where (sum <"+QString::number(limit->value()/2)+" or  sum>="+QString::number(limit->value())+")";
    }
    if(!showCritikal->isChecked()){
        q+=" where NOT sum>= "+QString::number(limit->value());
    }
    q+=" ORDER BY "+listColumnHaider[index]+((DESC)?" DESC ":"");
    ((MySqlQueryColorModel*)model)->setLimit(limit->value());
    qDebug()<<qyer->exec(q)<<q;
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
        QString temp_data,q;
        temp_data=QDate::fromString(model->headerData(indexColumn,Qt::Horizontal).toString(),"dd.MMMM.yyyy").toString("yyyy_MM_dd");
        q="UPDATE "+groups->currentText()+" SET "+temp_data+"="+QString::number(tempData)+" WHERE ФИО='"+tempName+"'";
        StackItem temp_i;
        temp_i.Group=groups->currentText();
        temp_i.qyer=q;
        qyer->exec(q);
        bd->sumCount(groups->currentText(),model->data(model->index(indexRow,0)).toString());
        updateTable();
        if(++indexRow>=model->rowCount()){
            indexColumn++;
            indexRow=0;
        }
        table->selectionModel()->setCurrentIndex(model->index(indexRow,indexColumn),QItemSelectionModel::Select);
    }
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
        menu.addAction(enter);
        menu.exec(event->globalPos());
}
StarastaMode::~StarastaMode(){

}
