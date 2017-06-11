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
    stream<<object.defaultDatabase;
    stream<<object.connect;
    return stream;
}
QDataStream& operator>>(QDataStream&stream,settings& object){
    stream>>object.pass;
    stream>>object.host;
    stream>>object.port;
    stream>>object.user;
    stream>>object.defaultDatabase;
    stream>>object.connect;
    return stream;
}
void settings::default_(){
    pass=defaultPass;
    user=defaultUser;
    host=defaultHost;
    port=defaultPORT;
    defaultDatabase=defaultDDB;
    connect=defaultConnect;
}
settings& settings::operator=(const settings& right){
    this->host=right.host;
    this->pass=right.pass;
    this->user=right.user;
    this->port=right.port;
    this->defaultDatabase=right.defaultDatabase;
    this->connect=right.connect;
    return *this;
}
Settings::Settings(sqlDataBase *bd, QWidget *parent):
    QDialog(parent)
{
    Bd=bd;
    conf=Settings::readConf();
    this->setModal(true);
    QVBoxLayout *vbox=new QVBoxLayout();
    QHBoxLayout *box=new QHBoxLayout();
    QVBoxLayout *hboxr=new QVBoxLayout();
    QVBoxLayout *hboxl=new QVBoxLayout();

    hboxl->addWidget(new QLabel(ELanguage::getWord(USER_LOGIN)));
    hboxl->addWidget(new QLabel(ELanguage::getWord(USER_PASSS)));
    hboxl->addWidget(new QLabel(ELanguage::getWord(USER_HOST)));
    hboxl->addWidget(new QLabel(ELanguage::getWord(USER_PORT)));
    hboxl->addWidget(new QLabel(ELanguage::getWord(DEFAULT_DATABASE)));
    hboxl->addWidget(new QLabel(ELanguage::getWord(AUTO_CONNECT)));


    Login=new QLineEdit(conf.user);
    hboxr->addWidget(Login);

    pass=new QLineEdit(conf.pass);
    pass->setEchoMode(QLineEdit::Password);
    pass->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    hboxr->addWidget(pass);

    host=new QLineEdit(conf.host);
    hboxr->addWidget(host);

    port=new QLineEdit(conf.port);
    hboxr->addWidget(port);

    defaultDataBase=new QLineEdit(conf.defaultDatabase);
    hboxr->addWidget(defaultDataBase);

    autoConnect=new QCheckBox();
    autoConnect->setCheckState((Qt::CheckState)(conf.connect*2));
    hboxr->addWidget(autoConnect);

    QHBoxLayout *hbox=new QHBoxLayout();
    cancle=new QPushButton(ELanguage::getWord(BUTTON_CANCLE));
    connect(cancle,SIGNAL(clicked(bool)),this,SLOT(cancleClick(bool)));
    hbox->addWidget(cancle);

    default_=new QPushButton(ELanguage::getWord(BUTTON_DEFAULT));
    connect(default_,SIGNAL(clicked(bool)),this,SLOT(defaultClick(bool)));
    hbox->addWidget(default_);

    complit=new QPushButton(ELanguage::getWord(BUTTON_OK));
    connect(complit,SIGNAL(clicked(bool)),this,SLOT(complitClick(bool)));
    hbox->addWidget(complit);
    box->addLayout(hboxl);
    box->addLayout(hboxr);
    vbox->addLayout(box);
    vbox->addLayout(hbox);
    this->setLayout(vbox);
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
    conf.defaultDatabase=defaultDataBase->text();
    conf.connect=autoConnect->isChecked();
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
    defaultDataBase->setText(conf.defaultDatabase);
    autoConnect->setCheckState((Qt::CheckState)(conf.connect*2));
}
void Settings::cancleClick(bool){
    this->close();
}
/*void Settings::update(){
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
    //update();
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
}*/
Settings::~Settings(){
}

