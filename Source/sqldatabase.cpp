#include "sqldatabase.h"
sqlDataBase::sqlDataBase():
    QSqlDatabase("QMYSQL")
{
    qer=new QSqlQuery(*this);
    model=new QSqlQueryModel();
    model->setQuery(*qer);
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
    if(state!=isOpen()){
        emit stateChanged(state=isOpen());
    }
}
QSqlQuery* sqlDataBase::registration(){
    return new QSqlQuery(*this);
}
QDate sqlDataBase::getReceiptDate(const QString &group){
    if(qer->exec(QString("select receipt_date from groups where name='%0' ").arg(group))&&qer->next()){
        return qer->value(0).toDate();
    }
    return QDate(0,0,0);
}
QDate sqlDataBase::getExpirationDate(const QString &group){
    if(qer->exec(QString("select expiration_date from groups where name='%0' ").arg(group))&&qer->next()&&!qer->isNull(0)){
        return qer->value(0).toDate();
    }
    return QDate(0,0,0);
}
bool sqlDataBase::GetState(){
    return state;
}
void sqlDataBase::error_msg(const QString& Messag){
    if(!this->isOpen()){
        connect_to();
    }
    if(Messag!="none"){
        emit Message(0,Messag);
    }
}
/*
 * CREATE USER 'Admin'@'%' IDENTIFIED BY 'ipf';
 * GRANT ALL PRIVILEGES ON [имя базы данных].* TO 'Admin'@'%';

CREATE USER 'Headman'@'%';
GRANT SELECT ON [имя базы данных].* TO 'Headman'@'%';
GRANT INSERT,UPDATE ON [имя базы данных].attendance TO 'Headman'@'%';


CREATE USER 'Teacher'@'%';
GRANT SELECT ON [имя базы данных].* TO 'Teacher'@'%';
GRANT INSERT,UPDATE ON [имя базы данных].attendance TO 'Teacher'@'%';
GRANT INSERT,UPDATE ON [имя базы данных].performance TO 'Teacher'@'%';
GRANT INSERT,UPDATE ON [имя базы данных].limits TO 'Teacher'@'%';

CREATE USER 'Student'@'%';
GRANT SELECT ON [имя базы данных].* TO 'Student'@'%';
*/
bool sqlDataBase::createDefaultUsers(const QString &databaseName){
    QStringList sqlExec;
    sqlExec<<QString("CREATE USER 'Admin_%0'@'%' IDENTIFIED BY 'ipf';").arg(databaseName);
    sqlExec<<QString("GRANT ALL PRIVILEGES ON %0.* TO 'Admin_%0'@'%';").arg(databaseName);
    sqlExec<<QString("CREATE USER 'Headman_%0'@'%';").arg(databaseName);
    sqlExec<<QString("GRANT SELECT ON %0.* TO 'Headman_%0'@'%';").arg(databaseName);
    sqlExec<<QString("GRANT INSERT,UPDATE ON %0.attendance TO 'Headman_%0'@'%';").arg(databaseName);
    sqlExec<<QString("CREATE USER 'Teacher_%0'@'%';").arg(databaseName);
    sqlExec<<QString("GRANT SELECT ON %0.* TO 'Teacher_%0'@'%';").arg(databaseName);
    sqlExec<<QString("GRANT INSERT,UPDATE ON %0.attendance TO 'Teacher_%0'@'%';").arg(databaseName);
    sqlExec<<QString("GRANT INSERT,UPDATE ON %0.performance TO 'Teacher_%0'@'%';").arg(databaseName);
    sqlExec<<QString("GRANT INSERT,UPDATE ON %0.limits TO 'Teacher_%0'@'%';").arg(databaseName);
    sqlExec<<QString("CREATE USER 'Student_%0'@'%';").arg(databaseName);
    sqlExec<<QString("GRANT SELECT ON %0.* TO 'Student_%0'@'%';").arg(databaseName);
    bool result=true;
    for(QString str:sqlExec){
        result=result&&qer->exec(str);
        if(!result){
            qDebug()<<str<<"\n with error: "<<qer->lastError().databaseText();
        }
    }
    return result;
}
bool sqlDataBase::removeDefaultUsers(const QString &databaseName){
    QSqlQuery q(*this);
    bool result=true;
    if(qer->exec("SELECT User,Host FROM mysql.user")){
        while(qer->next()){
            QString user(qer->value("User").toString());
            if(user.indexOf("_"+databaseName)>-1){
                result=result&&q.exec(QString("DROP User '%0'@'%'").arg(user));
            }
        }
    }
    return result;
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
    if(!qer->exec("select name from groups")){
        error_msg(ELanguage::getWord(LOCALE_ERROR)+" QStringList& sqlDataBase::getGroupList()");
    }
    tempList.clear();
    while(qer->next()){
        tempList.push_back(qer->value(0).toString());
    }
    return tempList;
}
QStringList& sqlDataBase::getGroupPredmetsList(const QString& group){
    if(!qer->exec("select s.name from subjects_groups g, subjects s where s.id=g.subject and g._group=(select id from groups where name='"+group+"')")){
        error_msg();
    }
    tempList.clear();
    while(qer->next()){
        tempList.push_back(qer->value(0).toString());
    }
    return tempList;
}
QStringList& sqlDataBase::getDateListU(const QDate& beginRange,const QDate &endRange){
    if(!qer->exec("select point from dates where date_group=2 and "
                  " point>='"+ beginRange.toString("yyyy-MM-dd")+"' and "
                  "point<='"+endRange.toString("yyyy-MM-dd")+"'")){
        error_msg(ELanguage::getWord(LOCALE_ERROR)+"QStringList& sqlDataBase::getDateListU(const QDate& beginRange,const QDate &endRange)");
    }
    tempList.clear();
    while(qer->next()){
        tempList.push_back(qer->value(0).toString());
    }
    return tempList;
}
int sqlDataBase::getDateUCount(bool all){
    if(!qer->exec("select COUNT(point) from dates"+((!all)?QString(" where date_group=2"):QString()))){
        error_msg(ELanguage::getWord(LOCALE_ERROR)+"int sqlDataBase::getDateUCount(bool all)");
    }else{
        qer->next();
        return qer->value(0).toInt();
    }
    return -1;
}
int sqlDataBase::getDatePCount(bool all){
    if(!qer->exec("select COUNT(point) from dates"+((!all)?QString(" where date_group=1"):QString()))){
        error_msg(ELanguage::getWord(LOCALE_ERROR)+"int sqlDataBase::getDatePCount(bool all)");
    }else{
        qer->next();
        return qer->value(0).toInt();
    }
    return -1;
}
QStringList& sqlDataBase::getDateListP(const QDate &beginRange, const QDate &endRange){
    if(!qer->exec("select point from dates where date_group=1 and"
                  " point>='"+ beginRange.toString("yyyy-MM-dd")+"' and "
                  "point<='"+endRange.toString("yyyy-MM-dd")+"'")){
        error_msg(ELanguage::getWord(LOCALE_ERROR)+"QStringList& sqlDataBase::getDateListP(const QDate &beginRange, const QDate &endRange)");
    }
    tempList.clear();
    while(qer->next()){
        tempList.push_back(qer->value(0).toString());
    }
    return tempList;
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
        databaseName=BaseName;
        if(state!=isOpen()){
            emit stateChanged(state=isOpen());
        }
    }
}
bool sqlDataBase::createDB(const QString &BaseName){
    if(qer->exec("create database "+BaseName)&&
       qer->exec("use "+BaseName)){
       return SqlExec(qer,":/sql/sql/Create.sql")&&createDefaultUsers(BaseName);
    }
    return false;
}
void sqlDataBase::createGroup(QString GroupName){
    if(GroupName.isEmpty())
        return;
    if(qer->exec(QString("INSERT INTO groups(name,receipt_date,year,expiration_date) VALUE('%0',CURDATE(),0,DATE_ADD(CURDATE(), INTERVAL 4 YEAR))").arg(GroupName))){
        emit Message(1,ELanguage::getWord(GROUP_CREATED)+" "+GroupName);
    }else{
        error_msg(ELanguage::getWord(ERROR_CREATING_GROUP));
    }
}
void sqlDataBase::createPredmet(QString PredmetName){
    if(PredmetName.isEmpty())return;
    if(qer->exec(QString("INSERT INTO subjects(name) VALUE('%0')").arg(PredmetName))){
        emit Message(1,ELanguage::getWord(PREDMET_CREATED)+" "+PredmetName);
    }else{
        error_msg(ELanguage::getWord(ERROR_CREATING_PREDMET));
    }
}
int sqlDataBase::getGroupLimit(const QString &group){
    qer->exec("select _value from limitsGroup where _group=(select id from groups where name='"+group+"')");
    qer->next();
    return qer->value(0).toInt();
}
void sqlDataBase::setGroupLimit(const QString &group,const int &limit){
    if(!qer->exec(QString("INSERT INTO limitsGroup(_value,_group) VALUE(%0,(select id from groups where name='%1'))").arg(QString::number(limit),group))){
        qer->exec(QString("UPDATE  limitsGroup SET _value=%0 where _group=(select id from groups where name='%1')").arg(QString::number(limit),group));
    }
}
int sqlDataBase::getGroupLimit(const QString &group,const QString &prefix,const QString &predmet){
    QString q="select _value from limits where _group=(select id from groups where name='"+group+"')"
              " and subject=(select id from subjects where name='"+predmet+"') and "
              "work_group=(select id from work_groups where name='"+prefix+"')";
    qer->exec(q);
    qer->next();
    return qer->value(0).toInt();
}
void sqlDataBase::setGroupLimit(const QString &group,const QString &prefix,const QString &predmet, const int &limit){
    QString q=QString("INSERT INTO limits(_value,_group,subject,work_group) VALUE(%0,"
                          "(select id from groups where name='%1'),(select id from subjects where name='%2'),"
                          "(select id from work_groups where name='%3'))").arg(QString::number(limit),group,predmet,prefix);
    if(!qer->exec(q)){
        q=QString("UPDATE limits SET _value=%0 where _group=(select id from groups where name='%1') and "
                  "subject=(select id from subjects where name='%2') and work_group=(select id from work_groups where name='%3')")
          .arg(QString::number(limit),group,predmet,prefix);
        qDebug()<<q;
        qer->exec(q);
    }
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
    if(!name.isEmpty()&&qer->exec(
                QString("insert into students(name,_group) value('%0',(select id from groups where name='%1'))").arg(name).arg(group))){

            emit Message(1,ELanguage::getWord(STUDENT_ADD)+" "+name);
            //emit ChangedBD();
        }else{
            error_msg();
        }
}
void sqlDataBase::deleteStudent(const QString &group, const QString name){
    if(qer->exec(QString("DELETE FROM students where "
                         "_group=(select id from groups where name='%0') and name='%1'")
                 .arg(group).arg(name))){
        emit Message(1,ELanguage::getWord(STUDENT_DELETED)+" "+name);
        //emit ChangedBD();
    }else{
        error_msg(ELanguage::getWord(ERROR_DELETE));
    }
}
void sqlDataBase::deleteGrpoup(const QString &name){
    if(qer->exec(QString("DELETE FROM groups where name='%0'").arg(name))){
        emit Message(1,ELanguage::getWord(GROUP_DELETED)+" "+name);
    }else{
        error_msg(ELanguage::getWord(ERROR_DELETE));
    }
}

void sqlDataBase::deletePredmet(const QString &name){
    if(qer->exec(QString("DELETE FROM subjects where name='%0'").arg(name))){
        emit Message(1,ELanguage::getWord(PREDMET_DELETED)+" "+name);
    }else{
        error_msg(ELanguage::getWord(ERROR_DELETE));
    }
}
void sqlDataBase::addPredmetGroup(const QString &gr, const QString &pred){
    if(qer->exec(QString("INSERT INTO subjects_groups(_group,subject) VALUE("
                         "(select id from groups where name='%0'),"
                         "(select id from subjects where name ='%1'))").arg(gr).arg(pred))){
        emit Message(1,ELanguage::getWord(PREDMET_ADDED)+" "+pred+"->"+gr);
    }else{
        error_msg(ELanguage::getWord(ERROR_ADD_PREDMET));
    }
}
void sqlDataBase::removePredmetGroup(const QString &gr, const QString &pred){
    if(qer->exec(QString("DELETE FROM subjects_groups where _group=(select id from groups where name='%0')"
                         " and subject=(select id from subjects where name='%1')").arg(gr).arg(pred))){
        emit Message(1,ELanguage::getWord(PREDMET_DELETED_GR)+" "+pred+"->"+gr);
    }else{
        error_msg(ELanguage::getWord(ERROR_DELETE));
    }
}
QSqlQuery * sqlDataBase::getQuery(){
    return qer;
}
bool sqlDataBase::writeTableModel(Write_Line_Params description, QStandardItemModel * model){
    for(int r=0;r<model->rowCount();r++){
        for(int c=0;model->columnCount();c++){
             description.valueColumn=model->headerData(c,Qt::Horizontal).toString();
             description.valueRow=model->headerData(r,Qt::Vertical).toString();
             description.value=model->data(model->index(c,r)).toString();
            if(!writeLine(description))
                return false;
        }
    }
    return true;
}
bool sqlDataBase:: writeLine(const Write_Line_Params &description){
    QString q=QString("INSERT INTO %0(%1,%2,%3) VALUE((select id from %4"+((description.DescRowTable=="dates")?QString(" and date_group=%7 )"):QString(")"))+","
                      "(select id from %5"+((description.DescColumnTable=="dates")?QString(" and date_group=%7 )"):QString(")"))+",%6)").arg(
                description.InToTable,description.columnInTo,description.rowInTo,description.valueInTo,
                description.DescRowTable+" where "+description.DescRowTableColumn+ "='"+description.valueRow+"'",
                description.DescColumnTable+" where "+description.DescColumnTableColumn+ "='"+description.valueColumn+"'",
                description.value,description.dateType);
                qDebug()<<q;

    if(qer->exec(q)){
        return true;
    }else{
        QString q=QString("UPDATE %0 SET %3=%6 where %1=(select id from %4"+((description.DescRowTable=="dates")?QString(" and date_group=%7 )"):QString(")"))+" and "
                          " %2=(select id from %5"+((description.DescColumnTable=="dates")?QString(" and date_group=%7 )"):QString(")"))+"").arg(
                    description.InToTable,description.columnInTo,description.rowInTo,description.valueInTo,
                    description.DescRowTable+" where "+description.DescRowTableColumn+ "='"+description.valueRow+"'",
                    description.DescColumnTable+" where "+description.DescColumnTableColumn+ "='"+description.valueColumn+"'",
                    description.value,description.dateType);
        qDebug()<<q;
        return qer->exec(q);
    }
    return false;
}
bool sqlDataBase:: writeLine(const Write_Line_ParamsU& description){
    QString q=QString("INSERT INTO performance(student,_date,work_group,subject,_value) VALUE((select id from students where name='%0'), "
                      "(select id from dates where point='%1' and date_group=2),(select id from work_groups where name='%2' ),"
                      "(select id from subjects where name='%3'),%4)").arg(
                description.student,description.date,description.work_group,description.subject,description.value);
                qDebug()<<q;

    if(qer->exec(q)){
        return true;
    }else{
        QString q=QString("UPDATE performance SET _value=%4 where student=(select id from students where name='%0') and "
                          "_date=(select id from dates where point='%1' and date_group=2) and work_group=(select id from work_groups where name='%2' )"
                          "and subject=(select id from subjects where name='%3')").arg(
                    description.student,description.date,description.work_group,description.subject,description.value);
                    qDebug()<<q;
        return qer->exec(q);
    }
    return false;
}
bool sqlDataBase::setUserPass(const User & user){
    QString qery=QString("SET PASSWORD FOR '%0'@'%' = PASSWORD('%1')").arg(user.name,user.pass);
    return qer->exec(qery);
}
bool sqlDataBase::removeDatabase(const QString &databaseName){
    if(removeDefaultUsers(databaseName)){
        return qer->exec("drop database "+databaseName);
    }
    return false;
}
bool sqlDataBase::getUserInformation(Users &users){
    if(qer->exec("SELECT User,Host,authentication_string FROM mysql.user")){
        while(qer->next()){
            QString user(qer->value("User").toString());
            if(user.indexOf("_"+databaseName)>-1){
                if(user.indexOf("Student")>-1){
                    users[(int)ut::Student].name=user;
                    users[(int)ut::Student].pass=qer->value("authentication_string").toString();
                }
                if(user.indexOf("Teacher")>-1){
                    users[(int)ut::Teacher].name=user;
                    users[(int)ut::Teacher].pass=qer->value("authentication_string").toString();
                }
                if(user.indexOf("Headman")>-1){
                    users[(int)ut::Headman].name=user;
                    users[(int)ut::Headman].pass=qer->value("authentication_string").toString();
                }
                if(user.indexOf("Admin")>-1){
                    users[(int)ut::Admin].name=user;
                    users[(int)ut::Admin].pass=qer->value("authentication_string").toString();
                }
            }
        }
        return true;
    }
    return false;
}
bool sqlDataBase::transformQuery(const QString& query, const QString &columnData, const QString &rowData, const QString &valueData, QStandardItemModel * model){
    if(qer->exec(query)){
        model->clear();
        static_cast<MySqlQueryColorModel*>(model)->setLoad(false);
        while(qer->next()){
            int searchRow=0,searchColumn=0;
            while(searchColumn<model->columnCount()&&model->headerData(searchColumn,Qt::Horizontal)!=qer->value(columnData))++searchColumn;
            if(searchColumn==model->columnCount()){
                model->appendColumn(QList<QStandardItem*>());
                model->setHeaderData(model->columnCount()-1,Qt::Horizontal,qer->value(columnData));
            }
            while(searchRow<model->rowCount()&&model->headerData(searchRow,Qt::Vertical)!=qer->value(rowData))++searchRow;
            if(searchRow==model->rowCount()){
                model->appendRow(QList<QStandardItem*>());
                model->setHeaderData(model->rowCount()-1,Qt::Vertical,qer->value(rowData));
            }
            model->setData(model->index(searchRow,searchColumn),model->data(model->index(searchRow,searchColumn)).toInt()+qer->value(valueData).toInt(),Qt::EditRole);
        }
        static_cast<MySqlQueryColorModel*>(model)->setLoad(true);
        if(static_cast<MySqlQueryColorModel*>(model)->isSummColumn()){
            model->appendColumn(QList<QStandardItem*>());
            model->setHeaderData(model->columnCount()-1,Qt::Horizontal,ELanguage::getWord(SUMM));
        }
        return true;
    }
    return false;
}
sqlDataBase::~sqlDataBase(){
    delete qer;
    delete model;
}
