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
    QVBoxLayout *temp=new QVBoxLayout();
    state=new QLabel("<center>База данных не открыта!<center>");
    state->setMaximumHeight(20);
    state->setStyleSheet("background-color : red;");
    temp->addWidget(state);
    bd=new DataBase();
    tab=new QTabWidget(this);
    LogPanel=new QTextEdit();
    LogPanel->setMaximumWidth(300);
    LogPanel->setMinimumWidth(300);
    LogPanel->setReadOnly(true);
    tempPatch="NoArhive";
    QHBoxLayout *temp2=new QHBoxLayout();
    temp2->addWidget(LogPanel);
    temp2->addWidget(tab);
    temp->addLayout(temp2);
    MS=new StarastaMode(bd,this);
    tab->addTab(MS,"Староста");
    QWidget *widget=new QWidget;
    widget->setLayout(temp);
    this->setCentralWidget(widget);
    connect(bd,SIGNAL(StateChanged(StateDataBase)),MS,SLOT(StateChanged(StateDataBase)));
    connect(bd,SIGNAL(StateChanged(StateDataBase)),this,SLOT(StateBDChanged(StateDataBase)));
    connect(bd,SIGNAL(Error(int,QString)),this,SLOT(Error(int,QString)));
    connect(bd,SIGNAL(DataBaseCreated(QString)),this,SLOT(DataBaseCreated(QString)));
    connect(bd,SIGNAL(GroupSaved(GroupVoid*)),MS,SLOT(GroupSaved(GroupVoid*)));
    connect(bd,SIGNAL(GroupChanged(Group*)),MS,SLOT(GroupChanged(Group*)));
    connect(bd,SIGNAL(GroupOpened(GroupVoid*)),MS,SLOT(GroupOpened(GroupVoid*)));
    connect(bd,SIGNAL(GroupCreated(Group*)),MS,SLOT(GroupCreated(Group*)));
    connect(bd,SIGNAL(controlGroupCreated(GroupVoid*)),MS,SLOT(controlGroupCreated(GroupVoid*)));
    connect(bd,SIGNAL(controlGroupDeleted(Group*)),this,SLOT(ControlGroupDeleted(Group*)));
    connect(bd,SIGNAL(DataBaseOpened(QString)),MS,SLOT(BDOpened(QString)));
    connect(bd,SIGNAL(GroupDeleted(QString)),MS,SLOT(GroupDeleted(QString)));
    CreateMenu();
    bd->open(Setings::read());
}
void MainWindow::createGroupMenu(){
    grs=new QAction("Менеджер групп",this);
    //temp1->setShortcut(QKeySequence::Bold);
    grs->setStatusTip("Управление группами");
    this->menuBar()->addAction(grs);
    connect(grs,SIGNAL(triggered(bool)),this,SLOT(GRM(bool)));
}
void MainWindow::StateBDChanged(StateDataBase stat){
    switch (stat) {
    case notOpened:
        state->setText("<center>База данных не открыта!<center>");
        state->setStyleSheet("background-color : red;");
        arh->setEnabled(false);
        sv->setEnabled(false);
        star->setEnabled(false);
        mset->setEnabled(false);
        grs->setEnabled(false);
        crea->setEnabled(true);
        closeA->setEnabled(false);
        break;
    case Arhive:
        state->setText("<center>База данных является архивом!<center>");
        state->setStyleSheet("background-color : #F4A460;");
        arh->setEnabled(false);
        sv->setEnabled(false);
        star->setEnabled(false);
        mset->setEnabled(false);
        grs->setEnabled(false);
        crea->setEnabled(false);
        closeA->setEnabled(true);
        break;
    case notStarted:
        state->setText("<center>База данных не ведёт отчёт!<center>");
        state->setStyleSheet("background-color : #FFD700;");
        arh->setEnabled(false);
        sv->setEnabled(true);
        star->setEnabled(true);
        mset->setEnabled(true);
        grs->setEnabled(true);
        crea->setEnabled(true);
        closeA->setEnabled(false);
        break;
    case Started:
        state->setText("<center>Ведение отчётов запущено!<center>");
        state->setStyleSheet("background-color : #00FF7F;");
        arh->setEnabled(true);
        sv->setEnabled(true);
        star->setEnabled(true);
        mset->setEnabled(true);
        grs->setEnabled(true);
        crea->setEnabled(true);
        openarh->setEnabled(true);
        closeA->setEnabled(false);
        break;
    default:
        break;
    }
}
void MainWindow::CreateLogMenu(){
    QMenu * temp1=new QMenu("Архив",this);
    temp1->setStatusTip("Просмотр данных в архиве");
    this->menuBar()->addMenu(temp1);

    openarh=new QAction("Прочесть",this);
    openarh->setShortcut(QKeySequence::Redo);
    openarh->setStatusTip("Открыть архив");
    connect(openarh,SIGNAL(triggered(bool)),this,SLOT(arhive(bool)));
    temp1->addAction(openarh);

    closeA=new QAction("Закрыть",this);
    closeA->setShortcut(QKeySequence::Redo);
    closeA->setStatusTip("Вернуться к текущей базе даннных");
    connect(closeA,SIGNAL(triggered(bool)),this,SLOT(closeArhiv(bool)));
    temp1->addAction(closeA);

    mset=new QAction("Настройки",this);
    mset->setStatusTip("Настройки базы данных");
    this->menuBar()->addAction(mset);
    connect(mset,SIGNAL(triggered(bool)),this,SLOT(setings(bool)));

    QAction *temp2=new QAction("О DigitalLog",this);
    this->menuBar()->addAction(temp2);
    connect(temp2,SIGNAL(triggered(bool)),this,SLOT(about(bool)));

    temp2=new QAction("Справка",this);
    temp2->setShortcut(QKeySequence::HelpContents);
    this->menuBar()->addAction(temp2);
    connect(temp2,SIGNAL(triggered(bool)),this,SLOT(help(bool)));

}
void MainWindow::closeArhiv(bool){
    if(tempPatch.size()!=0){
        bd->open(tempPatch);
        tempPatch="NoArhive";
    }
}
void MainWindow::setings(bool){
    if(bd->security()){
        st=new Setings(bd,this);
        st->show();
    }else{
        QMessageBox::information(this,"Безопасность","Введен не верный пароль!");
    }
}
void MainWindow::GRM(bool){
    if(bd->security()){
        grm=new GroupMenager(bd,this);
        grm->show();
    }else{
        QMessageBox::information(this,"Безопасность","Введен не верный пароль!");
    }
}
void MainWindow::CreateInterface(){

}
void MainWindow::CreateMenu(){
    this->createFileMenu();
    this->createGroupMenu();
    this->CreateLogMenu();
}
void MainWindow::toArhiv(bool){
    bd->endTime();
}
void MainWindow::AchiveOk(QString str){
    if(tempPatch=="NoArhive")
        tempPatch=bd->getPatch();
    if(str!=""){
        /*int temp = QMessageBox::question(this, "Предуприждение","открытая база данных будет закрыта, сохранить все изменения?",
                                      QMessageBox::No|QMessageBox::Yes);
        if(temp== QMessageBox::Yes)
            bd->saveAll();*/
        bd->open(str);
    }
}
void MainWindow::arhive(bool){
    ReadArchiv *ar=new ReadArchiv(bd->getArhivePatch(),this);
    connect(ar,SIGNAL(Ok(QString)),this,SLOT(AchiveOk(QString)));
    ar->show();
}
void MainWindow::createFileMenu(){
    QMenu *temp= new QMenu("Файл");
    crea=new QAction("Создать",this);
    crea->setShortcut(QKeySequence::New);
    crea->setStatusTip("Создаст новую базу данных");
    connect(crea,SIGNAL(triggered(bool)),this,SLOT(CreateDataBase(bool)));
    temp->addAction(crea);
    QAction * temp1=new QAction("Открыть",this);
    temp1->setShortcut(QKeySequence::Open);
    temp1->setStatusTip("Откроет базу данных");
    connect(temp1,SIGNAL(triggered(bool)),this,SLOT(OpenDB(bool)));
    temp->addAction(temp1);
    sv=new QAction("Сохранить",this);
    sv->setShortcut(QKeySequence::Save);
    sv->setStatusTip("Сохранит всё базу данных");
    connect(sv,SIGNAL(triggered(bool)),this,SLOT(Save(bool)));
    temp->addAction(sv);
    star=new QAction("Запустить",this);
    star->setShortcut(QKeySequence::Refresh);
    star->setStatusTip("Запустить автоматический отчёт времени в базе данных");
    connect(star,SIGNAL(triggered(bool)),this,SLOT(start(bool)));
    temp->addAction(star);
    arh=new QAction("Записать в Архив",this);
    arh->setShortcut(QKeySequence::Bold);
    arh->setStatusTip("Переместить текущюю базу данных в архив (архив изменить нельзя!)");
    connect(arh,SIGNAL(triggered(bool)),this,SLOT(toArhiv(bool)));
    temp->addAction(arh);
    temp1=new QAction("Выход",this);
    temp1->setShortcut(QKeySequence::Close);
    temp1->setStatusTip("Выход");
    connect(temp1,SIGNAL(triggered(bool)),this,SLOT(Exit(bool)));
    temp->addAction(temp1);
    this->menuBar()->addMenu(temp);
}
void MainWindow::Save(bool){
    bd->saveAll();
}
void MainWindow::about(bool){
    About *a=new About(this);
    a->show();
}
void MainWindow::help(bool){
    Help *a=new Help(this);
    a->show();
}
void MainWindow::start(bool){
    bd->startTime();
}
void MainWindow::ControlGroupDeleted(Group *g){
    //QMessageBox::information(this,"Информация","ведение пропусков группы "+g->getName()+" прекращено");
    LogPanel->setText(QTime::currentTime().toString("hh:mm:ss")+": Ведение пропусков группы "+g->getName()+" прекращено<br><br>"+LogPanel->toHtml());
}
void MainWindow::Error(int i, QString str){
    switch(i){
    case -2:LogPanel->setText(QTime::currentTime().toString("hh:mm:ss")+": <div style='color:#009ACD;'>"+str+"</div><br><br>"+LogPanel->toHtml()); break;
    case -1:LogPanel->setText(QTime::currentTime().toString("hh:mm:ss")+": <div>"+str+"</div><br><br>"+LogPanel->toHtml()); break;
    case 0:LogPanel->setText(QTime::currentTime().toString("hh:mm:ss")+": <div style='color:red;'>"+str+"</div><br><br>"+LogPanel->toHtml()); break;
    case 1:LogPanel->setText(QTime::currentTime().toString("hh:mm:ss")+": <div style='color:red;'>"+"Не возможно записать файл \""+str+"\"</div><br><br>"+LogPanel->toHtml()); break;
    case 2:LogPanel->setText(QTime::currentTime().toString("hh:mm:ss")+": <div style='color:red;'>"+"группа не найдена "+str+"</div><br><br>"+LogPanel->toHtml());break;
    case 3:LogPanel->setText(QTime::currentTime().toString("hh:mm:ss")+": <div style='color:red;'>"+"Ошибка открытия группы \""+str+"\". Для данной группы не ведётся отчёт пропусков, для того что бы начать отчёт выберите и нажмите кнопку \"Начать отчёт\". </div><br><br>"+LogPanel->toHtml());break;
    case 4:LogPanel->setText(QTime::currentTime().toString("hh:mm:ss")+": <div style='color:red;'>"+"База данных уже создана \""+str+"\"</div><br><br>"+LogPanel->toHtml());break;
    default:break;
    }
}
void MainWindow::Exit(bool){
    this->close();
}
void MainWindow::CreateDataBase(bool){
    bd->createDataBase(QFileDialog::getSaveFileName(this,"Выберите место расположения и введите имя базе данных","","*.bdh"));
}
void MainWindow::OpenDB(bool){
    bd->open(QFileDialog::getOpenFileName(this,"Выберите базу данных","","*.bdh"));
}
void MainWindow::DataBaseCreated(QString str){
    MS->groups->clear();
    LogPanel->setText(QTime::currentTime().toString("hh:mm:ss")+": База данных создана в "+str+"<br><br>"+LogPanel->toHtml());
}
void MainWindow::addFilter(bool){

}
void MainWindow::clearFilter(bool){

}
void MainWindow::closeEvent(QCloseEvent *)
{
    if(bd->AutoSave){
        if(bd->getState()==notStarted||bd->getState()==Started)
             bd->saveAll();
    }
    else
        if(bd->getState()==notStarted||bd->getState()==Started){
            int temp= QMessageBox::question(NULL, "Предупреждении","открытая база данных будет закрыта, сохранить все изменения?",
                                          QMessageBox::No|QMessageBox::Yes);
            if(temp== QMessageBox::Yes)
                bd->saveAll();
        }
    //grm->close();
}
/*void MainWindow::GroupSaved(GroupVoid *){

}
void MainWindow::GroupChanged(Group *){

}
void MainWindow::GroupOpened(GroupVoid *gr){
    Redraw(gr);
}
void MainWindow::GroupCreated(Group *){

}
void MainWindow::controlGroupCreated(GroupVoid *){

}*/
MainWindow::~MainWindow()
{
}
