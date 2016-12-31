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
#include <QDateEdit>
#include "MounthDialog.h"
#include "PrinterDialog.h"
#include <QKeyEvent>

class StarastaMode : public QWidget
{
    Q_OBJECT
private:
    void ComboWrite();
    QStringList listColumnHaider;
    sqlDataBase *bd;
    QAction *sortUP,*sortDOWN,*enter;
    QSqlQuery *qyer;
    QSqlQueryModel *model;
    QComboBox *groups;
    QDateEdit *beginRange,*endRange;
    QPushButton *print_;
    QSpinBox *limit;
    //QPushButton *change;
    //QPushButton *remove;
    QTableView *table;
    void createContextMenu();
    //void countSum();
public:
    explicit StarastaMode(sqlDataBase*bd, QWidget *parent = 0);
    ~StarastaMode();
private slots:
    //void sortChanged(QModelIndex, QModelIndex);
    void Enter();
    void sortTableU();
    void sortTableD();
    void updateTable(const short &index=0,bool DESC=false);
    void keyPressEvent(QKeyEvent* event);
    void updateGroups();
    void PrintClick();
    void limitChange(int);
   // void removeClick();
    void GroupListChanged(int);
protected slots:
    void contextMenuEvent(QContextMenuEvent *event);
signals:
public slots:
    //void BDOpened(QString);
};

#endif // STARASTAMODE_H
