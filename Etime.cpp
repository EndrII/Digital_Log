/****************************************************************************
**
** Copyright (C) 2016 A.N.Yankovich.
** Contact: https://vk.com/digitalface
**
** $QT_BEGIN_LICENSE:LGPL21$
** This file is distributed under the GPLv3 license.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "Etime.h"

ETime::ETime(bool ismounth , QObject *parent) :
    QObject(parent)
{
    isMounth=ismounth;
    timer.setInterval(10000);
    connect(&timer,SIGNAL(timeout()),this,SLOT(_add()));
    //timer.start();
    auto_=NULL;
    started=false;
}
QStringList ETime::getList() const{
    QStringList temp;
    for(QDate i:SourceDate)
        temp.push_back(i.toString());
    return temp;
}
int ETime::size()const{
    return SourceDate.size();
}
void ETime::_add(){
    if(SourceDate.size()!=0&&started){
        if(isMounth){
            if(QDate::currentDate().month()!=SourceDate.last().month()){
                SourceDate.push_back(QDate::currentDate());
                emit append(&SourceDate);
            }
        }
        else{
            bool temp=false;
            while (SourceDate.last().daysTo(QDate::currentDate())>=7) {
                SourceDate.push_back(SourceDate.last().addDays(7));
                temp=true;
            }
            if(temp)
                emit append(&SourceDate);
        }
        if(auto_!=NULL&&*auto_&&timeTestBegin()){
            this->start();
        }
        if(auto_!=NULL&&*auto_&&timeTestEnd()){
            this->stop();
        }
    }
}
inline bool ETime::timeTestEnd(){
    return
       (date[1].month()>=QDate::currentDate().month()&&
        date[1].day()>=QDate::currentDate().day()&&
        date[2].month()<=QDate::currentDate().month()&&
        date[2].day()<=QDate::currentDate().day()&&!isStarted())
            ||
       (date[3].month()>=QDate::currentDate().month()&&
        date[3].day()>=QDate::currentDate().day()&&
        date[0].month()<=QDate::currentDate().month()&&
        date[0].day()<=QDate::currentDate().day()&&!isStarted());
}
inline bool ETime::timeTestBegin(){
    return
       (date[0].month()>=QDate::currentDate().month()&&
        date[0].day()>=QDate::currentDate().day()&&
        date[1].month()<=QDate::currentDate().month()&&
        date[1].day()<=QDate::currentDate().day()&&!isStarted())
            ||
       (date[2].month()>=QDate::currentDate().month()&&
        date[2].day()>=QDate::currentDate().day()&&
        date[3].month()<=QDate::currentDate().month()&&
        date[3].day()<=QDate::currentDate().day()&&!isStarted());
}
bool ETime::start(){
    if(!started){
        SourceDate.push_back(QDate::currentDate());
        timer.start();
        emit statusChanget(true);
        return started=true;
    }
    return false;
}
void ETime::stop(){
    timer.stop();
    started=false;
    emit statusChanget(false);
}
bool ETime::isStarted(){
    return started;
}
void ETime::clear(){
    this->stop();
    SourceDate.clear();
}
bool ETime::find(const QDate& data)const{
    for(QDate dat:SourceDate)
        if(dat==data)
            return true;
    return false;
}
QDataStream& operator >>(QDataStream &stream, ETime & tim){
    int temp;
    stream>>temp;
    stream>>tim.isMounth;
    stream>>tim.started;
    if(tim.started)
        tim.timer.start();
    tim.SourceDate.clear();
    for(temp;temp>0;temp--){
        QDate temp2;
        stream>>temp2;
        tim.SourceDate.push_back(temp2);
    }
    tim._add();
    return stream;
}
QDataStream& operator <<(QDataStream &stream, ETime & tim){
    stream<<(int)tim.SourceDate.size();
    stream<<(bool)tim.isMounth;
    stream<<tim.started;
    for(QDate str:tim.SourceDate)
        stream<<str;
    return stream;
}
void ETime::attach(QDate *d, bool *b){
    date=d;
    auto_=b;
}
QDate& ETime::operator [](const unsigned int& index){
        return SourceDate[index];
}
bool ETime::operator !=(const ETime&B){
    return !((*this)==B);
}
bool ETime::operator ==(const ETime&B){
    if(size()==B.size()){
        for(int i=0;i<B.size();i++){
            if(SourceDate[i]!=B.SourceDate[i])
                return false;
        }
        return true;
    }
    return false;
}
ETime& ETime::operator =(const ETime&B){
    started=B.started;
    isMounth=B.isMounth;
    auto_=B.auto_;
    date=B.date;
    SourceDate.clear();
    for(QDate d:B.SourceDate){
        SourceDate.push_back(d);
    }
}
int  ETime::sizes(const ETime &A){
        return size()-A.size();
}
ETime::~ETime(){
    timer.stop();
    SourceDate.clear();
}

