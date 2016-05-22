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
#ifndef DATEWIDGET_H
#define DATEWIDGET_H

#include <QWidget>
#include <QDate>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QLabel>
class DateWidget : public QWidget
{
    Q_OBJECT
private:
    QDate date;
    QSpinBox *god,*day,*mon;
private slots:
    void changed(int);
public:
    explicit DateWidget(bool year=true,QWidget *parent = 0);
    explicit DateWidget(const QDate&Date,bool year=true, QWidget *parent=0);
    QDate getDate();
    void setDate(const QDate&);
    ~DateWidget();
signals:
    void valumeChanged(QDate);
public slots:
};

#endif // DATEWIDGET_H
