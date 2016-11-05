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
#include "group.h"

Group::Group(QObject *parent):
    QObject(parent){
    name="none";
    state=empty;
}
Group::Group(const QStringList &base, const QString name_,QObject* parent):
    QObject(parent)
{
    name=name_;
    state=Editing;
    for(const  QString str:base){
        students.push_back(str);
    }
}
int Group::size()const{
    return students.size();
}
/*void Group::sort(){
    bool b=true;
    int temp = students_colums.size();
    while(b &&--temp>0){
        b=false;
        for(int i=0;i<temp;i++){
            if(students_colums[i]>students_colums[i+1]){
                b=true;
                QString temp1=students_colums[i];
                students_colums[i]=students_colums[i+1];
                students_colums[i+1]=temp1;
            }
        }
    }
}*/
QString Group::getName()const{
    return name;
}
QDataStream& operator >>(QDataStream &stream, Group & group){
    int temp;
    stream>>temp;
    for(temp;temp>0;temp--){
        QString temp2;
        stream>>temp2;
        group.students.push_back(temp2);
    }
    stream>>group.name;
    group.state=noCreated;
    return stream;
}
QDataStream& operator <<(QDataStream &stream, Group & group){
    stream<<(int)group.students.size();
    for(QString str:group.students)
        stream<<str;
    stream<<group.name;
    return stream;
}
bool Group::operator ==(const Group&A){
    if(size()==A.size()){
        for(int i=0;i<A.size();i++){
            if(students[i]!=A.students[i])
                return false;
        }
        return true;
    }
    return false;
}
bool Group::operator !=(const Group&A){
    return !((*this)==A);

}
Group& Group::operator =(const Group&G){
    students.clear();
    for(QString i:G.students)
        students.push_back(i);
    state=G.state;
    name=G.name;
    return *this;
}
/*bool Group::Write(const QString &patch){
    QFile f(patch);
    if(f.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        QDataStream stream(&f);
        stream<<*this;
        f.close();
        return true;
    }
    return false;
}
bool Group::Read(const QString &patch){
    QFile f(patch);
    if(f.open(QIODevice::ReadOnly)){
        QDataStream stream(&f);
        stream>>*this;
        f.close();
        return true;
    }
    return false;
}*/
QStringList Group::getList() const{
    QStringList temp;
    for(QString i:students)
        temp.push_back(i);
    return temp;
}
QString& Group::operator [](const ui& index){
    return students[index];
}
bool Group::find(const QString &data) const{
    for(QString str:students)
        if(str==data)
            return true;
    return false;
}
void Group::clearStudents(){
    students.clear();
}
savedState Group::getSavedState(){
    return state;
}
void Group::setSavedState(const savedState &s){
    state=s;
}
void Group::addStudent(const QString &name,bool last){
    if(name.size()>0){
        students.push_back(name);
        if(last)
            emit changed(this);
    }

}
bool Group::deleteStudent(const ui &index){
    if(index>=(ui)students.size())
        return false;
    students.remove(index,1);
    emit changed(this);
    return true;
}
bool Group::deleteStudent(const QString&name){
    for(int i=0;i<students.size();i++){
        if(name==students[i]){
            students.remove(i,1);
            return true;
            emit changed(this);
        }
    }
    return false;
}
Group::~Group(){
    students.clear();
}
