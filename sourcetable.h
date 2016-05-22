/****************************************************************************
**
** Copyright (C) 2016 A.N.Yankovich.
** Contact: https://vk.com/digitalface
**
** $QT_BEGIN_LICENSE:LGPL21$
** This file is distributed under the GPLv3 license.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef SOURCETABLE_H
#define SOURCETABLE_H

#include "core.h"
#include <QHBoxLayout>
class SourceTable : public QWidget
{
    Q_OBJECT
private:
    QWidget *Source;
    QHBoxLayout *lb;
public:
    explicit SourceTable(QWidget *parent = 0);
    void setSource(QWidget*);
    bool isEmpty()const;
    QWidget* getSource()const;
    ~SourceTable();
signals:
    void SourceChanged(QWidget*);
public slots:
};

#endif // SOURCETABLE_H
