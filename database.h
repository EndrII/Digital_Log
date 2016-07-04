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
#ifndef DATABASE_H
#define DATABASE_H
#include <groupvoid.h>
#include <QInputDialog>
#include <QDir>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QMessageBox>
#define HEADER_DB "Header.bdh"
#define PATCH_NAME_S(GR) patch+"SGR"+GR
#define PATCH_NAME_P(GR) patch+"PGR"+GR
enum StateDataBase{notOpened,Arhive,notStarted,Started};
enum BaseMode{noData,MySqlMode,C_PlusMode};
typedef QPair<Group*,GroupVoid*> BPair;
class DataBase:public QObject
{
    Q_OBJECT
private:
    BaseMode mode;
    //********************************************
    //SQL Source
    QSqlDatabase *sqlbd;
    QSqlQuery *query;
    QList<QSqlTableModel> *sqlSource;
    //********************************************
    //C++ Source
    void PatchCorect();
    QString patch,pass_,ARhivePatch;
    ETime *time;
    bool End;
    bool autoSaveGroup;
    bool autoSem;
    QDate* datesave;//   QDate dateB1,dateE1,dateB2,dateE2;
    StateDataBase statusBD;
    //QVector<GroupVoid*> data;
    QVector<BPair> descript; //first - group name second group patch
    bool writeBD(const QString& groupID);
    bool writeBD(Group*);
    void writeAllBD();
    GroupVoid *readBD(const QString& groupID);
    GroupVoid *readBD(Group*);
    void readAllBD();
    GroupVoid*find_pointer(const QString &name);
    GroupVoid*find_pointer(Group*);
    int find_index(const QString &name);
    int find_index(Group*);
    bool writeDs(QString str="patch");
    bool readDS(QString str="patch");
private slots:
    void timerSchanged(bool);
    void groupChangedDeligate(Group*);
    void closeAll();
public:
    explicit DataBase(const QString& patch_="./Database/",QObject *ptr=NULL);
    bool find(const QString& name);
    bool find(Group*);
    void createDataBase(const QString& name,const BaseMode& mode=C_PlusMode,
                        const QString & host_name="local",
                        const QString& userName="Admin",
                        const QString& userPassword="Admin");
    bool createGroup(const QStringList& list, const QString &group);
    bool deleteGroup(const QString&group);
    bool AutoSave;
    StateDataBase getState()const;
    QString getArhivePatch()const;
    void setArhivePatch(const QString &str);
    bool isEnd()const;
    QString getPass()const;
    void setPass(const QString&pass);
    void startTime();
    bool endTime();
    bool security()const;
    bool getAutosaveGroup()const;
    void setAutosaveGroup(bool);
    bool getAutoSem()const;
    void setAutoSem(bool);
    QDate* Dates()const;//massiv 4 elementa 1b 1e 2b 2e
    int getLastPropusk()const;
    bool isCreated()const;
    void setPatch(const QString&);
    QString getPatch()const;
    bool createControlGroup(Group*);
    bool deleteControlGroup(Group*);
    QVector<BPair> *getDataList();
    void open(const QString& patch_="");
    void save(const QString&id);
    void saveAll();
    void openGroup(const QString&GroupName);
    ~DataBase();
public slots:
signals:
    void Error(int,QString);
    /*
     *
     *-2 - Важный текст
     *-1 - простой текст
     * 0 - простая ошибка с текстом
     * 1 - не возможно записать файл
     * 2 - группа не найденна
     * 3 - ошибка открытия группы
     * 4 - база данных уже создана
    */
    void StateChanged(StateDataBase);
    void DataBaseCreated(QString);
    void DataBaseOpened(QString);
    void GroupSaved(GroupVoid*);
    void GroupChanged(Group*);
    void GroupOpened(GroupVoid*);
    void GroupVoidChanged(GroupVoid*);
    void GroupCreated(Group*);
    void controlGroupCreated(GroupVoid*);
    void GroupDeleted(QString);
    void controlGroupDeleted(Group*);

};

#endif // DATABASE_H
