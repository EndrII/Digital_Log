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

StarastaMode::StarastaMode(DataBase *bd_, QWidget *parent) : QWidget(parent)
{
    bd=bd_;
    groups=new QComboBox(this);
    add=new QPushButton("Начать отчет",this);
    add->setToolTip("Начать вести отчет для выбранной группы");
    save=new QPushButton("Записать",this);
    save->setToolTip("записать введенные значения по пропускам");
    remove=new QPushButton("удалить отчет",this);
    remove->setToolTip("прекратить вести отчеты для выбранной группы");
    table=new QTableWidget(this);
    groups->setGeometry(interval,interval,Bwidth,Bheight);
    add->setGeometry(1*(interval+Bwidth),interval,Bwidth,Bheight);
    save->setGeometry(2*(interval+Bwidth),interval,Bwidth,Bheight);
    remove->setGeometry(3*(interval+Bwidth),interval,Bwidth,Bheight);
    table->setGeometry(interval,2*interval+Bheight,Bwidth,Bheight);
    this->setMinimumSize(640,480);
    //connect(bd,SIGNAL(gr),this,SLOT(addClick()));
    connect(add,SIGNAL(clicked(bool)),this,SLOT(addClick()));
    connect(save,SIGNAL(clicked(bool)),this,SLOT(saveClick()));
    connect(remove,SIGNAL(clicked(bool)),this,SLOT(removeClick()));
    connect(groups,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
}
void StarastaMode::resizeEvent(QResizeEvent *){
    table->setGeometry(interval,2*interval+Bheight,this->width()*0.95,this->height()*0.9);
}
void StarastaMode::addClick(){
    bd->createControlGroup(thisGroup);
}
void StarastaMode::saveClick(){
    if(bd->getState()!=Started) return;
    for(int i=0;i<table->rowCount();i++){
        thisVoidGroup->ChangeTopElement(i,((QLineEdit*)table->cellWidget(i,table->columnCount()-2))->text().toUInt());
    }
    thisVoidGroup->resetSumm();
    thisGroup->setSavedState(Saved);
    save->setEnabled(false);
    this->RedrawLite(thisVoidGroup);
}
void StarastaMode::removeClick(){
    if(bd->getPass()!=""){
        if(bd->getPass()==QInputDialog::getText(this,"Безопасность","Введите пароль безопасности для входа в настройки базы данных")){
            QMessageBox::StandardButton temp;
            temp = QMessageBox::question(this, "Удаление","Удалить группу "+thisGroup->getName()+" ?",
                                          QMessageBox::Yes|QMessageBox::No);
            if(temp==QMessageBox::Yes){
                if(bd->deleteControlGroup(thisGroup))
                      this->Redraw(thisVoidGroup);
            }

        }else{
            QMessageBox::information(this,"Безопасность","Введен не верный пароль!");
        }
    }else{
        QMessageBox::StandardButton temp;
        temp = QMessageBox::question(this, "Удаление","Удалить группу "+thisGroup->getName()+" ?",
                                      QMessageBox::Yes|QMessageBox::No);
        if(temp==QMessageBox::Yes){
            if(bd->deleteControlGroup(thisGroup))
                this->Redraw(thisVoidGroup);
        }
    }
}
void StarastaMode::BDOpened(QString){
    ComboWrite(groups);
}
void StarastaMode::GroupDeleted(QString){
    ComboWrite(groups);
}
void StarastaMode::GroupListChanged(int gr){
    TableDisconnect();
    table->clear();
    table->setColumnCount(0);
    table->setRowCount(0);
    if(gr==-1) return;
    BPair TempGr=(*bd->getDataList())[gr];
    bd->openGroup(TempGr.first->getName());
}
void StarastaMode::ComboWrite(QComboBox *combo){
    groups->clear();
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
    }
}
void StarastaMode::TableDisconnect(){
    for(int j=0;j<table->rowCount();j++){
        if(table->columnCount()>0)
            disconnect(table->cellWidget(j,table->columnCount()-2));
        }
}
void StarastaMode::GroupSaved(GroupVoid *){

}
void StarastaMode::Editing(){
    if(bd->getAutosaveGroup()){
        this->saveClick();
    }else{
        thisGroup->setSavedState(noSaved);
        save->setEnabled(true);
    }
}
void StarastaMode::StateChanged(StateDataBase st){
    switch (st) {
    case notOpened:
        add->setEnabled(false);
        save->setEnabled(false);
        remove->setEnabled(false);
        groups->setEnabled(false);
        break;
    case Arhive:
        add->setEnabled(false);
        save->setEnabled(false);
        remove->setEnabled(false);
        groups->setEnabled(true);
        break;
    case notStarted:
        add->setEnabled(false);
        save->setEnabled(false);
        remove->setEnabled(false);
        groups->setEnabled(false);
        break;
    case Started:
        add->setEnabled(groups->count()>0);
        //save->setEnabled(true);
        remove->setEnabled(groups->count()>0);
        groups->setEnabled(true);
        break;
    default:
        break;
    }
}
void StarastaMode::GroupChanged(Group *){
}
void StarastaMode::ControlGroupChanged(GroupVoid *gr){
    if(thisVoidGroup==gr)
      Redraw(thisVoidGroup);
}
void StarastaMode::GroupOpened(GroupVoid *gr){
    Redraw(gr);
    connect(gr,SIGNAL(Changed(GroupVoid*)),this,SLOT(ControlGroupChanged(GroupVoid *)));
    thisGroup->setSavedState(Saved);
    save->setEnabled(false);
}
void StarastaMode::GroupCreated(Group *){
    ComboWrite(groups);
}
void StarastaMode::controlGroupCreated(GroupVoid *gr){
    GroupOpened(gr);
    gr->setLim(QInputDialog::getInt(this,"Создание отчета","введите лимит пропусков для группы "+gr->getName(),bd->getLastPropusk()));
}
void StarastaMode::Redraw(GroupVoid *gr){
    if(gr==NULL)
    {
        table->clear();
        table->setRowCount(0);
        table->setColumnCount(0);

    }else{
        QList<QString> tempRow=gr->getColumsHeader();
        QList<QString> tempCol=gr->getRowsHeader();
        table->setRowCount(gr->getColumsHeader().size());
        table->setColumnCount(gr->getRowsHeader().size()+1);
        gr->resetSumm();
        for(int i=0;i<tempRow.size();i++){
            table->setVerticalHeaderItem(i,new QTableWidgetItem(tempRow[i]));
            for(int j=0;j<tempCol.size();j++){
                if(i==0)table->setHorizontalHeaderItem(j,new QTableWidgetItem(tempCol[j]));
                if(j!=tempCol.size()-1||bd->isEnd()){
                    table->setCellWidget(i,j,new QLabel(QString::number(gr->getItem(j,i))));
                    table->cellWidget(i,j)->setStyleSheet("background-color:  #F0FFFF");
                    if((*gr)[i]>(ui)gr->getLim()/2)
                        table->cellWidget(i,j)->setStyleSheet("background-color: yellow");
                    if((*gr)[i]>(ui)gr->getLim())
                        table->cellWidget(i,j)->setStyleSheet("background-color: #ff4500");
                }
                else{
                    table->setCellWidget(i,j,new QLineEdit(QString::number(gr->getItem(j,i))));
                    connect((QLineEdit*)table->cellWidget(i,j),SIGNAL(editingFinished()),this,SLOT(Editing()));
                }
                    //table->horizontalHeader()
                //temp++;
            }
        }
        table->setHorizontalHeaderItem(table->columnCount()-1,new QTableWidgetItem("Сумма"));
        for(int i=0;i<gr->size();i++){
            table->setCellWidget(i,table->columnCount()-1,new QLabel(QString::number((*gr)[i])));
            table->cellWidget(i,table->columnCount()-1)->setStyleSheet("background-color:  #F0FFFF");
            if((*gr)[i]>(ui)gr->getLim()/2)
                table->cellWidget(i,table->columnCount()-1)->setStyleSheet("background-color: yellow");
            if((*gr)[i]>(ui)gr->getLim())
                table->cellWidget(i,table->columnCount()-1)->setStyleSheet("background-color: #ff4500");
        }
    }

}
void StarastaMode::RedrawLite(GroupVoid *gr){
        QList<QString> tempRow=gr->getColumsHeader();
        QList<QString> tempCol=gr->getRowsHeader();
        for(int i=0;i<tempRow.size();i++){
            for(int j=0;j<tempCol.size();j++){
                if(j!=tempCol.size()-1||bd->isEnd()){
                    table->cellWidget(i,j)->setStyleSheet("background-color:  #F0FFFF");
                    if((*gr)[i]>(ui)gr->getLim()/2)
                        table->cellWidget(i,j)->setStyleSheet("background-color: yellow");
                    if((*gr)[i]>(ui)gr->getLim())
                        table->cellWidget(i,j)->setStyleSheet("background-color: #ff4500");
                }
                else{
                    ((QLineEdit*)table->cellWidget(i,j))->setText(QString::number(gr->getItem(j,i)));
                }
            }
        }
        table->setHorizontalHeaderItem(table->columnCount()-1,new QTableWidgetItem("Сумма"));
        for(int i=0;i<gr->size();i++){
            ((QLabel*)table->cellWidget(i,table->columnCount()-1))->setText(QString::number((*gr)[i]));
            table->cellWidget(i,table->columnCount()-1)->setStyleSheet("background-color:  #F0FFFF");
            if((*gr)[i]>(ui)gr->getLim()/2)
                table->cellWidget(i,table->columnCount()-1)->setStyleSheet("background-color: yellow");
            if((*gr)[i]>(ui)gr->getLim())
                table->cellWidget(i,table->columnCount()-1)->setStyleSheet("background-color: #ff4500");
        }
}
StarastaMode::~StarastaMode(){

}
