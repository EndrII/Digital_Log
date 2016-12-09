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
    change=new QPushButton(ELanguage::getWord(LIMIT));
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
    //table->setFocusPolicy(Qt::NoFocus);
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
    list->addWidget(predmets);
    list->addWidget(times);
    list->addWidget(prefix);
    list->addWidget(beginRangeL);
    list->addWidget(beginRange);
    list->addWidget(endRangeL);
    list->addWidget(endRange);
    list->addWidget(print_);
    list->addWidget(change);
    box->addLayout(list);
    box->addWidget(table);
    this->setLayout(box);
    connect(bd,SIGNAL(ChangedBD()),this,SLOT(updateGroups()));
    connect(print_,SIGNAL(clicked(bool)),this,SLOT(PrintClick()));
    connect(change,SIGNAL(clicked(bool)),this,SLOT(changeClick()));
    connect(times,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
    connect(predmets,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
    connect(groups,SIGNAL(currentIndexChanged(int)),this,SLOT(ComboWrite(int)));
    connect(prefix,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
    connect(beginRange,SIGNAL(dateChanged(QDate)),SLOT(updateTable()));
    connect(endRange,SIGNAL(dateChanged(QDate)),SLOT(updateTable()));

}
void PredmetMode::updateGroups(){
    groups->clear();
    groups->addItems(bd->getGroupList());
    ComboWrite(0);
}
void PredmetMode::keyPressEvent(QKeyEvent *event){
    if(times->currentText()!="*"&&(event->key()==Qt::Key_Return||event->key()==Qt::Key_Enter)){
        bool ok;
        short indexRow=table->currentIndex().row();
        short indexColumn=table->currentIndex().column();
        QString tempName=model->data(model->index(indexRow,0)).toString();
        int tempData=QInputDialog::getInt(this,ELanguage::getWord(ENTER_VALUE),ELanguage::getWord(ENTER_FROM)+
                                          "("+model->headerData(indexColumn,Qt::Horizontal).toString()+") "+
                                          tempName,0,0,56,1,&ok);
        if(ok){
            QStringList prefix;
            prefix<<"NONE"<<"LC_"<<"PC_"<<"KRC_"<<"RGRC_"<<"LD_"<<"KRD_"<<"RGRD_";
            QString name=predmets->currentText()+groups->currentText();
            QString time=times->currentText().replace('-','_');
            qyer->exec("UPDATE "+prefix[table->currentIndex().column()]+name+" "
                       "SET "+time+"="+QString::number(tempData)+" WHERE ФИО='"+tempName+"'");
            bd->sumCount(groups->currentText(),predmets->currentText(),indexColumn-1);
            updateTable(name,time);
            if(++indexRow>=model->rowCount()){
                indexColumn++;
                indexRow=0;
            }
            table->selectionModel()->setCurrentIndex(model->index(indexRow,indexColumn),QItemSelectionModel::Select);
        }
    }
}
void PredmetMode::PrintClick(){
    ( new PrinterDialog(table,this))->exec();
}
void PredmetMode::GroupListChanged(int){
    QString name=predmets->currentText()+groups->currentText();
    QString time=times->currentText().replace('-','_');
    qDebug()<<(qyer->exec("call update_table('LC_"+name+"','времяУроки')")&&
              qyer->exec("call update_table('PC_"+name+"','времяУроки')")&&
              qyer->exec("call update_table('KRC_"+name+"','времяУроки')")&&
              qyer->exec("call update_table('RGRC_"+name+"','времяУроки')")&&
              qyer->exec("call update_table('LD_"+name+"','времяУроки')")&&
              qyer->exec("call update_table('KRD_"+name+"','времяУроки')")&&
              qyer->exec("call update_table('RGRD_"+name+"','времяУроки')"));
    if(times->currentText()=="*"){
        updateTable(name);
        prefix->setEnabled(true);
        beginRange->setEnabled(true);
        beginRangeL->setEnabled(true);
        endRange->setEnabled(true);
        endRangeL->setEnabled(true);
    }else{
        updateTable(name,time);
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
}

void PredmetMode::Editing(){

}
void PredmetMode::updateTable(){
    updateTable(predmets->currentText()+groups->currentText());
}
void PredmetMode::updateTable(const QString& name){
    QString q="select ФИО as '"+ELanguage::getWord(NAME)+"'";
    for(QString i:bd->getDateListU(beginRange->date(),endRange->date())){
        q+=","+i.replace('-','_')+ " as '"+QDate::fromString(i,"yyyy-MM-dd").toString("dd.MMMM.yyyy")+"'";
    }
    switch (prefix->currentIndex()) {
    case 0:
        q+=",sum as '"+ELanguage::getWord(SUMM)+"' from LC_"+name;
        break;
    case 1:
        q+=",sum as '"+ELanguage::getWord(SUMM)+"' from PC_"+name;
        break;
    case 2:
        q+=",sum as '"+ELanguage::getWord(SUMM)+"' from KRC_"+name;
        break;
    case 3:
        q+=",sum as '"+ELanguage::getWord(SUMM)+"' from RGRC_"+name;
        break;
    case 4:
        q+=",sum as '"+ELanguage::getWord(SUMM)+"' from LD_"+name;
        break;
    case 5:
        q+=",sum as '"+ELanguage::getWord(SUMM)+"' from KRD_"+name;
        break;
    case 6:
        q+=",sum as '"+ELanguage::getWord(SUMM)+"' from RGRD_"+name;
        break;
    default:
        break;
    }
    qyer->exec(q);
    model->setQuery(*qyer);
}
void PredmetMode::updateTable(const QString& name,const QString&time){
    qyer->exec("select "+
               groups->currentText()+".ФИО as '"+ELanguage::getWord(LABS_V)+"', "+
               "LC_"+name+"."+time+" as '"+ELanguage::getWord(LABS_V)+"', "+
               "PC_"+name+"."+time+" as '"+ELanguage::getWord(PRACT_V)+"', "+
               "KRC_"+name+"."+time+" as '"+ELanguage::getWord(KONTR_V)+"', "+
               "RGRC_"+name+"."+time+" as '"+ELanguage::getWord(RGR_V)+"', "+
               "LD_"+name+"."+time+" as '"+ELanguage::getWord(LABS_Z)+"', "+
               "KRD_"+name+"."+time+" as '"+ELanguage::getWord(KONTR_Z)+"', "+
               "RGRD_"+name+"."+time+" as '"+ELanguage::getWord(RGR_Z)+"' "
               "from "+
               groups->currentText()+",LC_"+name+","+"PC_"+name+","+"KRC_"+name+","+
             "RGRC_"+name+","+"LD_"+name+","+"KRD_"+name+","+"RGRD_"+name+" "+
               "where "+
                groups->currentText()+".ФИО="+"LC_"+name+".ФИО AND "+
                groups->currentText()+".ФИО="+"PC_"+name+".ФИО AND "+
                groups->currentText()+".ФИО="+"KRC_"+name+".ФИО AND "+
                groups->currentText()+".ФИО="+"RGRC_"+name+".ФИО AND "+
                groups->currentText()+".ФИО="+"LD_"+name+".ФИО AND "+
                groups->currentText()+".ФИО="+"KRD_"+name+".ФИО AND "+
                groups->currentText()+".ФИО="+"RGRD_"+name+".ФИО ");
    model->setQuery(*qyer);
}
void PredmetMode::createContextMenu(){
}
void PredmetMode::changeClick(){
}
void PredmetMode::contextMenuEvent(QContextMenuEvent *event){
    if(change->isEnabled()){
        QMenu menu(this);
        menu.exec(event->globalPos());
    }
}
PredmetMode::~PredmetMode(){

}
