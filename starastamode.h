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
#include <QLineEdit>
#include "database.h"
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>
#define Bheight 20
#define Bwidth 150
#define interval 10
#define thisGroup ((*bd->getDataList())[groups->currentIndex()].first)
#define thisVoidGroup ((*bd->getDataList())[groups->currentIndex()].second)
class StarastaMode : public QWidget
{
    Q_OBJECT
private:
    void ComboWrite(QComboBox*);
    //int oldIndex;
    DataBase *bd;
    void TableDisconnect();
public:
    explicit StarastaMode(DataBase*bd, QWidget *parent = 0);
    QComboBox *groups;
    QPushButton *add;
    QPushButton *save;
    QPushButton *remove;
    QTableWidget *table;
    ~StarastaMode();
private slots:
    void addClick();
    void saveClick();
    void removeClick();
    void GroupListChanged(int);
protected slots:
    void resizeEvent(QResizeEvent*);
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
