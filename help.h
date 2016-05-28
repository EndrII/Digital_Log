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
#ifndef HELP_H
#define HELP_H
#include <QDialog>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QHBoxLayout>
#include <QTextBrowser>
class Help: public QDialog
{
    Q_OBJECT
private :

    QTextBrowser *aboutLogo;
private slots:
    void anchers(QUrl);
public:

    Help(QWidget *ptr=NULL);
};


#endif // HELP_H
