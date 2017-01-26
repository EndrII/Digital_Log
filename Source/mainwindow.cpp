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
    //CSS::ReadCss(this,"://ccs/StyleProgramm.css");
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
    MP=new PredmetMode(bd,this);
    tab->addTab(MS,ELanguage::getWord(STEWARD_MOD));
    tab->addTab(MP,ELanguage::getWord(PREDMET_MOD));
    QWidget *widget=new QWidget;
    widget->setLayout(temp);
    this->setCentralWidget(widget);
    connect(bd,SIGNAL(Message(int,QString)),this,SLOT(Error(int,QString)));
    connect(bd,SIGNAL(stateChanged(state_BD)),this,SLOT(stateChanged(state_BD)));
    createMenu();
    if(conf.connect){
        connect_bd(false);
    }
}
void MainWindow::createMenu(){
    file=new QMenu(ELanguage::getWord(FILE_MENU),this);
    this->menuBar()->addMenu(file);
    connect_m=new QAction(ELanguage::getWord(RECONNECT),this);
    file->addAction(connect_m);
    connect(connect_m,SIGNAL(triggered(bool)),this,SLOT(connect_bd(bool)));

    grs=new QAction(ELanguage::getWord(GROUP_MG),this);
    file->addAction(grs);
    connect(grs,SIGNAL(triggered(bool)),this,SLOT(GRM(bool)));

    bases=new QAction(ELanguage::getWord(DATA_BASES),this);
    bases->setStatusTip(ELanguage::getWord(DATA_BASES_ABOUT));
    file->addAction(bases);
    connect(bases,SIGNAL(triggered(bool)),this,SLOT(BASES(bool)));

    mset=new QAction(ELanguage::getWord(DATABASE_SETTINGS),this);
    file->addAction(mset);
    connect(mset,SIGNAL(triggered(bool)),this,SLOT(setings(bool)));

    control=new QMenu(ELanguage::getWord(STATUS_CONTROL),this);
    this->menuBar()->addMenu(control);
    DateManager=new QAction(ELanguage::getWord(DATE_MENAGER),this);
    control->addAction(DateManager);
    connect(DateManager,SIGNAL(triggered(bool)),this,SLOT(dateManager_(bool)));

    /*stop=new QAction(ELanguage::getWord(STOP),this);
    control->addAction(stop);
    connect(stop,SIGNAL(triggered(bool)),this,SLOT(stop_(bool)));*/

    toArhive=new QAction(ELanguage::getWord(ARHIVE),this);
    control->addAction(toArhive);
    connect(toArhive,SIGNAL(triggered(bool)),this,SLOT(toArhiv(bool)));
}
void MainWindow::stateChanged(state_BD stat){
    switch (stat) {
    case disconected:
        state->setText("<center>"+ELanguage::getWord(OFFLINE)+"!<center>");
        state->setMaximumHeight(20);
        state->setStyleSheet("background-color : red;");
        break;
    case stoped:
        state->setText("<center>"+ELanguage::getWord(ONLINE)+"!<center>");
        state->setMaximumHeight(20);
        state->setStyleSheet("background-color : #00FA9A;");
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
void MainWindow::dateManager_(bool){
   (new DateEditor(bd,this))->show();
}
void MainWindow::stop_(bool){
    bd->StopBD();
}
void MainWindow::toArhiv(bool){
    bd->StopBD(true);
}
void MainWindow::connect_bd(bool){
    conf= Settings::readConf();
    bd->connect_to(conf.user,conf.pass,conf.host,conf.port);
    if(!conf.defaultDatabase.isEmpty()){
        bd->openDB(conf.defaultDatabase);
    }
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
