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
        if(state!=isOpen()){
            emit stateChanged(state=isOpen());
        }
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
    qer->exec("select lim from limits where _group='"+group+"' and isattendance=1");
    qer->next();
    return qer->value(0).toInt();
}
void sqlDataBase::setGroupLimit(const QString &group,const int &limit){
    if(!qer->exec(QString("INSERT INTO limitsGroup(_value,_group) VALUE(%0,(select id from groups where name='%1'))").arg(QString::number(limit),group))){
        qer->exec(QString("UPDATE  limitsGroup SET _value=%0 where _group='%1'").arg(QString::number(limit),group));
    }
}
int sqlDataBase::getGroupLimit(const QString &group,const QString &prefix,const QString &predmet){
    qer->exec("select _value from limits where _group='"+group+"' and subject='"+predmet+"' and work_group='"+prefix+"'");
    qer->next();
    return qer->value(0).toInt();
}
void sqlDataBase::setGroupLimit(const QString &group,const QString &prefix,const QString &predmet, const int &limit){
    if(!qer->exec(QString("INSERT INTO limits(_value,_group,subject,work_group) VALUE(%0,"
                      "select id from groups where name='%1',select if from subjects where name='%2',"
                      "select id from work_groups where name='%3')").arg(QString::number(limit),group,predmet,prefix))){
        qer->exec(QString("UPDATE limits SET _value=%0 where _group=(select id from groups where name='%1' and "
                          "'select if from subjects where name='%2' and 'select id from work_groups where name='%3'')").arg(QString::number(limit),group,predmet,prefix));
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
bool sqlDataBase:: writeLine(const Write_Line_Params& description){
    QString q=QString("INSERT INTO %0(%1,%2,%3) VALUE((select id from %4"+((description.DescRowTable=="dates")?QString(" and date_group=%7 )"):QString(")"))+","
                      "(select id from %5"+((description.DescColumnTable=="dates")?QString(" and date_group=%7 )"):QString(")"))+",%6)").arg(
                description.InToTable,description.columnInTo,description.rowInTo,description.valueInTo,
                description.DescRowTable+" where "+description.DescRowTableColumn+ "='"+description.valueRow+"'",
                description.DescColumnTable+" where "+description.DescColumnTableColumn+ "='"+description.valueColumn+"'",
                description.value,description.dateType);
                //qDebug()<<q;

    if(qer->exec(q)){
        return true;
    }else{
        QString q=QString("UPDATE %0 SET %3=%6 where %1=(select id from %4"+((description.DescRowTable=="dates")?QString(" and date_group=%7 )"):QString(")"))+" and "
                          " %2=(select id from %5"+((description.DescColumnTable=="dates")?QString(" and date_group=%7 )"):QString(")"))+"").arg(
                    description.InToTable,description.columnInTo,description.rowInTo,description.valueInTo,
                    description.DescRowTable+" where "+description.DescRowTableColumn+ "='"+description.valueRow+"'",
                    description.DescColumnTable+" where "+description.DescColumnTableColumn+ "='"+description.valueColumn+"'",
                    description.value,description.dateType);
        //qDebug()<<q;
        return qer->exec(q);
    }
    return false;
}

bool sqlDataBase::transformQuery(const QString& query,const QString &columnData, const QString &rowData, const QString &valueData, QStandardItemModel * model){
    if(qer->exec(query)){
        model->clear();
        qer->seek(0);
        while(qer->next()){
            int searchRow=0,searchColumn=0;
            while(searchColumn<model->columnCount()&&model->headerData(searchColumn,Qt::Horizontal)!=qer->value(columnData))++searchColumn;
            if(searchColumn==model->columnCount()){
                model->appendColumn(QList<QStandardItem*>());
                qDebug()<<"set name empty colum("<<qer->value(columnData)<<")="<<model->setHeaderData(model->columnCount()-1,Qt::Horizontal,qer->value(columnData));
            }
            while(searchRow<model->rowCount()&&model->headerData(searchRow,Qt::Vertical)!=qer->value(rowData))++searchRow;
            if(searchRow==model->rowCount()){
                model->appendRow(QList<QStandardItem*>());
                qDebug()<<"set name empty row("<<qer->value(rowData)<<")="<<model->setHeaderData(model->rowCount()-1,Qt::Vertical,qer->value(rowData));
            }
            qDebug()<<"r="<<searchRow<<" c="<<searchColumn<<" value="<<qer->value(valueData)<<" result="<<model->setData(model->index(searchRow,searchColumn),qer->value(valueData),Qt::EditRole);
        }
        return true;
    }
    return false;
}
sqlDataBase::~sqlDataBase(){
    delete qer;
    delete model;
}
