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
#include "readarchiv.h"

ReadArchiv::ReadArchiv(const QString &str, QWidget *ptr):
    QDialog(ptr)
{
    patch=str;
    this->setModal(true);
    ok=new QPushButton("ok");
    list=new QComboBox();
    QDir d(patch);
    for(QFileInfo s :d.entryInfoList(QDir::Dirs)){
        if(test(s.fileName()))
            list->addItem(s.fileName());
    }
    QHBoxLayout *temp=new QHBoxLayout;
    temp->addWidget(list);
    temp->addWidget(ok);
    connect(ok,SIGNAL(clicked(bool)),this,SLOT(okDown(bool)));
    this->setLayout(temp);
}
bool ReadArchiv::test(const QString &str){
    QRegExp er("[0-9]{2}.*.[0-9]{4}");
    return er.exactMatch(str);
}
void ReadArchiv::okDown(bool){
    QDir d(patch);
    d.cd(list->currentText());
    QFileInfoList temp=d.entryInfoList(QStringList()<<"*.bdh");
    if(temp.size()>0)
        emit Ok(temp.first().absoluteFilePath());
    this->close();
}
