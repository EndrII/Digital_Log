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
#include "datewidget.h"
DateWidget::DateWidget(bool year, QWidget *parent):QWidget(parent){
    date.setDate(0,0,0);
    QHBoxLayout *lay=new QHBoxLayout(this);
    day=new QSpinBox(this);
    day->setMinimum(1);
    day->setMaximum(31);
    day->setValue(date.day());
    mon=new QSpinBox(this);
    mon->setMinimum(1);
    mon->setMaximum(12);
    mon->setValue(date.month());
    lay->addWidget(day);
    lay->addWidget(new QLabel(":"));
    lay->addWidget(mon);
    if(year){
        god=new QSpinBox(this);
        god->setMinimum(0);
        god->setMinimumWidth(100);
        god->setValue(date.year());
        lay->addWidget(new QLabel(":"));
        lay->addWidget(god);
        connect(god,SIGNAL(valueChanged(int)),this,SLOT(changed(int)));
    }else{
        god=NULL;
    }
    connect(mon,SIGNAL(valueChanged(int)),this,SLOT(changed(int)));
    connect(day,SIGNAL(valueChanged(int)),this,SLOT(changed(int)));
    this->setLayout(lay);
}
DateWidget::DateWidget(const QDate &Date, bool year, QWidget *parent) : QWidget(parent)
{
    date=Date;
    QHBoxLayout *lay=new QHBoxLayout(this);
    day=new QSpinBox(this);
    day->setMinimum(1);
    day->setMaximum(31);
    day->setValue(date.day());
    mon=new QSpinBox(this);
    mon->setMinimum(1);
    mon->setMaximum(12);
    mon->setValue(date.month());
    lay->addWidget(day);
    lay->addWidget(new QLabel(":"));
    lay->addWidget(mon);
    if(year){
        god=new QSpinBox(this);
        god->setMinimum(0);
        god->setMinimumWidth(100);
        god->setValue(date.year());
        lay->addWidget(new QLabel(":"));
        lay->addWidget(god);
        connect(god,SIGNAL(valueChanged(int)),this,SLOT(changed(int)));
    }else{
        god=NULL;
    }
    connect(mon,SIGNAL(valueChanged(int)),this,SLOT(changed(int)));
    connect(day,SIGNAL(valueChanged(int)),this,SLOT(changed(int)));
    this->setLayout(lay);
}
QDate DateWidget::getDate(){
    return this->date;
}
void DateWidget::setDate(const QDate& src){
    date=src;
    if(god!=NULL)
        god->setValue(date.year());
    mon->setValue(date.month());
    day->setValue(date.day());
}
void DateWidget::changed(int){
    date.setDate((god==NULL)?1:god->value(),mon->value(),day->value());
    emit valumeChanged(date);
    if(god!=NULL)
        god->setValue(date.year());
    mon->setValue(date.month());
    day->setValue(date.day());
}
DateWidget::~DateWidget(){

}
