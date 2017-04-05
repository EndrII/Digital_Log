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
void sqlDataBase::error_msg(const QString& Messag){
    if(!this->isOpen()){
        connect_to();
    }
    if(Messag!="none"){
        emit Message(0,Messag);
    }
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
        error_msg(ELanguage::getWord(LOCALE_ERROR)+" QStringList& sqlDataBase::getGroupList()");
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
QStringList& sqlDataBase::getDateListU(const QDate& beginRange,const QDate &endRange){
    if(!qer->exec("select Даты from времяУроки where Даты<NOW() and"
                  " Даты>='"+ beginRange.toString("yyyy-MM-dd")+"' and "
                  "Даты<='"+endRange.toString("yyyy-MM-dd")+"'")){
        error_msg(ELanguage::getWord(LOCALE_ERROR)+"QStringList& sqlDataBase::getDateListU(const QDate& beginRange,const QDate &endRange)");
    }
    tempList.clear();
    while(qer->next()){
        tempList.push_back(qer->value(0).toString());
    }
    return tempList;
}
int sqlDataBase::getDateUCount(bool all){
    if(!qer->exec("select COUNT(Даты) from времяУроки"+((!all)?QString(" where Даты<NOW()"):QString()))){
        error_msg(ELanguage::getWord(LOCALE_ERROR)+"int sqlDataBase::getDateUCount(bool all)");
    }else{
        qer->next();
        return qer->value(0).toInt();
    }
    return -1;
}
int sqlDataBase::getDatePCount(bool all){
    if(!qer->exec("select COUNT(Даты) from времяПропуски"+((!all)?QString(" where Даты<NOW()"):QString()))){
        error_msg(ELanguage::getWord(LOCALE_ERROR)+"int sqlDataBase::getDatePCount(bool all)");
    }else{
        qer->next();
        return qer->value(0).toInt();
    }
    return -1;
}
QStringList& sqlDataBase::getDateListP(const QDate &beginRange, const QDate &endRange){
    if(!qer->exec("select Даты from времяПропуски where Даты<NOW() and"
                  " Даты>='"+ beginRange.toString("yyyy-MM-dd")+"' and "
                  "Даты<='"+endRange.toString("yyyy-MM-dd")+"'")){
        error_msg(ELanguage::getWord(LOCALE_ERROR)+"QStringList& sqlDataBase::getDateListP(const QDate &beginRange, const QDate &endRange)");
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
        error_msg(ELanguage::getWord(DB_DONOTSELECTED));
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
void sqlDataBase::createGroup(QString GroupName){
    if(GroupName.isEmpty())
        return;
    GroupName.remove(QRegExp("[`/'.]"));
    removeFirstAndLastChars(' ',GroupName);
    GroupName.replace(' ','_');
    bool ok;
    GroupName.toInt(&ok);
    if(ok)GroupName.insert(0,'_');
    if(qer->exec("call createGroup('"+GroupName+"');")){
        emit Message(1,ELanguage::getWord(GROUP_CREATED)+" "+GroupName);
        emit ChangedBD();
    }else{
        error_msg(ELanguage::getWord(ERROR_CREATING_GROUP));
    }
}
void sqlDataBase::createPredmet(QString PredmetName){
    if(PredmetName.isEmpty())return;
    PredmetName.remove(QRegExp("[`/'.]"));
    removeFirstAndLastChars(' ', PredmetName);
    PredmetName.replace(' ','_');
    bool ok;
    PredmetName.toInt(&ok);
    if(ok)
        PredmetName.insert(0,'_');
    if(qer->exec("call createPredmet('"+PredmetName+"');")){
        emit Message(1,ELanguage::getWord(PREDMET_CREATED)+" "+PredmetName);
        emit ChangedBD();
    }else{
        error_msg(ELanguage::getWord(ERROR_CREATING_PREDMET));
    }
}
int sqlDataBase::getGroupLimit(const QString &group){
    qer->exec("select lim from группы where Имя='"+group+"'");
    qer->next();
    return qer->value(0).toInt();
}
void sqlDataBase::setGroupLimit(const QString &group,const int &limit){
    qer->exec("update группы set lim="+QString::number(limit)+"  where Имя='"+group+"'");
}
int sqlDataBase::getGroupLimit(const QString &group,const QString &prefix,const QString &predmet){
    qer->exec("select lim from limits where группа='"+group+"' and предмет='"+predmet+"' and prefix_='"+prefix+"'");
    qer->next();
    return qer->value(0).toInt();
}
void sqlDataBase::setGroupLimit(const QString &group,const QString &prefix,const QString &predmet, const int &limit){
    QString q="update limits set lim="+QString::number(limit)+"  where группа='"+group+"' and предмет='"+predmet+"' and prefix_='"+prefix+"'";
    qer->exec(q);
}
void sqlDataBase::removeFirstAndLastChars(const QChar item, QString &data){
    while (data[0]==item) {
        data.remove(0,1);
    }
    while (data[data.length()-1]==item) {
        data.remove(data.length()-1,1);
    }
}
void sqlDataBase::addStudent(const QString &group, const QString name){
    if(!name.isEmpty()&&qer->exec("call addStudent('"+name+"','"+group+"');")){
        qer->exec("select count(word) from prefix");
        qer->next();
        int longPrefix=qer->value(0).toInt();
        qer->exec("select * from P_"+group);
        QSqlQuery tempQ(*this);
        while(qer->next())
            for(int i=2;i<longPrefix+2;i++){
                QString tem="INSERT INTO "+qer->value(i).toString()+"(ФИО) "
                            " (select "+group+".ФИО from "+group+" LEFT JOIN"
                            " "+qer->value(i).toString()+" ON "+qer->value(i).toString()+
                            ".ФИО="+group+".ФИО where "+qer->value(i).toString()+".ФИО is NULL)";

                qDebug()<<i<<"|"<<tempQ.exec(tem)<<tem;
            }
        emit Message(1,ELanguage::getWord(STUDENT_ADD)+" "+name);
        emit ChangedBD();
    }else{
        error_msg();
    }
}
void sqlDataBase::sumCount(const QString &group){
    qer->exec("select *  from "+ group);
    QSqlQuery query(*this);
    while(qer->next()){
        short summ=0;
        //qDebug()<<"qer->record().count()="<<qer->record().count();
        for(short i=2;i<qer->record().count();i++){
            summ+=qer->value(i).toInt();
        }
        QString debug="update "+group+" set sum="+QString::number(summ)+
                " where ФИО='"+qer->value(1).toString()+"'";
        query.exec(debug);
    }
}
void sqlDataBase::sumCount(const QString &group, const QString &name){
    qer->exec("select *  from "+ group+" where ФИО='"+name+"'");
    QSqlQuery query(*this);
    while(qer->next()){
        short summ=0;
        for(short i=2;i<qer->record().count();i++){
            summ+=qer->value(i).toInt();
        }
        QString debug="update "+group+" set sum="+QString::number(summ)+
                " where ФИО='"+qer->value(1).toString()+"'";
        query.exec(debug);
    }
}
void sqlDataBase::sumCount(const QString &group, const QString &predmet,const int &index){
    QStringList prefix;
    prefix<<"PC_"<<"LC_"<<"KRC_"<<"RGRC_"<<"LD_"<<"KRD_"<<"RGRD_";
    sumCount(prefix[index]+predmet+group);
}
void sqlDataBase::sumCount(const QString &group, const QString &predmet,const int &index,const QString & name){
    QStringList prefix;
    prefix<<"PC_"<<"LC_"<<"KRC_"<<"RGRC_"<<"LD_"<<"KRD_"<<"RGRD_";
    sumCount(prefix[index]+predmet+group,name);
}
/*void sqlDataBase::sumCount(const QString &group, const QString &predmet){
    QStringList prefix;
    prefix<<"PC_"<<"LC_"<<"KRC_"<<"RGRC_"<<"LD_"<<"KRD_"<<"RGRD_";
    for(QString i:prefix){
        sumCount(i+predmet+group);
    }
}*/
void sqlDataBase::deleteStudent(const QString &group, const QString name){
    if(qer->exec("call deleteStudent('"+name+"','"+group+"');")){
        emit Message(1,ELanguage::getWord(STUDENT_DELETED)+" "+name);
        emit ChangedBD();
    }else{
        error_msg(ELanguage::getWord(ERROR_DELETE));
    }
}
void sqlDataBase::deleteGrpoup(const QString &name){
    if(qer->exec("call deleteGroup('"+name+"');")){
        emit Message(1,ELanguage::getWord(GROUP_DELETED)+" "+name);
        emit ChangedBD();
    }else{
        error_msg(ELanguage::getWord(ERROR_DELETE));
    }
}

void sqlDataBase::deletePredmet(const QString &name){
    if(qer->exec("call deleteGlobalPredmet('"+name+"');")){
        emit Message(1,ELanguage::getWord(PREDMET_DELETED)+" "+name);
        emit ChangedBD();
    }else{
        error_msg(ELanguage::getWord(ERROR_DELETE));
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
        error_msg(ELanguage::getWord(ERROR_ADD_PREDMET));
    }
}
void sqlDataBase::removePredmetGroup(const QString &gr, const QString &pred){
    if(qer->exec("call deletePredmet('"+pred+"','"+gr+"',1);")){
        emit Message(1,ELanguage::getWord(PREDMET_DELETED_GR)+" "+pred+"->"+gr);
        emit ChangedBD();
    }else{
        error_msg(ELanguage::getWord(ERROR_DELETE));
    }
}
QSqlQuery * sqlDataBase::getQuery(){
    return qer;
}
bool sqlDataBase::writeTableModel(const QString &tableName ,const QString &columnData, const QString &rowData, const QString &valueData, QStandardItemModel * model){
    for(int r=0;r<model->rowCount();r++){
        for(int c=0;model->columnCount();c++){
            if(!writeLine(tableName,columnData,rowData,valueData,
                      model->headerData(c,Qt::Horizontal).toString(),
                      model->headerData(r,Qt::Vertical).toString(),
                      model->data(model->index(c,r)).toString()))
                return false;
        }
    }
    return true;
} // this function not tested !!!!
bool sqlDataBase:: writeLine(const QString& tableName, const QString &column, const QString &row, const QString& value, const QString& columnData, const QString& rowData, const QString& valueData){
    bool okColumn,okRow,okVal;
    columnData.toInt(&okColumn);
    rowData.toInt(&okRow);
    valueData.toInt(&okVal);
    if(qer->exec(QString("INSERT INTO %0(%1,%2,%3) VALUE(%4,%5,%6)").arg(
                      tableName,column,row,value,(okColumn)?columnData:"'"+columnData+"'",
                      (okRow)?rowData:"'"+rowData+"'",(okVal)?valueData:"'"+valueData+"'"))){
        return true;
    }else{
        return qer->exec(QString("UPDATE %0 SET %3=%6 WHERE %1=%4 AND %2=%5").arg(
                             tableName,column,row,value,(okColumn)?columnData:"'"+columnData+"'",
                             (okRow)?rowData:"'"+rowData+"'",(okVal)?valueData:"'"+valueData+"'"));
    }
    return false;
}// this function not tested !!!!

bool sqlDataBase::transformQuery(const QString &columnData, const QString &rowData, const QString &valueData, QStandardItemModel * model){
    if(qer->record().indexOf(columnData)>=0&&qer->record().indexOf(rowData)>=0&&qer->record().indexOf(valueData)){
        model->clear();
        qer->seek(0);
        while(qer->next()){
            int searchRow=0,searchColumn=0;
            while(searchColumn<model->columnCount()&&model->headerData(searchColumn++,Qt::Horizontal)!=qer->value(columnData));
            if(searchColumn==model->columnCount()){
                model->appendColumn(QList<QStandardItem*>());
                model->setHeaderData(model->columnCount(),Qt::Horizontal,qer->value(columnData));
            }
            while(searchRow<model->rowCount()&&model->headerData(searchRow++,Qt::Vertical)!=qer->value(rowData));
            if(searchRow==model->rowCount()){
                model->appendRow(QList<QStandardItem*>());
                model->setHeaderData(model->rowCount(),Qt::Vertical,qer->value(rowData));
            }
            model->setData(model->index(searchRow,searchColumn),qer->value(valueData),Qt::EditRole);
        }
        return true;
    }
    return false;
}
sqlDataBase::~sqlDataBase(){
    delete qer;
    delete model;
}
