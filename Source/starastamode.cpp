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
    groups=new QComboBox(this);
    add=new QPushButton("Начать отчет",this);
    add->setToolTip("Начать вести отчет для выбранной группы");
    change=new QPushButton("Изменить лимит пропусков",this);
    change->setToolTip("Изменить предел количества пропусков");
    remove=new QPushButton("удалить отчет",this);
    remove->setToolTip("прекратить вести отчеты для выбранной группы");
    table=new QTableWidget(this);
    groups->setGeometry(interval,interval,Bwidth,Bheight);
    add->setGeometry(1*(interval+Bwidth),interval,Bwidth,Bheight);
    change->setGeometry(2*(interval+Bwidth),interval,Bwidth,Bheight);
    remove->setGeometry(3*(interval+Bwidth),interval,Bwidth,Bheight);
    table->setGeometry(interval,2*interval+Bheight,Bwidth,Bheight);
    this->setMinimumSize(800,480);
    change->setEnabled(false);
    createContextMenu();
    connect(add,SIGNAL(clicked(bool)),this,SLOT(addClick()));
    connect(change,SIGNAL(clicked(bool)),this,SLOT(changeClick()));
    connect(remove,SIGNAL(clicked(bool)),this,SLOT(removeClick()));
    connect(groups,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
}
void StarastaMode::resizeEvent(QResizeEvent *){
    table->setGeometry(interval,2*interval+Bheight,this->width()*0.95,this->height()*0.9);
}
void StarastaMode::addClick(){
    //bd->createControlGroup(thisGroup);
}
void StarastaMode::changeClick(){
    /*if(bd->security()){
        thisVoidGroup->setLim(QInputDialog::getInt(this,"Лимит поличество пропусков","текущее максимально доапустимое количество пропусков",thisVoidGroup->getLim(true),0));
        RedrawLite(thisVoidGroup);
    }*/
}
void StarastaMode::removeClick(){
    /*if(bd->getPass()!=""){
        if(bd->getPass()==QInputDialog::getText(this,"Безопасность","Введите пароль безопасности для входа в настройки базы данных")){
            QMessageBox::StandardButton temp;
            temp = QMessageBox::question(this, "Удаление","Удалить группу "+thisGroup->getName()+" ?",
                                          QMessageBox::Yes|QMessageBox::No);
            if(temp==QMessageBox::Yes){
                if(bd->deleteControlGroup(thisGroup)){
                    change->setEnabled(false);
                    this->Redraw(thisVoidGroup);
                }
            }

        }else{
            QMessageBox::information(this,"Безопасность","Введен не верный пароль!");
        }
    }else{
        QMessageBox::StandardButton temp;
        temp = QMessageBox::question(this, "Удаление","Удалить группу "+thisGroup->getName()+" ?",
                                      QMessageBox::Yes|QMessageBox::No);
        if(temp==QMessageBox::Yes){
            if(bd->deleteControlGroup(thisGroup)){
                change->setEnabled(false);
                this->Redraw(thisVoidGroup);
            }
        }
    }*/
}
void StarastaMode::BDOpened(QString){
    //ComboWrite(groups);
}
/*void StarastaMode::GroupDeleted(QString){
    //ComboWrite(groups);
}*/
void StarastaMode::GroupListChanged(int gr){
    /*change->setEnabled(false);
    TableDisconnect();
    table->clear();
    table->setColumnCount(0);
    table->setRowCount(0);
    if(gr==-1) return;
    BPair TempGr=(*bd->getDataList())[gr];
    bd->openGroup(TempGr.first->getName());*/
}
void StarastaMode::ComboWrite(QComboBox *combo){
    /*groups->clear();
    for(BPair p:*bd->getDataList()){
        combo->addItem(p.first->getName());
    }
    if(groups->count()>0&&bd->getState()==Started){
        add->setEnabled(true);
        remove->setEnabled(true);
        if(thisVoidGroup!=NULL)Redraw(thisVoidGroup);
    }else{
        add->setEnabled(false);
        remove->setEnabled(false);
    }*/
}
void StarastaMode::TableDisconnect(){
    /*for(int j=0;j<table->rowCount();j++){
        if(table->columnCount()>0)
            disconnect(table->cellWidget(j,table->columnCount()-2));
        }*/
}
/*void StarastaMode::GroupSaved(GroupVoid *){
}*/
void StarastaMode::Editing(){
   /* if(bd->getState()!=Started) return;
    for(int i=0;i<table->rowCount();i++){
        thisVoidGroup->ChangeTopElement(indexs[i],((QLineEdit*)table->cellWidget(i,table->columnCount()-2))->text().toUInt());
    }
    thisVoidGroup->resetSumm();
    thisGroup->setSavedState(Saved);
     this->RedrawLite(thisVoidGroup);*/
}
/*void StarastaMode::StateChanged(StateDataBase st){
    /*switch (st) {
    case notOpened:
        add->setEnabled(false);
        change->setEnabled(false);
        remove->setEnabled(false);
        groups->setEnabled(false);
        break;
    case Arhive:
        add->setEnabled(false);
        change->setEnabled(false);
        remove->setEnabled(false);
        groups->setEnabled(true);
        break;
    case notStarted:
        add->setEnabled(false);
        change->setEnabled(false);
        remove->setEnabled(false);
        groups->setEnabled(false);
        break;
    case Started:
        add->setEnabled(groups->count()>0);
        //change->setEnabled(true);
        remove->setEnabled(groups->count()>0);
        groups->setEnabled(true);
        break;
    default:
        break;
    }
}*/
/*void StarastaMode::print(QTableWidget *Table, QString patch){

}*/
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
    /*bool* temp= thisVoidGroup->getKolendar();
    for(int i=0;i<12;i++)
        temp[i]=false;
    this->Redraw(thisVoidGroup);
*/
}
void StarastaMode::ClickPrintPDF(bool){
  //  Printer::printPDF(this->table,thisVoidGroup->getLastReport(),QFileDialog::getSaveFileName(this,"Создание отчёта в PDF","./","*.pdf"));
}
void StarastaMode::ClickPrintHTML(bool){
    //Printer::printHtml(this->table,thisVoidGroup->getLastReport(),QFileDialog::getSaveFileName(this,"Создание отчёта в HTML","./","*.html"));
}
void StarastaMode::clearFilterClick(bool){
    /*indexPush(thisVoidGroup);
    bool* temp= thisVoidGroup->getKolendar();
    for(int i=0;i<12;i++)
        temp[i]=true;
    this->Redraw(thisVoidGroup);*/
}
void StarastaMode::alfavitClick(bool){
    /*QStringList s=thisVoidGroup->getColumsHeader();
    bool Switcher=true;
    int siz=s.size();
    while(Switcher&&siz){
        Switcher=false;
        for(int i=1;i<siz;i++){
            if(s[indexs[i]].compare(s[indexs[i-1]],Qt::CaseInsensitive)<0){
                Switcher=true;
                unsigned int temp= indexs[i];
                indexs[i]=indexs[i-1];
                indexs[i-1]=temp;
            }
        }
        siz--;
    }
    this->Redraw(thisVoidGroup);*/
}
void StarastaMode::maxtominClick(bool){
   /* QStringList s=thisVoidGroup->getColumsHeader();
    bool Switcher=true;
    int siz=s.size();
    while(Switcher&&siz){
        Switcher=false;
        for(int i=1;i<siz;i++){
            if(tableSumWidget(i)>tableSumWidget(i-1)){
              //  QString::toInt();
                Switcher=true;
                unsigned int temp= indexs[i];
                indexs[i]=indexs[i-1];
                indexs[i-1]=temp;
            }
        }
        siz--;
    }
    this->Redraw(thisVoidGroup);*/
}
void StarastaMode::mintomaxClick(bool){
    /*QStringList s=thisVoidGroup->getColumsHeader();
    bool Switcher=true;
    int siz=s.size();
    while(Switcher&&siz){
        Switcher=false;
        for(int i=1;i<siz;i++){
            if(tableSumWidget(i)<tableSumWidget(i-1)){
              //  QString::toInt();
                Switcher=true;
                unsigned int temp= indexs[i];
                indexs[i]=indexs[i-1];
                indexs[i-1]=temp;
            }
        }
        siz--;
    }
    this->Redraw(thisVoidGroup);*/
}
void StarastaMode::curentTimeClick(bool){
    /*MounthDialog mou(thisVoidGroup->getKolendar(),this);
    mou.exec();
    this->Redraw(thisVoidGroup);*/
}
/*void StarastaMode::GroupChanged(Group *){
}*/
/*void StarastaMode::indexPush(GroupVoid *gr){
    indexs.clear();
    for(int i=0;i<gr->getColumsHeader().size();i++)
        indexs.push_back(i);
}*/
/*void StarastaMode::ControlGroupChanged(GroupVoid *gr){
    if(thisVoidGroup==gr){
        indexPush(gr);
        Redraw(thisVoidGroup);
    }
}*/
/*void StarastaMode::GroupOpened(GroupVoid *gr){
    indexPush(gr);
    Redraw(gr);
    connect(gr,SIGNAL(Changed(GroupVoid*)),this,SLOT(ControlGroupChanged(GroupVoid *)));
    thisGroup->setSavedState(Saved);
    change->setEnabled(true);
}*/
/*void StarastaMode::GroupCreated(Group *){
    ComboWrite(groups);
}*/
/*void StarastaMode::controlGroupCreated(GroupVoid *gr){
    GroupOpened(gr);
    gr->setLim(QInputDialog::getInt(this,"Создание отчета","введите лимит пропусков для группы "+gr->getName(),bd->getLastPropusk()));
    change->setEnabled(true);
}*/
/*void StarastaMode::Redraw(GroupVoid *gr){
    table->clear();
    if(gr==NULL)
    {
        table->setRowCount(0);
        table->setColumnCount(0);
    }else{
        gr->resetSumm();
        QList<QString> tempRow=gr->getColumsHeader();
        QList<QString> tempCol=gr->getRowsHeader();
        table->setRowCount(tempRow.size());
        table->setColumnCount(gr->getGotWeek()+1);
        for(int i=0;i<tempRow.size();i++){

            table->setVerticalHeaderItem(i,new QTableWidgetItem(tempRow[indexs[i]]));
            int tempIndex=0;
            for(int j=0;j<tempCol.size();j++){
                if(gr->isdrawColumn(j)) continue;
                if(i==0)table->setHorizontalHeaderItem(tempIndex,new QTableWidgetItem(tempCol[j]));
                if(j!=tempCol.size()-1||bd->isEnd()){
                    table->setCellWidget(i,tempIndex,new QLabel(QString::number(gr->getItem(j,indexs[i]))));
                    table->cellWidget(i,tempIndex)->setStyleSheet("background-color:  #F0FFFF");
                    if((*gr)[indexs[i]]>(ui)gr->getLim()/2)
                        table->cellWidget(i,tempIndex)->setStyleSheet("background-color: yellow");
                    if((*gr)[indexs[i]]>(ui)gr->getLim())
                        table->cellWidget(i,tempIndex)->setStyleSheet("background-color: #ff4500");
                }
                else{
                    table->setCellWidget(i,tempIndex,new QLineEdit(QString::number(gr->getItem(j,indexs[i]))));
                    connect((QLineEdit*)table->cellWidget(i,tempIndex),SIGNAL(editingFinished()),this,SLOT(Editing()));
                }
                    //table->horizontalHeader()
                tempIndex++;
            }
        }
        table->setHorizontalHeaderItem(table->columnCount()-1,new QTableWidgetItem("Сумма"));
        for(int i=0;i<gr->size();i++){
            table->setCellWidget(i,table->columnCount()-1,new QLabel(QString::number((*gr)[indexs[i]])));
            table->cellWidget(i,table->columnCount()-1)->setStyleSheet("background-color:  #F0FFFF");
            if((*gr)[indexs[i]]>(ui)gr->getLim()/2)
                table->cellWidget(i,table->columnCount()-1)->setStyleSheet("background-color: yellow");
            if((*gr)[indexs[i]]>(ui)gr->getLim())
                table->cellWidget(i,table->columnCount()-1)->setStyleSheet("background-color: #ff4500");
        }
    }

}*/
/*void StarastaMode::RedrawLite(GroupVoid *gr){
        QList<QString> tempRow=gr->getColumsHeader();
        QList<QString> tempCol=gr->getRowsHeader();
        for(int i=0;i<tempRow.size();i++){
            for(int j=0;j<gr->getGotWeek();j++){
                if(gr->isdrawColumn(j)) continue;
                if(j!=tempCol.size()-1||bd->isEnd()){
                    table->cellWidget(i,j)->setStyleSheet("background-color:  #F0FFFF");
                    if((*gr)[indexs[i]]>(ui)gr->getLim()/2)
                        table->cellWidget(i,j)->setStyleSheet("background-color: yellow");
                    if((*gr)[indexs[i]]>(ui)gr->getLim())
                        table->cellWidget(i,j)->setStyleSheet("background-color: #ff4500");
                }
                else{
                    ((QLineEdit*)table->cellWidget(i,j))->setText(QString::number(gr->getItem(j,indexs[i])));
                }
            }
        }
        table->setHorizontalHeaderItem(table->columnCount()-1,new QTableWidgetItem("Сумма"));
        for(int i=0;i<gr->size();i++){
            ((QLabel*)table->cellWidget(i,table->columnCount()-1))->setText(QString::number((*gr)[indexs[i]]));
            table->cellWidget(i,table->columnCount()-1)->setStyleSheet("background-color:  #F0FFFF");
            if((*gr)[indexs[i]]>(ui)gr->getLim()/2)
                table->cellWidget(i,table->columnCount()-1)->setStyleSheet("background-color: yellow");
            if((*gr)[indexs[i]]>(ui)gr->getLim())
                table->cellWidget(i,table->columnCount()-1)->setStyleSheet("background-color: #ff4500");
        }
}*/

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
