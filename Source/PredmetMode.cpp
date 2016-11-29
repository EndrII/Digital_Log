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
}
void PredmetMode::updateGroups(){
    groups->clear();
    groups->addItems(bd->getGroupList());
    ComboWrite(0);
}
void PredmetMode::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Return){
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
           // model->setData(table->currentIndex(),tempData);
           // table->repaint();
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
}
void PredmetMode::changeClick(){
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
    updateTable(name,time);

}
void PredmetMode::ComboWrite(int){
    predmets->clear();
    times->clear();
    times->addItems(bd->getDateListP());
    predmets->addItems(bd->getGroupPredmetsList(groups->currentText()));
}

void PredmetMode::Editing(){

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
void PredmetMode::ClickOnlySumm(bool){
}
void PredmetMode::ClickPrintPDF(bool){
}
void PredmetMode::ClickPrintHTML(bool){
}
void PredmetMode::clearFilterClick(bool){
}
void PredmetMode::alfavitClick(bool){
}
void PredmetMode::maxtominClick(bool){
}
void PredmetMode::mintomaxClick(bool){
}
void PredmetMode::curentTimeClick(bool){
}

void PredmetMode::contextMenuEvent(QContextMenuEvent *event){
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
PredmetMode::~PredmetMode(){

}
