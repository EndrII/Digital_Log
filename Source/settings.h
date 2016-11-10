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
#ifndef SETINGS_H
#define SETINGS_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "sqldatabase.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDataStream>
#include <QAction>
#include <QMenu>
#include "datewidget.h"
#define STR_NULL "NULL"
#define defaultPass "user"
#define defaultUser "user"
#define defaultHost "localhost"
#define defaultPORT "3306"
#define defaultPatchFromConfFile "./settings/conf.cfg"
struct settings{
    QString pass;
    QString port;
    QString user;
    QString host;
    void default_();
    friend QDataStream& operator<<(QDataStream&stream,const settings& object);
    friend QDataStream& operator>>(QDataStream&stream,settings& object);
    settings& operator=(const settings& right);
};
class Settings : public QDialog
{
    Q_OBJECT
private:
    settings conf;
    sqlDataBase *Bd;
    QPushButton *complit,*default_,*cancle;
    QLineEdit *Login,*pass,*host,*port;
protected:
    static void writeConf(settings* conf,const QString& patch=defaultPatchFromConfFile);
private slots:
    void complitClick(bool);
    void defaultClick(bool);
    void cancleClick(bool);
public:
    explicit Settings(sqlDataBase *bd,QWidget *parent = 0);
    static settings readConf(const QString& patch=defaultPatchFromConfFile);
    ~Settings();
signals:

public slots:
};

#endif // SETINGS_H
