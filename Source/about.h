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
#ifndef ABOUT_H
#define ABOUT_H
#include <QDialog>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QHBoxLayout>
class About: public QDialog
{
    Q_OBJECT
private :
    QLabel *aboutLogo;
public:

    About(QWidget *ptr=NULL);
};

#endif // ABOUT_H
