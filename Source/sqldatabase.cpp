#include "sqldatabase.h"
sqlDataBase::sqlDataBase(const QString &user, const QString &pass, const QString &host, const QString& dataBaseName):
    QSqlDatabase("QMYSQL")
{
    qer=new QSqlQuery(*this);
    model=new QSqlQueryModel();
    model->setQuery(*qer);
    this->setHostName(host);
    this->setUserName(user);
    this->setPassword(pass);
    if(this->open()){
        emit Message(1,ELanguage::getWord(M_CONNECTED));
    }
}
QSqlQuery* sqlDataBase::registration(){
    return new QSqlQuery(*this);
}
bool sqlDataBase::SqlExec(QSqlQuery *sq, const QString &sqlFile){
    QFile f(sqlFile);
    bool result=true;
    if(f.open(QIODevice::ReadOnly)){
        QString temp,delimiter=";";
        QTextStream stream(&f);
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
                    result=result&&sq->exec(temp);
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
        emit Message(0,ELanguage::getWord(MYSQL_ERROR_MSG)+"\n"+
                     ELanguage::getWord(DRIVER_MSG)+": "+this->lastError().driverText()+"\n"+
                     ELanguage::getWord(BATABASE_MSG)+": "+this->lastError().databaseText());
    }
    tempList.clear();
    while(qer->next()){
        tempList.push_back(qer->value(0).toString());
    }
    return tempList;
}
void sqlDataBase::Query(const QString &str){
    if(!qer->exec(str)){
        emit Message(0,ELanguage::getWord(MYSQL_ERROR_MSG)+"\n"+
                     ELanguage::getWord(DRIVER_MSG)+": "+this->lastError().driverText()+"\n"+
                     ELanguage::getWord(BATABASE_MSG)+": "+this->lastError().databaseText());
    }
    model->setQuery(*qer);
}
bool sqlDataBase::Query_no_update(const QString &str){
    return qer->exec(str);
}
void sqlDataBase::openDB(const QString &BaseName){
    if(!qer->exec("use "+BaseName)){
        emit Message(0,ELanguage::getWord(MYSQL_ERROR_MSG)+"\n"+
                     ELanguage::getWord(DRIVER_MSG)+": "+this->lastError().driverText()+"\n"+
                     ELanguage::getWord(BATABASE_MSG)+": "+this->lastError().databaseText());
    }else
        emit Message(1,ELanguage::getWord(SELECTED_DB)+" "+BaseName);
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
    }else{
        emit Message(0,ELanguage::getWord(MYSQL_ERROR_MSG)+"\n"+
                     ELanguage::getWord(DRIVER_MSG)+": "+this->lastError().driverText()+"\n"+
                     ELanguage::getWord(BATABASE_MSG)+": "+this->lastError().databaseText());
    }
}
void sqlDataBase::createPredmet(const QString &PredmetName){
    if(qer->exec("call createPredmet('"+PredmetName+"');")){
        emit Message(1,ELanguage::getWord(PREDMET_CREATED)+" "+PredmetName);
    }else{
        emit Message(0,ELanguage::getWord(MYSQL_ERROR_MSG)+"\n"+
                     ELanguage::getWord(DRIVER_MSG)+": "+this->lastError().driverText()+"\n"+
                     ELanguage::getWord(BATABASE_MSG)+": "+this->lastError().databaseText());
    }
}
void sqlDataBase::addStudent(const QString &group, const QString name){
    if(qer->exec("call addStudent('"+name+"','"+group+"');")){
        emit Message(1,ELanguage::getWord(STUDENT_ADD)+" "+name);
    }else{
        emit Message(0,ELanguage::getWord(MYSQL_ERROR_MSG)+"\n"+
                     ELanguage::getWord(DRIVER_MSG)+": "+this->lastError().driverText()+"\n"+
                     ELanguage::getWord(BATABASE_MSG)+": "+this->lastError().databaseText());
    }
}
void sqlDataBase::deleteStudent(const QString &group, const QString name){
    if(qer->exec("call deleteStudent('"+name+"','"+group+"');")){
        emit Message(1,ELanguage::getWord(STUDENT_DELETED)+" "+name);
    }else{
        emit Message(0,ELanguage::getWord(MYSQL_ERROR_MSG)+"\n"+
                     ELanguage::getWord(DRIVER_MSG)+": "+this->lastError().driverText()+"\n"+
                     ELanguage::getWord(BATABASE_MSG)+": "+this->lastError().databaseText());
    }
}
void sqlDataBase::deleteGrpoup(const QString &name){
    if(qer->exec("call deleteGroup('"+name+"');")){
        emit Message(1,ELanguage::getWord(GROUP_DELETED)+" "+name);
    }else{
        emit Message(0,ELanguage::getWord(MYSQL_ERROR_MSG)+"\n"+
                     ELanguage::getWord(DRIVER_MSG)+": "+this->lastError().driverText()+"\n"+
                     ELanguage::getWord(BATABASE_MSG)+": "+this->lastError().databaseText());
    }
}

void sqlDataBase::deletePredmet(const QString &name){
    if(qer->exec("call deleteGlobalPredmet('"+name+"');")){
        emit Message(1,ELanguage::getWord(PREDMET_DELETED)+" "+name);
    }else{
        emit Message(0,ELanguage::getWord(MYSQL_ERROR_MSG)+"\n"+
                     ELanguage::getWord(DRIVER_MSG)+": "+this->lastError().driverText()+"\n"+
                     ELanguage::getWord(BATABASE_MSG)+": "+this->lastError().databaseText());
    }
}
void sqlDataBase::addPredmetGroup(const QString &gr, const QString &pred){
    if(qer->exec("call addPredmet('"+pred+"','"+gr+"');")){
        emit Message(1,ELanguage::getWord(PREDMET_ADDED)+" "+pred+"->"+pred);
    }else{
        emit Message(0,ELanguage::getWord(MYSQL_ERROR_MSG)+"\n"+
                     ELanguage::getWord(DRIVER_MSG)+": "+this->lastError().driverText()+"\n"+
                     ELanguage::getWord(BATABASE_MSG)+": "+this->lastError().databaseText());
    }
}
void sqlDataBase::removePredmetGroup(const QString &gr, const QString &pred){
    if(qer->exec("call deletePredmet('"+pred+"','"+gr+"');")){
        emit Message(1,ELanguage::getWord(PREDMET_DELETED_GR)+" "+pred+"->"+pred);
    }else{
        emit Message(0,ELanguage::getWord(MYSQL_ERROR_MSG)+"\n"+
                     ELanguage::getWord(DRIVER_MSG)+": "+this->lastError().driverText()+"\n"+
                     ELanguage::getWord(BATABASE_MSG)+": "+this->lastError().databaseText());
    }
}
sqlDataBase::~sqlDataBase(){
    delete qer;
    delete model;
}
