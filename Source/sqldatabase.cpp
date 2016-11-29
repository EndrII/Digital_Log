#include "sqldatabase.h"
sqlDataBase::sqlDataBase():
    QSqlDatabase("QMYSQL")
{
    qer=new QSqlQuery(*this);
    model=new QSqlQueryModel();
    model->setQuery(*qer);
    //connect_to(user,pass,host,port);
}
void sqlDataBase::connect_to(const QString &user, const QString &pass, const QString &host, const QString &port){
    if(!user.isEmpty()){
        this->setHostName(host);
        this->setUserName(user);
        this->setPassword(pass);
        this->setPort(port.toInt());
    }
    if(!open()){
        emit Message(0,ELanguage::getWord(MYSQL_ERROR_MSG)+":\n"+
                     ELanguage::getWord(DRIVER_MSG)+": "+this->lastError().driverText()+". \n"+
                     ELanguage::getWord(BATABASE_MSG)+": "+this->lastError().databaseText());

    }
    stateChang();
}
QSqlQuery* sqlDataBase::registration(){
    return new QSqlQuery(*this);
}
state_BD sqlDataBase::GetState(){
    return state;
}
void sqlDataBase::stateChang(){
    int op= isOpen();
    qer->exec("select flag from config_flags where id=1");
    qer->next();
    state_BD temp=(state_BD)(op+qer->value(0).toInt());
    if(temp!=state)
        emit stateChanged(state=temp);
}
void sqlDataBase::error_msg(){
    if(!this->isOpen()){
        connect_to();
    }
    emit Message(0,ELanguage::getWord(MYSQL_ERROR_MSG)+":\n"+
                 ELanguage::getWord(DRIVER_MSG)+": "+this->lastError().driverText()+". \n"+
                 ELanguage::getWord(BATABASE_MSG)+": "+this->lastError().databaseText());
}
bool sqlDataBase::SqlExec(QSqlQuery *sq, const QString &sqlFile){
    QFile f(sqlFile);
    bool result=true;
    if(f.open(QIODevice::ReadOnly)){
        QString temp,delimiter=";";
        QTextStream stream(&f);
        stream.setCodec("UTF8");
        while(!stream.atEnd()){
            temp+=stream.readLine();
            if(temp.lastIndexOf("delimiter",-1,Qt::CaseInsensitive)>-1){
                temp.remove("delimiter",Qt::CaseInsensitive);
                int last=temp.indexOf(QRegularExpression("[^ \f\n\r\t\v]"))+1;
                int begin=temp.lastIndexOf(QRegularExpression("[^ \f\n\r\t\v]"));
                delimiter=temp.mid(begin,last-begin);
                temp="";
            }else{
                if(temp.lastIndexOf(delimiter)>-1){
                    temp.remove(delimiter);
                    (result=result&&sq->exec(temp));
                    temp="";
                }
            }
        }
        return result;
    }
    return false;
}
QSqlQueryModel *sqlDataBase::getModel() const{
    return model;
}
QStringList& sqlDataBase::getGroupList(){
    if(!qer->exec("select Имя from группы")){
        error_msg();
    }
    tempList.clear();
    while(qer->next()){
        tempList.push_back(qer->value(0).toString());
    }
    return tempList;
}
QStringList& sqlDataBase::getGroupPredmetsList(const QString& group){
    if(!qer->exec("select предмет from P_"+group)){
        error_msg();
    }
    tempList.clear();
    while(qer->next()){
        tempList.push_back(qer->value(0).toString());
    }
    return tempList;
}
QStringList& sqlDataBase::getDateListU(){
    if(!qer->exec("select Даты from времяУроки")){
        error_msg();
    }
    tempList.clear();
    while(qer->next()){
        tempList.push_back(qer->value(0).toString());
    }
    return tempList;
}
QStringList& sqlDataBase::getDateListP(){
    if(!qer->exec("select Даты from времяПропуски")){
        error_msg();
    }
    tempList.clear();
    while(qer->next()){
        tempList.push_back(qer->value(0).toString());
    }
    return tempList;
}
void sqlDataBase::StartDB(const char dayU, const char dayP){
    if(dayU>=1&&dayU>28)
        qer->exec("UPDATE config_flags SET flag="+QString::number(dayU)+" where id=2");
    if(dayP>=1&&dayP>6)
        qer->exec("UPDATE config_flags SET flag="+QString::number(dayP)+" where id=3");
    qer->exec("call startBd()");
    stateChang();
}
void sqlDataBase::StopBD(const bool toArhive){
    qer->exec("call stopBd()");
    stateChang();
}
void sqlDataBase::Query(const QString &str){
    if(!qer->exec(str)){
        error_msg();
    }
    model->setQuery(*qer);
}
bool sqlDataBase::Query_no_update(const QString &str){
    return qer->exec(str);
}
void sqlDataBase::openDB(const QString &BaseName){
    if(!qer->exec("use "+BaseName)){
        error_msg();
    }else{
        emit Message(1,ELanguage::getWord(SELECTED_DB)+" "+BaseName);
        emit ChangedBD();
        stateChang();

    }

}
bool sqlDataBase::createDB(const QString &BaseName){
    if(qer->exec("create database "+BaseName)&&
       qer->exec("use "+BaseName)){
       return SqlExec(qer,":/sql/sql/Create.sql");
    }
    return false;
}
void sqlDataBase::createGroup(const QString &GroupName){
    if(qer->exec("call createGroup('"+GroupName+"');")){
        emit Message(1,ELanguage::getWord(GROUP_CREATED)+" "+GroupName);
        emit ChangedBD();
    }else{
        error_msg();
    }
}
void sqlDataBase::createPredmet(const QString &PredmetName){
    if(qer->exec("call createPredmet('"+PredmetName+"');")){
        emit Message(1,ELanguage::getWord(PREDMET_CREATED)+" "+PredmetName);
        emit ChangedBD();
    }else{
        error_msg();
    }
}
void sqlDataBase::addStudent(const QString &group, const QString name){
    if(qer->exec("call addStudent('"+name+"','"+group+"');")){
        qer->exec("select count(word) from prefix");
        qer->next();
        int longPrefix=qer->value(0).toInt();
        qDebug()<<"all tables="<<qer->exec("select * from p_"+group);
        QSqlQuery tempQ(*this);
        while(qer->next())
            for(int i=2;i<longPrefix+2;i++){
                QString tem="INSERT INTO "+qer->value(i).toString()+"(ФИО) "
                                                                     " (select ФИО from "+group+")";

                qDebug()<<"temp q="<<tem<<"="<<tempQ.exec(tem);
            }
        emit Message(1,ELanguage::getWord(STUDENT_ADD)+" "+name);
        emit ChangedBD();
    }else{
        error_msg();
    }
}
void sqlDataBase::deleteStudent(const QString &group, const QString name){
    if(qer->exec("call deleteStudent('"+name+"','"+group+"');")){
        emit Message(1,ELanguage::getWord(STUDENT_DELETED)+" "+name);
        emit ChangedBD();
    }else{
        error_msg();
    }
}
void sqlDataBase::deleteGrpoup(const QString &name){
    if(qer->exec("call deleteGroup('"+name+"');")){
        emit Message(1,ELanguage::getWord(GROUP_DELETED)+" "+name);
        emit ChangedBD();
    }else{
        error_msg();
    }
}

void sqlDataBase::deletePredmet(const QString &name){
    if(qer->exec("call deleteGlobalPredmet('"+name+"');")){
        emit Message(1,ELanguage::getWord(PREDMET_DELETED)+" "+name);
        emit ChangedBD();
    }else{
        error_msg();
    }
}
void sqlDataBase::addPredmetGroup(const QString &gr, const QString &pred){
    if(qer->exec("call addPredmet('"+pred+"','"+gr+"',1);")){
        /*qer->exec("select count(word) from prefix");
        qer->next();
        int longPrefix=qer->value(0).toInt();
        qer->exec("select * from p_"+gr+" where предмет="+pred);
        QSqlQuery tempQ(*this);
        while(qer->next())
            for(int i=0;i<longPrefix;i++){
                qDebug()<<"temp SELECT="<<tempQ.exec("INSERT INTO "+qer->value(i).toString()+"(ФИО) "
                           " (select ФИО from "+gr+")");
            }*/
        emit Message(1,ELanguage::getWord(PREDMET_ADDED)+" "+pred+"->"+gr);
        emit ChangedBD();
    }else{
        error_msg();
    }
}
void sqlDataBase::removePredmetGroup(const QString &gr, const QString &pred){
    if(qer->exec("call deletePredmet('"+pred+"','"+gr+"',1);")){
        emit Message(1,ELanguage::getWord(PREDMET_DELETED_GR)+" "+pred+"->"+gr);
        emit ChangedBD();
    }else{
        error_msg();
    }
}
sqlDataBase::~sqlDataBase(){
    delete qer;
    delete model;
}
