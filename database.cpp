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
#include "database.h"

DataBase::DataBase(const QString & patch_,QObject *ptr):
    QObject(ptr)
{
    patch=patch_;
    ARhivePatch="./";
    pass_="";
    statusBD=notOpened;
    End=false;
    AutoSave=true;
    datesave=new QDate[4];
    time=NULL;
    //open(patch_);
}
void DataBase::PatchCorect(){
    if(patch.indexOf(HEADER_DB)!=-1){
        patch.remove(HEADER_DB);
    }else{
        int i=patch.size();
        while(patch[i]!='/'&&i>0){
            if(patch[i]=='.'){
                patch.remove(i,patch.size()-i);
                return;
            }
            i--;
        }
    }
}
bool DataBase::writeDs(QString str){
    if(str=="patch")str=patch;
    QFile f(str+HEADER_DB);
    if(f.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        QDataStream stream(&f);
        if(time!=NULL){
            stream<<*time;
        }
            else{
            throw EError("time is not detected ","DataBase::writeDs");
            return false;
        }
        stream<<End;
        stream<<(bool)AutoSave;
        stream<<pass_;
        stream<<ARhivePatch;
        stream<<autoSaveGroup;
        stream<<autoSem;
        if(autoSem)
            stream<<datesave[0]<<datesave[1]<<datesave[2]<<datesave[3];
        //stream<<patch;
        stream<<(int)descript.size();
        for(BPair i:descript){
            stream<<*i.first;
        }
        f.close();
        return true;
    }
    else{
        throw EError("write HeiderError!","DataBase::writeDs");
        return false;
    }
}
bool DataBase::readDS(QString str){
    if(str=="patch")str=patch;
    QFile f(str+HEADER_DB);
    if(f.open(QIODevice::ReadOnly)){
        QDataStream stream(&f);
        descript.clear();
        int temp;
       // DELETE(time);
        if(time==NULL){
            time=new ETime();
            time->attach(datesave,&autoSem);
            connect(time,SIGNAL(statusChanget(bool)),this,SLOT(timerSchanged(bool)));
        }
        stream>>(*time);
        stream>>End;
        stream>>AutoSave;
        stream>>pass_;
        stream>>ARhivePatch;
        stream>>autoSaveGroup;
        stream>>autoSem;
        if(autoSem)
            stream>>datesave[0]>>datesave[1]>>datesave[2]>>datesave[3];
        stream>>temp;
        while(temp--){
            descript.push_back(BPair(new Group,NULL));
            stream>>*descript.last().first;
            connect(descript.last().first,SIGNAL(changed(Group*)),this,SLOT(groupChangedDeligate(Group*)));
        }
        f.close();
        return true;
    }
    else{
        return false;
    }
}
void DataBase::groupChangedDeligate(Group *gr){
    emit GroupChanged(gr);
}
void DataBase::timerSchanged(bool b){
    if(b)
        emit StateChanged(statusBD=Started);
    else
        emit StateChanged(statusBD=notStarted);
}
void DataBase::closeAll(){
    for(BPair i:descript){
        delete i.second;
        delete i.first;
    }
    descript.clear();
}
void DataBase::open(const QString &patch_){

    if(patch_!="")patch=patch_;
    PatchCorect();
    closeAll();
    if(!readDS()){
        emit Error(0,"Не удалось открыть Базу данных по пути "+patch);
        emit StateChanged(statusBD);
    }else{
        if(time->isStarted())
            emit StateChanged(statusBD=Started);
        else{
            emit StateChanged(statusBD=StateDataBase(2-this->End));
        }
        emit DataBaseOpened(patch);    }
}
bool DataBase::writeBD(const QString &groupID){
    //*******************************
    if((int)statusBD<2) return false;
    GroupVoid *temp=NULL;
    try{
        temp=find_pointer(groupID);
    }catch(int){
        emit Error(2,groupID);
        return NULL;
    }
    if(temp==NULL){
        if(!temp->Write(PATCH_NAME_S(groupID)))
            emit Error(1,groupID);
        else{
            return true;
        }
    }
    return false;
}
bool DataBase::writeBD(Group *gr){
    //*******************************
    if(gr!=NULL)
        return writeBD(gr->getName());
    else
        return false;
}
void DataBase::writeAllBD(){
    if((int)statusBD<2) return;
    for(BPair p:descript){
        if(p.second!=NULL){
            if(!p.second->Write(PATCH_NAME_S(p.second->getName())))
                emit Error(1,p.second->getName());
        }
    }
}
GroupVoid* DataBase::readBD(const QString &groupID){
    GroupVoid *temp=NULL;
    try{
        temp=find_pointer(groupID);
    }catch(int){
        emit Error(2,groupID);
        return NULL;
    }
    QFile f(PATCH_NAME_S(groupID));
    if(f.open(QIODevice::ReadOnly)){
        temp=new GroupVoid(descript[find_index(groupID)].first,this->time);
        QDataStream stream(&f);
        stream>>(*temp);
        f.close();
    }else{
        emit Error(3,groupID);
        return NULL;
    }
    return temp;
}
GroupVoid* DataBase::readBD(Group* gr){
    if(gr!=NULL)
    return readBD(gr->getName());
    else return NULL;
}
void DataBase::readAllBD(){
    QString tempMessage="";
    for(BPair i: descript){
        if(readBD(i.first)==NULL){
            tempMessage+=i.first->getName()+", ";
        }
    }
    if(tempMessage!=""){
        emit Error(0,"не удалось прочесть следующие группы: "+tempMessage+"возможно для них не видеться журнал.");
    }
}
void DataBase::createDataBase(const QString &name){
    if(name=="") return;
    patch=name;
    PatchCorect();
    if(QFile(patch+HEADER_DB).exists()){
        emit Error(4,patch);
    }else{
        DELETE(time);
        for(BPair i:descript){
            delete i.second;
            delete i.first;
        }
        descript.clear();
        time=new ETime();
        time->attach(datesave,&autoSem);
        connect(time,SIGNAL(statusChanget(bool)),this,SLOT(timerSchanged(bool)));
        writeDs();
        emit StateChanged( statusBD=notStarted);
        emit DataBaseCreated(patch);
    }
}
bool DataBase::isCreated()const{
    return time!=NULL;
}
void DataBase::setPatch(const QString &patch_){
    patch=patch_;
}
QString DataBase::getPatch()const{
    return patch;
}
bool DataBase::find(const QString &name){
    for(BPair i:descript)
        if(i.first->getName()==name)
            return true;
    return false;
}
GroupVoid* DataBase::find_pointer(const QString &name){
    for(BPair i:descript)
        if(i.first->getName()==name)
            return i.second;
    throw 1;
    //return NULL;
}
GroupVoid* DataBase::find_pointer(Group *gr){
    for(BPair i:descript)
        if(i.first==gr)
            return i.second;
    throw 1;
}
int DataBase::find_index(const QString &name){
    for(int i=0;i<descript.size();i++)
        if(descript[i].first->getName()==name)
            return i;
    return -1;
}
int DataBase::find_index(Group*gr){
    for(int i=0;i<descript.size();i++)
        if(descript[i].first==gr)
            return i;
    return -1;
}
void DataBase::startTime(){
    if(this->isCreated()){
        if(!time->start()){
            emit Error(0,"Данная база данных уже начала ведение отчётов");
        }else{
            if(statusBD!=Arhive){
                emit StateChanged(statusBD=Started);
                End=false;
            }else{
                emit Error(0,"Архив нельзя изменить!");
            }
        }
    }else{
        emit Error(0,"База данных не создана или не открыта!");
    }
}
bool DataBase::endTime(){
    if(!time->isStarted()){
        emit Error(0,"База данных не начала отчёт времени!");
        return false;
    }
    QString temp=QDate::currentDate().toString("dd.MMMM.yyyy");
    QDir d(ARhivePatch);
    d.mkdir(temp);
    time->stop();
    End=true;
    emit StateChanged(statusBD=notStarted);
    QString temp2=pass_;pass_="";
    writeDs(ARhivePatch+"/"+temp+"/");
    for(BPair p:descript){
        //if(p.first->getSavedState()==)
        this->openGroup(p.first->getName());
        if(p.second==NULL||!p.second->Write(ARhivePatch+"/"+temp+"/"+"SGR"+p.first->getName())){
            emit Error(0,"Ошибка записи в акхив, вероятно данные группы "+p.first->getName()+" поврежденны");
        }
        this->deleteControlGroup(p.first);
    }
    time->clear();
    End=false;
    pass_=temp2;
    return true;
}
bool DataBase::security()const{
    if(this->getPass()!=""){
        if(QInputDialog::getText(NULL,"безопасность","введите пароль безопасности для входа в настройки базы данных")==this->getPass()){
            return true;
        }else{
            return false;
        }
    }else{
        return true;
    }
}
bool DataBase::getAutosaveGroup() const{
    return autoSaveGroup;
}
void DataBase::setAutosaveGroup(bool b){
    autoSaveGroup=b;
}
bool DataBase::getAutoSem()const{
    return autoSem;
}
void DataBase::setAutoSem(bool b){
    autoSem=b;
}
QDate* DataBase::Dates()const{
    return datesave;
}
int DataBase::getLastPropusk() const{
    for(BPair v:descript){
        if(v.second!=NULL){
            return v.second->getLim();
        }
    }
    return 0;
}
bool DataBase::createGroup(const QStringList& list, const QString &group){

    if(group==""||find(group)||((int)statusBD)<2)
        return false;
   // data.push_back();
    descript.push_back(BPair(new Group(list,group),NULL));
    connect(descript.last().first,SIGNAL(changed(Group*)),this,SLOT(groupChangedDeligate(Group*)));
    emit GroupCreated(descript.last().first);
    return true;
}
bool DataBase::createControlGroup(Group *gr){
    if(gr==NULL||((int)statusBD)<2){
        return false;
    }
    int  index=find_index(gr);
    if(descript[index].second!=NULL||QFile(PATCH_NAME_S(descript[index].first->getName())).exists()){
        return false;
    }
    descript[index].second=new GroupVoid(gr,time);
    descript[index].second->autoClear();
    emit controlGroupCreated(descript[index].second);
    return true;
}
bool DataBase::deleteControlGroup(Group *gr){
    if(gr==NULL||((int)statusBD)<2){
        return false;
    }
    if(QFile(PATCH_NAME_S(gr->getName())).exists()){
        QFile f(PATCH_NAME_S(gr->getName()));
        f.remove();
    }
    int index=find_index(gr);
    if(descript[index].second!=NULL)
        DELETE(descript[index].second);
    emit controlGroupDeleted(gr);
    return true;
}
void DataBase::setPass(const QString &pass){
    pass_=pass;
}
QString DataBase::getPass()const{
    return pass_;
}
bool DataBase::deleteGroup(const QString &group){
    int temp=find_index(group);
    if(temp==-1||((int)statusBD)<2){
        emit Error(2,group);
        return false;
    }else
    {
        int temp=find_index(group);
        deleteControlGroup(descript[temp].first);
        delete descript[temp].first;
        descript.remove(temp,1);
        emit GroupDeleted(group);
        return true;
    }
}
StateDataBase DataBase::getState()const{
    return statusBD;
}
QString DataBase::getArhivePatch()const{
    return ARhivePatch;
}
void DataBase::setArhivePatch(const QString &str){
    if(statusBD!=Arhive)
        ARhivePatch=str;
}
bool DataBase::isEnd()const{
    return End;
}
QVector<BPair>* DataBase::getDataList(){
    return &descript;
}
void DataBase::save(const QString &id){
     if(writeBD(id)||((int)statusBD>1))
        emit GroupSaved(find_pointer(id));
}
void DataBase::saveAll(){
    if(this->isCreated()){
        writeAllBD();
        writeDs();
    }else{
        emit Error(0,"База данных не создана или не открыта!");
    }
}
void DataBase::openGroup(const QString &GroupName){
    int temp=this->find_index(GroupName);
    if(temp!=-1&&descript[temp].second!=NULL)
        emit GroupOpened(descript[temp].second);
    else{
        descript[temp].second = readBD(GroupName);
        if(descript[temp].second!=NULL){
            emit GroupOpened(descript[temp].second);
        }
    }
}

DataBase::~DataBase(){
    writeDs();
   /* if(AutoSave)
        writeAllBD();*/
    for(BPair i:descript){
        delete i.second;
        delete i.first;
    }
    descript.clear();
}
