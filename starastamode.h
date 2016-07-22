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
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QContextMenuEvent>
#include <QLineEdit>
#include "database.h"
#include <QLabel>
#include <QMenu>
#include <QVector>
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include "MounthDialog.h"
#include "Printer.h"
#define Bheight 20
#define Bwidth 190
#define interval 10
#define thisGroup ((*bd->getDataList())[groups->currentIndex()].first)
#define thisVoidGroup ((*bd->getDataList())[groups->currentIndex()].second)
#define tableSumWidget(i) (*thisVoidGroup)[indexs[i]]

class StarastaMode : public QWidget
{
    Q_OBJECT
private:
    void ComboWrite(QComboBox*);
    void indexPush(GroupVoid *gr);
    //int oldIndex;
    QVector<unsigned short> indexs;
    DataBase *bd;
    QAction *clearFilter,*alfavit,*maxtomin,*mintomax,*curentTime,*PrintHTML,*PrintPDF,*onlySumm;
    void createContextMenu();
    void TableDisconnect();
public:
    explicit StarastaMode(DataBase*bd, QWidget *parent = 0);
    QComboBox *groups;
    QPushButton *add;
    QPushButton *change;
    QPushButton *remove;
    QTableWidget *table;
  //  static void print(QTableWidget *Table,QString patch="./");
    ~StarastaMode();
private slots:
    void clearFilterClick(bool);
    void alfavitClick(bool);
    void maxtominClick(bool);
    void mintomaxClick(bool);
    void curentTimeClick(bool);
    void ClickPrintHTML(bool);
    void ClickPrintPDF(bool);
    void ClickOnlySumm(bool);
    void addClick();
    void changeClick();
    void removeClick();
    void GroupListChanged(int);
protected slots:
    void resizeEvent(QResizeEvent*);
    void contextMenuEvent(QContextMenuEvent *event);
signals:
public slots:
    void StateChanged(StateDataBase st);
    void Editing();
    void GroupChanged(Group*);
    void BDOpened(QString);
    void GroupSaved(GroupVoid*);
    void GroupOpened(GroupVoid*);
    void controlGroupCreated(GroupVoid*);
    void ControlGroupChanged(GroupVoid*);
    void GroupCreated(Group*);
    void GroupDeleted(QString gr);
    void Redraw(GroupVoid*);
    void RedrawLite(GroupVoid*);

};

#endif // STARASTAMODE_H
