#ifndef SQLDATABASE_H
#define SQLDATABASE_H
#include "core.h"
#include <QtSql>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#define PORT 2277
class sqlDataBase:public QObject, protected QSqlDatabase{
    Q_OBJECT
private:
    QStringList tempList;
    QSqlQuery *qer;
    QSqlQueryModel *model;
private slots:
protected:
    bool SqlExec(QSqlQuery*,const QString &sqlFile);
public:
    explicit sqlDataBase(const QString&user, const QString&pass, const QString &host,const QString &dataBaseName="mysql");
    QSqlQuery* registration();
    void openDB(const QString &BaseName);
    void createGroup(const QString &GroupName);
    void createPredmet(const QString &PredemtName);
    bool createDB(const QString &BaseName);
    void addPredmetGroup(const QString&gr,const QString&pred);
    void removePredmetGroup(const QString&gr,const QString&pred);
    void deleteGrpoup(const QString& name);
    void deletePredmet(const QString& name);
   // void registration(QSqlQuery*quer,QSqlQueryModel *model);
    void deleteStudent(const QString &group, const QString name);
    QSqlQueryModel* getModel()const;
    QStringList& getGroupList();
    void Query(const QString&);
    bool Query_no_update(const QString&);
    //bool showDataBases();
    void addStudent(const QString& group,const QString name);
    bool createTruancy(const QString& GroupName);
    bool removeTruancy(const QString& GroupNmae);
    //QSqlQueryModel* get
    ~sqlDataBase();

signals:
    /*
     *
     * 2 - Важный текст
     * 1 - простой текст
     * 0 - простая ошибка с текстом
    */
    void Message(int,QString);
};
#endif // SQLDATABASE_H

