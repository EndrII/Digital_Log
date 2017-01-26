#ifndef MYSQLQUERYCOLORMODEL_H
#define MYSQLQUERYCOLORMODEL_H
#include <QMap>
#include <list>
#include "sqldatabase.h"
#include <QDebug>
#include <QBrush>
enum pref{LC_,PC_,KRC_,RGRC_};
struct StackItem{
    StackItem(){
        index=-1;
    }
    short index;// 0 - Enter data //  1 -  rename object;
    QString qyer;
    QString predmet;
    QString Group;
};
class MySqlQueryColorModel:public QSqlQueryModel
{
    Q_OBJECT
private:
    int limit;
   // sqlDataBase *db;
   // QList<StackItem> stack;
    void stackWrite();
public:
    MySqlQueryColorModel();
    virtual void setLimit(int lim=-1);
    virtual void enter(const QModelIndex& index, int value, const StackItem &item);
    QVariant data(const QModelIndex &index,int role)const Q_DECL_OVERRIDE;
    ~MySqlQueryColorModel();
public slots:
    void save();
signals:
    void startSave(int);
    void SavesChanged(int);
    void tableSaveProgress(int);
};

class MySqlQueryColorModelU:public MySqlQueryColorModel
{
    Q_OBJECT
private:
    bool isAll;
    int limit[4],last;
public:
    MySqlQueryColorModelU();
    void clear();
    void setLimit(int lim=-1,pref prefix=LC_,bool all=false);
    QVariant data(const QModelIndex &index,int role)const Q_DECL_OVERRIDE;
    ~MySqlQueryColorModelU();
};

#endif // MYSQLQUERYCOLORMODEL_H
