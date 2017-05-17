#ifndef MYSQLQUERYCOLORMODEL_H
#define MYSQLQUERYCOLORMODEL_H
#include <QMap>
#include <list>
#include <QStandardItemModel>
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
enum class filter{normal=0x01,warning=0x02,critical=0x04};
class MySqlQueryColorModel:public QStandardItemModel
{
    Q_OBJECT
private:
    int limit;
    char filter_;
    bool isSumm,loaded;
    void stackWrite();
private slots:
    void calc(int i);
    void calc(QStandardItem *item);
protected:
    bool check(const filter& filt)const;
public:
    MySqlQueryColorModel();
    void recalc();
    void setLoad(bool);
    virtual filter checkLimit(const QModelIndex &index)const;
    void setSummColumn(bool);
    bool isSummColumn()const;
    virtual void setLimit(int lim=-1);
    virtual void setFilter(const char filter);
    QVariant data(const QModelIndex &index,int role)const Q_DECL_OVERRIDE;
    ~MySqlQueryColorModel();
public slots:
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
    virtual filter checkLimit(const QModelIndex &index)const;
    void clear();
    void setLimit(int lim=-1,pref prefix=LC_,bool all=false);
    ~MySqlQueryColorModelU();
};

#endif // MYSQLQUERYCOLORMODEL_H
