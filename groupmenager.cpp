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
#include "groupmenager.h"
GroupMenager::GroupMenager(DataBase *bd_, QWidget *parent) : QDialog(parent)
{
    bd=bd_;
    this->setModal(true);
    tableGroub=new QTableWidget(this);
    add=new QPushButton("Создать группу",this);     add->setFocusPolicy(Qt::NoFocus);
    dell=new QPushButton("Удалить группу",this);    dell->setFocusPolicy(Qt::NoFocus);
    addU=new QPushButton("Добавить обучающегося",this);  addU->setFocusPolicy(Qt::NoFocus);
    save=new QPushButton("Записать изменения",this);         save->setFocusPolicy(Qt::NoFocus);
    ComboWrite(cb=new QComboBox(this));
    connect(cb,SIGNAL(activated(int)),this,SLOT(ComboChsnged(int)));
    cb->setGeometry(interval,interval,Bwidth,Bheight);
    add->setGeometry(1*(interval+Bwidth),interval,Bwidth,Bheight);
    addU->setGeometry(2*(interval+Bwidth),interval,Bwidth,Bheight);
    save->setGeometry(1*(interval+Bwidth),interval+Bheight,Bwidth,Bheight);
    dell->setGeometry(2*(interval+Bwidth),interval+Bheight,Bwidth,Bheight);
    tableGroub->setGeometry(interval,3*(interval+Bheight),Bwidth,Bheight);
    tableGroub->setFocus();
    this->setMinimumSize(640,480);
    save->setEnabled(false);
    connect(addU,SIGNAL(clicked(bool)),this,SLOT(addu()));
    connect(add,SIGNAL(clicked(bool)),this,SLOT(add_()));
    connect(save,SIGNAL(clicked(bool)),this,SLOT(Save()));
    connect(dell,SIGNAL(clicked(bool)),this,SLOT(del()));
    if(cb->count()>0)ComboChsnged(0);
}
void GroupMenager::addu(){
    if(cb->count()>0){
        tableGroub->setRowCount(tableGroub->rowCount()+1);
        tableGroub->setCellWidget(tableGroub->rowCount()-1,0,new QLineEdit("Новый обучающийся "+QString::number(tableGroub->rowCount())));
        connect((QLineEdit*)tableGroub->cellWidget(tableGroub->rowCount()-1,0),SIGNAL(textChanged(QString)),this,SLOT(edited()));
        edited();
    }
}
void GroupMenager::resizeEvent(QResizeEvent *){
    tableGroub->setGeometry(interval,2*(interval+Bheight),this->width()*0.98,this->height()*0.87);
    if(tableGroub->columnCount()>0)tableGroub->setColumnWidth(0,this->width()*0.93);
}
void GroupMenager::Save(){
    save_(true);
  /*  if(cb->count()>0&&tableGroub->rowCount()>0){
        thisGroup->clearStudents();
        for(int i=0;i<tableGroub->rowCount();i++){
            thisGroup->addStudent(((QLineEdit*)tableGroub->cellWidget(i,0))->text(),i==tableGroub->rowCount()-1);
        }
        thisGroup->setSavedState(noCreated);
        save->setEnabled(false);
        ComboChsnged(0);
    }else{
        QMessageBox::information(this,"Error","Таблица пуста!");
    }*/
}

#define thisGroup ((*bd->getDataList())[cb->currentIndex()].first)
void GroupMenager::save_(bool logUpdate){
    if(cb->count()>0&&tableGroub->rowCount()>0){
        thisGroup->clearStudents();
        for(int i=0;i<tableGroub->rowCount();i++){
            thisGroup->addStudent(((QLineEdit*)tableGroub->cellWidget(i,0))->text(),i==tableGroub->rowCount()-1);
        }
        thisGroup->setSavedState(noCreated);
        save->setEnabled(false);
        if(logUpdate)
            ComboChsnged(0);
    }else{
        QMessageBox::information(this,"Error","Таблица пуста!");
    }
}
void GroupMenager::del(){
    QMessageBox::StandardButton temp;
    temp = QMessageBox::question(this, "Удаление","Удалить группу "+thisGroup->getName()+" ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(temp==QMessageBox::Yes){
        if(bd->deleteGroup(thisGroup->getName())){
            QMessageBox::information(this,"Message","Группа успешно удалена!");
            tableGroub->clear();
            ComboWrite(cb);
        }
            else
            QMessageBox::information(this,"Error","Группу удалить не удалось!");
    }
}
void GroupMenager::edited(){
    //if(bd->getAutosaveGroup())
      //  save_(false);
    save->setEnabled(true);
}
void GroupMenager::add_(){
    QString temp;
    int temp3;
    do{
        temp=QInputDialog::getText(this,"Новая группа","Введите номер новой группы");
        temp3=QInputDialog::getInt(this,"Новая группа","Введите количество студентов",0,0,500);
        if(temp=="") return;
    }while(bd->find(temp));
    tableGroub->setRowCount(temp3);
    tableGroub->setColumnCount(1);
    tableGroub->setHorizontalHeaderItem(0,new QTableWidgetItem(temp));
    for(int j=0;j<tableGroub->rowCount();j++){
        tableGroub->setCellWidget(j,0,new QLineEdit("обучающийся "+QString::number(j+1)));
    }
    bd->createGroup(QStringList(),temp);
    QMessageBox::information(this,"Message","группа "+temp+" создана");
    ComboWrite(cb);
    cb->setCurrentText(temp);
    save->setEnabled(true);
    Save();
}
void GroupMenager::ComboWrite(QComboBox *combo){
    combo->clear();
    for(BPair p:*bd->getDataList()){
        combo->addItem(p.first->getName());
    }
    dell->setEnabled(combo->count()!=0);
}
void GroupMenager::TableDisconnect(){
    for(int j=0;j<tableGroub->rowCount();j++)
        disconnect(tableGroub->cellWidget(j,0));
}
void GroupMenager::ComboChsnged(int){
    TableDisconnect();
    tableGroub->clear();
    tableGroub->setRowCount(thisGroup->size());
    tableGroub->setColumnCount(1);
    tableGroub->setHorizontalHeaderItem(0,new QTableWidgetItem(thisGroup->getName()));
    tableGroub->setColumnWidth(0,this->width()*0.93);
    for(int j=0;j<thisGroup->size();j++){
        tableGroub->setCellWidget(j,0,new QLineEdit((*thisGroup)[j]));
        connect((QLineEdit*)tableGroub->cellWidget(j,0),SIGNAL(editingFinished()),this,SLOT(edited()));
    }
    save->setEnabled(false);
}
GroupMenager::~GroupMenager(){
}

