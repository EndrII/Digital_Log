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
#include "help.h"

Help::Help(QWidget *ptr):QDialog(ptr){

        this->setModal(true);
        aboutLogo=new QTextBrowser();
        QFile f(":/help/index.html");
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
        this->setMinimumSize(950,510);
        //this->setMaximumSize(650,510);
        //connect(aboutLogo,SLOT())
        connect(aboutLogo,SIGNAL(anchorClicked(QUrl)),this,SLOT(anchers(QUrl)));
}
void Help::anchers(QUrl u){
    QString temp=":/help/"+u.url();
    QFile f(temp);
    if(f.open(QIODevice::ReadOnly)){
        QTextStream stream(&f);
        aboutLogo->setText(stream.readAll());
        f.close();
    }
    //aboutLogo->setSource(QUrl(temp));
}
