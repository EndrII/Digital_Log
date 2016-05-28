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
#include "about.h"

About::About(QWidget *ptr):QDialog(ptr)
{
    this->setModal(true);
    aboutLogo=new QLabel();

    QFile f(":/help/About.html");
    if(f.open(QIODevice::ReadOnly)){
        QString temp;
        QTextStream stream(&f);
        temp= stream.readAll();
        aboutLogo->setText(temp);
        f.close();
    }
    QHBoxLayout *l=new QHBoxLayout();
    l->addWidget(aboutLogo);
    this->setLayout(l);
    this->setMinimumSize(650,550);
    this->setMaximumSize(650,550);
    //connect(aboutLogo,SLOT())

}
