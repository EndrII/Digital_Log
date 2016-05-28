
#ifndef __My
#define __My
#include "core.h"
#include <QTimer>
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
class ETime : public QObject
{
    Q_OBJECT
private:
    //QDate *interval;
    QTimer timer;
    bool started;
    bool isMounth;
    bool *auto_;
    QDate * date;
    QVector<QDate> SourceDate;
private slots:
    void _add();
public:
    explicit ETime(bool ismounth=false ,QObject *parent = 0);
    friend QDataStream &operator >>(QDataStream &stream,ETime &time);
    friend QDataStream &operator <<(QDataStream &stream,ETime &time);
    QDate& operator[](const unsigned int& index);
    void attach(QDate*,bool*);
    bool operator !=(const ETime&B);
    bool operator ==(const ETime&B);
    ETime& operator =(const ETime&B);
    bool timeTestBegin();
    bool timeTestEnd();
    bool start();
    void stop();
    void clear();
    bool isStarted();
    //bool tested();
    QStringList getList()const;
    int sizes(const ETime&A);
    int size()const;
    bool find(const QDate& data)const;
    QDate* getInterval()const;
    //void Update();
    ~ETime();
signals:
    void append(const QVector<QDate>*);
    void statusChanget(bool);
public slots:
};

#endif // TIME_H

