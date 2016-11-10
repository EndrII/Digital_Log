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
#ifndef STARASTAMODE_H
#define STARASTAMODE_H
#include <QTableView>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QContextMenuEvent>
#include <QLineEdit>
#include "sqldatabase.h"
#include <QLabel>
#include <QMenu>
#include <QVector>
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include "MounthDialog.h"
#include "Printer.h"

class StarastaMode : public QWidget
{
    Q_OBJECT
private:
    void ComboWrite();
    sqlDataBase *bd;
    QSqlQuery *qyer;
    QSqlQueryModel *model;
    QComboBox *groups;
    QComboBox *times;
    QPushButton *print_;
    QPushButton *change;
    //QPushButton *remove;
    QTableView *table;
    QAction *clearFilter,*alfavit,*maxtomin,*mintomax,*curentTime,*PrintHTML,*PrintPDF,*onlySumm;
    void createContextMenu();
public:
    explicit StarastaMode(sqlDataBase*bd, QWidget *parent = 0);
    ~StarastaMode();
private slots:
    void updateGroups(state_BD);
    void clearFilterClick(bool);
    void alfavitClick(bool);
    void maxtominClick(bool);
    void mintomaxClick(bool);
    void curentTimeClick(bool);
    void ClickPrintHTML(bool);
    void ClickPrintPDF(bool);
    void ClickOnlySumm(bool);
    void PrintClick();
    void changeClick();
   // void removeClick();
    void GroupListChanged(int);
protected slots:
    void contextMenuEvent(QContextMenuEvent *event);
signals:
public slots:
    void Editing();
    //void BDOpened(QString);
};

#endif // STARASTAMODE_H
