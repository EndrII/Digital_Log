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
#include "settings.h"
QDataStream& operator<<(QDataStream&stream,const settings& object){
    stream<<object.pass;
    stream<<object.host;
    stream<<object.port;
    stream<<object.user;
    return stream;
}
QDataStream& operator>>(QDataStream&stream,settings& object){
    stream>>object.pass;
    stream>>object.host;
    stream>>object.port;
    stream>>object.user;
    return stream;
}
void settings::default_(){
    pass=defaultPass;
    user=defaultUser;
    host=defaultHost;
    port=defaultPORT;
}
settings& settings::operator=(const settings& right){
    this->host=right.host;
    this->pass=right.pass;
    this->user=right.user;
    this->port=right.port;
    return *this;
}
Settings::Settings(sqlDataBase *bd, QWidget *parent):
    QDialog(parent)
{
    Bd=bd;
    conf=Settings::readConf();
    this->setModal(true);
    QVBoxLayout *vbox=new QVBoxLayout();
    QHBoxLayout *hbox=new QHBoxLayout();
    hbox->addWidget(new QLabel(ELanguage::getWord(USER_LOGIN)));
    Login=new QLineEdit(conf.user);
    hbox->addWidget(Login);
    vbox->addLayout(hbox);
    hbox=new QHBoxLayout();
    hbox->addWidget(new QLabel(ELanguage::getWord(USER_PASSS)));
    pass=new QLineEdit(conf.pass);
    hbox->addWidget(pass);
    vbox->addLayout(hbox);
    createContextMenu();


    hbox=new QHBoxLayout();
    hbox->addWidget(new QLabel(ELanguage::getWord(USER_HOST)));
    host=new QLineEdit(conf.host);
    hbox->addWidget(host);
    vbox->addLayout(hbox);
    hbox=new QHBoxLayout();
    hbox->addWidget(new QLabel(ELanguage::getWord(USER_PORT)));
    port=new QLineEdit(conf.port);
    hbox->addWidget(port);
    vbox->addLayout(hbox);
    quer=Bd->registration();
    model=new QSqlQueryModel();
    table=new QTableView();
    table->resizeColumnToContents(0);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setColumnWidth(1,100);

    table->setModel(model);
    vbox->addWidget(new QLabel(ELanguage::getWord(DATABASE_SETTINGS)));
    vbox->addWidget(table);

    hbox=new QHBoxLayout();
    cancle=new QPushButton(ELanguage::getWord(BUTTON_CANCLE));
    connect(cancle,SIGNAL(clicked(bool)),this,SLOT(cancleClick(bool)));
    hbox->addWidget(cancle);

    default_=new QPushButton(ELanguage::getWord(BUTTON_DEFAULT));
    connect(default_,SIGNAL(clicked(bool)),this,SLOT(defaultClick(bool)));
    hbox->addWidget(default_);

    complit=new QPushButton(ELanguage::getWord(BUTTON_OK));
    connect(complit,SIGNAL(clicked(bool)),this,SLOT(complitClick(bool)));
    hbox->addWidget(complit);
    vbox->addLayout(hbox);

    this->setLayout(vbox);

    update();

}
settings Settings::readConf(const QString &patch){
    QFile f(patch);
    settings result;
    if(f.open(QIODevice::ReadOnly)){
        QDataStream stream(&f);
        stream>>result;
        f.close();
    }else{
        result.default_();
    }
    return result;
}
void Settings::writeConf(settings* conf,const QString &patch){
    QFile f(patch);
    if(f.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        QDataStream stream(&f);
        stream<<*conf;
        f.close();
    }else{
        qDebug()<<"Config File Error!";
    }
}
void Settings::complitClick(bool){
    conf.user=Login->text();
    conf.pass=pass->text();
    conf.host=host->text();
    conf.port=port->text();
    Settings::writeConf(&conf);
    Bd->connect_to(conf.user,conf.pass,conf.host,conf.port);
    this->close();
}
void Settings::defaultClick(bool){
    conf.default_();
    host->setText(conf.host);
    port->setText(conf.port);
    Login->setText(conf.user);
    pass->setText(conf.pass);

    Bd->Query_no_update("update config_flags "
                        "set flag=0 where id=1");
    Bd->Query_no_update("update config_flags "
                        "set flag=1 where id=2");
    Bd->Query_no_update("update config_flags "
                        "set flag=6 where id=3");
    update();
}
void Settings::cancleClick(bool){
    this->close();
}
void Settings::update(){
    quer->exec("select Описание as '"+ELanguage::getWord(PROPERTY)+"',flag as'"+
               ELanguage::getWord(VALUE)+"' from  config_flags");
    model->setQuery(*quer);
}
void Settings::change(bool){
    for(QModelIndex index :table->selectionModel()->selectedIndexes()){
        Bd->Query_no_update("update config_flags "
                            "set flag="+QString::number(QInputDialog::getInt(this,ELanguage::getWord(CHANGE),
                                                              ELanguage::getWord(CHANGE_ABOUT)))+
                            " where flag="+table->model()->data(index).toString());
    }
    update();
}
void Settings::createContextMenu(){
    change_=new QAction(ELanguage::getWord(CHANGE));
    change_->setStatusTip(ELanguage::getWord(CHANGE_ABOUT));
    connect(change_,SIGNAL(triggered(bool)),this,SLOT(change(bool)));
}
void Settings::contextMenuEvent(QContextMenuEvent *event){
    QMenu menu(this);
    menu.addAction(change_);
    menu.exec(event->globalPos());
}
Settings::~Settings(){
    delete quer;
    delete model;
    delete change_;
}

