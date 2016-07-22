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
#include "sourcetable.h"

SourceTable::SourceTable(QWidget *parent) : QWidget(parent){
    Source=NULL;
    lb=new QHBoxLayout(this);
}
void SourceTable::setSource(QWidget *w){
    lb->removeWidget(Source);
    DELETE(Source);
    Source=w;
    lb->addWidget(Source);
    emit SourceChanged(Source);
}
bool SourceTable::isEmpty()const{
    return Source==NULL;
}
QWidget* SourceTable::getSource()const{
    return Source;
}
SourceTable::~SourceTable(){
    DELETE(Source);
}
