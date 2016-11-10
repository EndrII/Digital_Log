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

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    grm=NULL;
    conf=Settings::readConf();
    QVBoxLayout *temp=new QVBoxLayout();
    bd=new sqlDataBase();
    state=new QLabel("<center>"+ELanguage::getWord(OFFLINE)+"!<center>");
    state->setMaximumHeight(20);
    state->setStyleSheet("background-color : red;");
    temp->addWidget(state);
    tab=new QTabWidget(this);
    LogPanel=new QTextEdit();
    LogPanel->setMaximumWidth(300);
    LogPanel->setMinimumWidth(300);
    LogPanel->setReadOnly(true);
    QHBoxLayout *temp2=new QHBoxLayout();
    temp2->addWidget(LogPanel);
    temp2->addWidget(tab);
    temp->addLayout(temp2);
    MS=new StarastaMode(bd,this);
    tab->addTab(MS,"Староста");
    QWidget *widget=new QWidget;
    widget->setLayout(temp);
    this->setCentralWidget(widget);
    connect(bd,SIGNAL(Message(int,QString)),this,SLOT(Error(int,QString)));
    connect(bd,SIGNAL(stateChanged(state_BD)),this,SLOT(stateChanged(state_BD)));
    createMenu();
}
void MainWindow::createMenu(){
    connect_m=new QAction(ELanguage::getWord(RECONNECT),this);
    this->menuBar()->addAction(connect_m);
    connect(connect_m,SIGNAL(triggered(bool)),this,SLOT(connect_bd(bool)));

    grs=new QAction(ELanguage::getWord(GROUP_MG),this);
    this->menuBar()->addAction(grs);
    connect(grs,SIGNAL(triggered(bool)),this,SLOT(GRM(bool)));

    bases=new QAction(ELanguage::getWord(DATA_BASES),this);
    bases->setStatusTip(ELanguage::getWord(DATA_BASES_ABOUT));
    this->menuBar()->addAction(bases);
    connect(bases,SIGNAL(triggered(bool)),this,SLOT(BASES(bool)));

    mset=new QAction(ELanguage::getWord(DATABASE_SETTINGS),this);
    this->menuBar()->addAction(mset);
    connect(mset,SIGNAL(triggered(bool)),this,SLOT(setings(bool)));
}
void MainWindow::stateChanged(state_BD stat){
    switch (stat) {
    case disconected:
        state->setText("<center>"+ELanguage::getWord(OFFLINE)+"!<center>");
        state->setMaximumHeight(20);
        state->setStyleSheet("background-color : red;");
        break;
    case stoped:
        state->setText("<center>"+ELanguage::getWord(STOPED)+"!<center>");
        state->setMaximumHeight(20);
        state->setStyleSheet("background-color : #F4A460;");
        break;
    case conectd:
        state->setText("<center>"+ELanguage::getWord(ONLINE)+"!<center>");
        state->setMaximumHeight(20);
        state->setStyleSheet("background-color : #FFD700;");
        break;
    default:
        break;
    }
}

/*void MainWindow::AchiveOk(QString ){
}*/
void MainWindow::about(bool){
}
void MainWindow::help(bool){
}
void MainWindow::BASES(bool){
    (new DataBasesMenager(bd,this))->show();
}
void MainWindow::GRM(bool){
    (new GroupMenager(bd,this))->show();
}
void MainWindow::setings(bool){
    (new Settings(bd,this))->show();
}
void MainWindow::connect_bd(bool){
    conf= Settings::readConf();
    bd->connect_to(conf.user,conf.pass,conf.host,conf.port);
}



void MainWindow::Error(int i, QString str){
    switch(i){
    case 0:LogPanel->setText(QTime::currentTime().toString("hh:mm:ss")+": <div style='color:red;'>"+str+"</div><br><br>"+LogPanel->toHtml()); break;
    case 1:LogPanel->setText(QTime::currentTime().toString("hh:mm:ss")+": <div>"+str+"</div><br><br>"+LogPanel->toHtml()); break;
    case 2:LogPanel->setText(QTime::currentTime().toString("hh:mm:ss")+": <div style='color:#009ACD;'>"+str+"</div><br><br>"+LogPanel->toHtml()); break;
    default:break;
    }
}
void MainWindow::Exit(bool){
    this->close();
}
MainWindow::~MainWindow()
{
}
