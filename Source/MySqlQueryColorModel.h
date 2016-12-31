#ifndef MYSQLQUERYCOLORMODEL_H
#define MYSQLQUERYCOLORMODEL_H
#include <QSqlQueryModel>
#include <QMap>
#include <QDebug>
#include <QBrush>
enum pref{LC_,PC_,KRC_,RGRC_};
class MySqlQueryColorModel:public QSqlQueryModel
{
private:
    //QMap<int, QColor> colorRows;
    int limit;

public:
    MySqlQueryColorModel();
    void setLimit(int lim=-1);
    //void setColorRov(const int row,const QColor& color);
    QVariant data(const QModelIndex &index,int role)const Q_DECL_OVERRIDE;
    ~MySqlQueryColorModel();
};

class MySqlQueryColorModelU:public QSqlQueryModel
{
private:
    //QMap<int, QColor> colorRows;
    bool isAll;
    int limit[4],last;
public:
    MySqlQueryColorModelU();
    void clear();
    void setLimit(int lim=-1,pref prefix=LC_,bool all=false);
    //void setColorRov(const int row,const QColor& color);
    QVariant data(const QModelIndex &index,int role)const Q_DECL_OVERRIDE;
    ~MySqlQueryColorModelU();
};

#endif // MYSQLQUERYCOLORMODEL_H
