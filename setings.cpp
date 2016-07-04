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
#include "setings.h"

Setings::Setings(DataBase *bd,QWidget *parent):
    QDialog(parent)
{
    if(bd==NULL){
        this->close();
    }
    Bd=bd;
    this->setModal(true);
    QVBoxLayout *vbox=new QVBoxLayout(this);
    QHBoxLayout *hbox=new QHBoxLayout();
    hbox->addWidget(new QLabel("Первичный путь к базе данных "));
    patch=new QLineEdit(this);
    patch->setText(bd->getPatch());
    hbox->addWidget(patch);
    patchButton=new QPushButton("Выбрать");
    hbox->addWidget(patchButton);
    vbox->addLayout(hbox);
    hbox=new QHBoxLayout();
    hbox->addWidget(new QLabel("Путь к архиву базе данных"));
    patchArhov=new QLineEdit();
    patchArhov->setText(bd->getArhivePatch());
    hbox->addWidget(patchArhov);
    ArhivButton=new QPushButton("Выбрать");
    hbox->addWidget(ArhivButton);
    autoSave=new QCheckBox("Автосохранение Базы данных",this);
    autoSave->setCheckState((Qt::CheckState)(Bd->AutoSave*2));
    //autoGroup=new QCheckBox("Автосохранение менеджера групп",this);
    //autoGroup->setCheckState((Qt::CheckState)(Bd->getAutosaveGroup()*2));
    autoSem=new QCheckBox("Автоматическое ведение отчётов в семестрах (БЕТА)",this);
    autoSem->setCheckState((Qt::CheckState)(Bd->getAutoSem()*2));
    dateB1=new DateWidget(false,this);
    dateB1->setDate(Bd->Dates()[0]);
    dateE1=new DateWidget(false,this);
    dateE1->setDate(Bd->Dates()[1]);
    dateB2=new DateWidget(false,this);
    dateB2->setDate(Bd->Dates()[2]);
    dateE2=new DateWidget(false,this);
    dateE2->setDate(Bd->Dates()[3]);

    autoSemChang((int)autoSem->checkState());
    vbox->addLayout(hbox);
    vbox->addWidget(autoSave);
    //vbox->addWidget(autoGroup);
    vbox->addWidget(autoSem);

    hbox=new QHBoxLayout(this);
    hbox->addWidget(new QLabel("Начало семестра (полугодие 1)"));
    hbox->addWidget(dateB1);
    vbox->addLayout(hbox);

    hbox=new QHBoxLayout(this);
    hbox->addWidget(new QLabel("Конец семестра (полугодие 1)"));
    hbox->addWidget(dateE1);
    vbox->addLayout(hbox);

    hbox=new QHBoxLayout(this);
    hbox->addWidget(new QLabel("Начало семестра (полугодие 2)"));
    hbox->addWidget(dateB2);
    vbox->addLayout(hbox);

    hbox=new QHBoxLayout(this);
    hbox->addWidget(new QLabel("Конец семестра (полугодие 2)"));
    hbox->addWidget(dateE2);
    vbox->addLayout(hbox);
    passChang=new QPushButton("Установить новый пароль безопасности");
    vbox->addWidget(passChang);
    passClear=new QPushButton("Удалить пароль безопасности");
    vbox->addWidget(passClear);
    passClear->setEnabled(Bd->getPass()!="");
    hbox=new QHBoxLayout(this);
    cancle=new QPushButton("отмена",this);
    hbox->addWidget(cancle);
    default_=new QPushButton("по умолчанию",this);
    hbox->addWidget(default_);
    complit=new QPushButton("Применить",this);
    hbox->addWidget(complit);
    vbox->addLayout(hbox);
    connect(ArhivButton,SIGNAL(clicked(bool)),this,SLOT(ArhiveButtonClick(bool)));
    connect(passChang,SIGNAL(clicked(bool)),this,SLOT(passChanged(bool)));
    connect(passClear,SIGNAL(clicked(bool)),this,SLOT(passDelete(bool)));
    connect(patchButton,SIGNAL(clicked(bool)),this,SLOT(patchButtonClick(bool)));
    //connect(patch,SIGNAL(),this,SLOT(textChanged(QString)));
    connect(complit,SIGNAL(clicked(bool)),this,SLOT(autocomplitClick(bool)));
    connect(default_,SIGNAL(clicked(bool)),this,SLOT(autodefault_Click(bool)));
    connect(cancle,SIGNAL(clicked(bool)),this,SLOT(autocancleClick(bool)));
    connect(autoSem,SIGNAL(stateChanged(int)),this,SLOT(autoSemChang(int)));
    this->setLayout(vbox);
}
void Setings::ArhiveButtonClick(bool){
    QString temp=QFileDialog::getExistingDirectory(this);
    if(temp!=""){
        patchArhov->setText(temp);
    }
}
void Setings::write(const QString&str){
    //if(str!=STR_NULL){
        QFile f("settings");
        f.open(QIODevice::WriteOnly|QIODevice::Truncate);
        QDataStream stream(&f);
        stream<<str;
        f.close();
   // }
}
void Setings::passDelete(bool){
    Bd->setPass("");
    passClear->setEnabled(false);
}
QString Setings::read(){
    QFile f("settings");
    QString temp="Error";
    if(f.open(QIODevice::ReadOnly)){
        QDataStream stream(&f);
        stream>>temp;
        f.close();
    }
    return temp;
}
void Setings::patchButtonClick(bool){
    QString temp=QFileDialog::getOpenFileName(this,patch->text(),patch->text(),"*.bdh");
    if(temp!=""){
        patch->setText(temp);
    }
}
void Setings::passChanged(bool){
    QString temp= QInputDialog::getText(this,"Новый пароль","введите новый пароль или оставьте строку пустой что бы отключить его");
    if(temp!=""){
        Bd->setPass(temp);
        passClear->setEnabled(true);
    }
}
void Setings::autoSemChang(int i){
    dateB1->setEnabled(i==2);
    dateB2->setEnabled(i==2);
    dateE1->setEnabled(i==2);
    dateE2->setEnabled(i==2);
}
void Setings::autocomplitClick(bool){
    Bd->AutoSave=(autoSave->checkState()==Qt::Checked);
    Bd->setPatch(patch->text());
    Bd->setArhivePatch(patchArhov->text());
    //Bd->setAutosaveGroup(autoGroup->checkState()==Qt::Checked);
    Bd->setAutoSem(autoSem->checkState()==Qt::Checked);
    if(Bd->getAutoSem()){
        Bd->Dates()[0]=dateB1->getDate();
        Bd->Dates()[1]=dateE1->getDate();
        Bd->Dates()[2]=dateB2->getDate();
        Bd->Dates()[3]=dateE2->getDate();
    }
    Setings::write(patch->text());
    this->close();
}
void Setings::autodefault_Click(bool){
    autoSave->setCheckState(Qt::Checked);
    //autoGroup->setCheckState(Qt::Unchecked);
    autoSem->setCheckState(Qt::Unchecked);
    patch->setText("/DataBase/");
    patchArhov->setText("./");
    //Bd->AutoSave=false;
}
void Setings::autocancleClick(bool){
    this->close();
}

Setings::~Setings(){
    Setings::write(patch->text());
}

