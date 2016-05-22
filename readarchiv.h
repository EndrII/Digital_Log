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
#ifndef READARCHIV_H
#define READARCHIV_H
#include <QDialog>
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDir>
class ReadArchiv: public QDialog
{
    Q_OBJECT
private:
    bool test(const QString &str);
    QPushButton *ok;
    QComboBox *list;
    QString patch;
private slots:
    void okDown(bool);
public:
    ReadArchiv(const QString &str,QWidget *ptr=NULL);
signals:
    void Ok(QString);
};

#endif // READARCHIV_H
/*
 * в архиве лежат котологи при открыти читаеться дискриптор, в котором при экспорте был положен флаг об окончании ведения учёта, что сообщает редактору о том что базу данных редактрировать уже нельзя
*/
