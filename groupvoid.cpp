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
#include "groupvoid.h"
GroupVoid::GroupVoid( Group *gr, ETime *system, QObject *ptr):
    QObject(ptr)
{
    rows=system;
    colums=gr;
    gotWeek=limit=0;
    for(int i=0;i<12;i++)
        kolendar[i]=true;
    colums_TEST=new Group;
    rows_TEST=new ETime;
    connect(rows,SIGNAL(append(const QVector<QDate>*)),this,SLOT(append(const QVector<QDate>*)));
    connect(colums,SIGNAL(changed(Group*)),this,SLOT(Schanged(Group*)));
    //autoClear();
    //resetSumm();
}
ui GroupVoid::getItem(const int row, const int col){

    return (*dataStudents[row])[col];
}
void GroupVoid::setLim(const int &lim){
    if(rows->isStarted())
        limit=lim;
}
int GroupVoid::getLim(bool absolut)const{
    if(gotWeek!=0&&!absolut)
        return limit*gotWeek/dataStudents.size();
    else
        return limit;
}
QStringList GroupVoid::getRowsHeader()const{
    return rows->getList();
}
QStringList GroupVoid::getColumsHeader()const{
    return colums->getList();
}
void GroupVoid::Schanged(Group*){
    autoClear();
    emit Changed(this);
}
void GroupVoid::append(const QVector<QDate> *){
    autoClear();
    emit Changed(this);
}
bool GroupVoid::addRecord(QVector<ui> *parent){
    if(parent==NULL){
        if(rows->size()>dataStudents.size()){
            dataStudents.push_back(new QVector<ui>(colums->size()));
           // dataStudents.push_back(new QVector<ui>);
            return true;
    }
        return false;
    }else{
        if(rows->size()>dataStudents.size()&&parent->size()==colums->size()){
            dataStudents.push_back(parent);
            return true;
        }
        return false;
    }
}
QString GroupVoid::getName()const
{
    return colums->getName();
}
QString& GroupVoid::getLastReport(){
    return report;
}
int GroupVoid::getGotWeek()const{
    return gotWeek;
}
bool GroupVoid::ChangeTop(QVector<ui> *parent){
    if(parent!=NULL&&parent->size()==colums->size()){
        delete dataStudents.last();
        dataStudents.pop_back();
        dataStudents.push_back(parent);
        return true;
    }
    return false;
}
bool GroupVoid::ChangeTopElement(const ui &index, const ui &value){
    (*dataStudents.last())[index]=value;
    return true;
}
bool GroupVoid::ChangeTopElement(const QString &name, const int &value){
    if(name=="") return false;
    for(int i=0; i<colums->size();i++){
        if(name==(*colums)[i]){
            return ChangeTopElement(i,value);
        }
    }
    return false;
}
void GroupVoid::clear(){
    for(QVector<ui>* i:dataStudents){
        i->clear();
        delete i;
    }
    dataStudents.clear();
    //colums.clear();
    //rows.clear();
}
void GroupVoid::autoClear(){
    bool test=true;
    if(rows_TEST!=rows){
       int temp= rows->sizes(*rows_TEST);
       while(temp>0){
           this->addRecord();
           temp--;
           test=false;
       }
    }
    *rows_TEST=*rows;
    if(colums_TEST!=colums){
            for(int i=0;i<colums_TEST->size();i++)
                if(!colums->find((*colums_TEST)[i])){
                    for(int j=0;j<rows->size();j++){
                        dataStudents[j]->remove(i,1);
                    }
                    colums_TEST->deleteStudent(i);
                }
            int temp=colums->size()-colums_TEST->size();
            while(temp>0&&test){
                for(int j=0;j<rows->size();j++){
                    dataStudents[j]->push_back(0);
                }
                temp--;
            }
        }
    *colums_TEST=*colums;
}
void GroupVoid::resetSumm(){
    if(dataStudents.size()==0)return;
    unsigned int temp=0;
    QString temp1="",temp2="";
    summa.clear();
    bool tempBool=true;
    for(int i=0;i<12;i++)
        tempBool=tempBool&&!kolendar[i];
    for(int j=0;j<dataStudents[0]->size();j++){
        gotWeek=temp=0;
        for(int i=0;i<dataStudents.size();i++){
            if(kolendar[(*rows)[i].month()-1]||tempBool){
                temp+=(*dataStudents[i])[j];
                if(!tempBool)gotWeek++;
            }
        }
        if(temp>(limit*(tempBool)?1:(gotWeek/dataStudents.size()))){
            temp2+="<br>"+(*colums)[j];
        }else
        if(temp>(limit/2*(tempBool)?1:(gotWeek/dataStudents.size()))){
            temp1+="<br>"+(*colums)[j];
        }
        summa.push_back(temp);
    }
    report.clear();
    if(temp1>0)
        report+="Обучающиеся с большим количеством пропусков:"+temp1+"<br><br>";
    if(temp2>0)
        report+="Обучающиеся с критическим количеством пропусков:"+temp2+"<br>";
    if(temp1.size()+temp2.size()>0)
        emit Warning(this,report);
    else
        report+="У всех обучающихся допустимое количество пропусков.<br>";
}
ui GroupVoid::operator [](const int&i){
    return summa[i];
}
bool *GroupVoid::getKolendar(){
    return kolendar;
}
int GroupVoid::size()const{
    return summa.size();
}
QDataStream& operator >>(QDataStream &stream,GroupVoid &gr){
    gr.clear();
    stream>>*gr.colums_TEST;
    stream>>*gr.rows_TEST;
    for(int i=0;i<gr.rows_TEST->size();i++){
        gr.dataStudents.push_back(new QVector<ui>);
        for(int j=0;j<gr.colums_TEST->size();j++){
            ui temp;
            stream>>temp;
            gr.dataStudents[i]->push_back(temp);
        }
    }
    stream>>gr.limit;
    gr.resetSumm();
    gr.autoClear();
    return stream;
}
QDataStream& operator <<(QDataStream &stream,GroupVoid &gr){
    if(gr.rows->size()!=gr.dataStudents.size()){
        throw EError("DataBase ByteArray Error","GroupVoid::operator <<");
    }else{
        stream<<*gr.colums;
        stream<<*gr.rows;
        for(int i=0;i<gr.rows->size();i++){
            for(int j=0;j<gr.colums->size();j++){
                stream<<(*gr.dataStudents[i])[j];
            }
        }
        stream<<gr.limit;
    }
   return stream;

}
bool GroupVoid::Write(const QString &patch){
    QFile f(patch);
    if(f.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        QDataStream stream(&f);
        stream<<*this;
        f.close();
        return true;
    }
    return false;
}
bool GroupVoid::Read(const QString &patch){
    QFile f(patch);
    if(f.open(QIODevice::ReadOnly)){
        QDataStream stream(&f);
        stream>>*this;
        f.close();
        return true;
    }
    return false;
}
bool GroupVoid::toArchive(const QString &patch){
    rows->stop();
    bool temp =this->Write(patch);
    rows->start();
    return temp;
}
bool GroupVoid::isdrawColumn(const int &i){
    return !kolendar[(*rows)[i].month()-1];
}
GroupVoid::~GroupVoid(){
    for(QVector<ui>* i:dataStudents){
        i->clear();
        delete i;
    }
    delete colums_TEST;
    delete rows_TEST;
}
