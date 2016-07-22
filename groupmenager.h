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
#ifndef GROUPMENAGER_H
#define GROUPMENAGER_H
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QInputDialog>
#include "database.h"
#include <QDialog>
#include <QSpacerItem>
#define Bheight 20
#define Bwidth 180
#define interval 10
class GroupMenager : public QDialog
{
    Q_OBJECT
private:
    //QSpacerItem *item;
    DataBase *bd;
    QComboBox *cb;
    QTableWidget *tableGroub;
    QPushButton *add,*dell,*save,*addU;
    void ComboWrite(QComboBox*);
    void TableDisconnect();
    void save_(bool logUpdate);
private slots:
    void resizeEvent(QResizeEvent *event);
    void ComboChsnged(int);
    void Save();
    void del();
    void addu();
    void edited();
    void add_();
public:
    explicit GroupMenager(DataBase *bd_,QWidget *parent = 0);
    ~GroupMenager();
signals:

public slots:
};

#endif // GROUPMENAGER_H
