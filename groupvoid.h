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
#ifndef GROUPVOID_H
#define GROUPVOID_H
#include "group.h"
#include "Etime.h"
class GroupVoid:public QObject
{
    Q_OBJECT
private:
    Group *colums_TEST;
    ETime *rows_TEST;
    bool kolendar[12];
    void clear();
    int gotWeek;
    int limit;
private slots:
    void Schanged(Group *);
    void append(const QVector<QDate>*);
protected:
    Group *colums;
    ETime *rows;
    QVector<ui> summa;
    QVector<QVector<ui>*> dataStudents;
public:
    explicit GroupVoid(Group*gr, ETime *system,QObject *ptr=NULL);
    bool addRecord(QVector<ui>*parent=NULL);
    void autoClear();
    QString getName()const;
    int getGotWeek()const;
    bool ChangeTop(QVector<ui>*parent);
    bool Write(const QString& patch);
    bool Read(const QString& patch);
    bool toArchive(const QString&patch);
    bool isdrawColumn(const int&i);
    void resetSumm();
    //void printToHtml(const QString);
    ui operator[](const int&);
    bool * getKolendar();
    int size()const;
    void setLim(const int&lim=48);
    int getLim()const;
    ui getItem(const int row,const int col);
    QStringList getRowsHeader()const;
    QStringList getColumsHeader()const;
    bool ChangeTopElement(const ui& index,const ui& value);
    bool ChangeTopElement(const QString& name,const int& value);
    friend QDataStream &operator<<(QDataStream& steam,GroupVoid& gr);
    friend QDataStream &operator>>(QDataStream& steam,GroupVoid& gr);
    ~GroupVoid();
signals:
    void noWritedData();
    void Changed(GroupVoid*);
    void Warning(GroupVoid*,QString);
};

#endif // GROUPVOID_H
