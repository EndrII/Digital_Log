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
#ifndef PredmetMode_H
#define PredmetMode_H
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
#include "PrinterDialog.h"
#include <QDateEdit>
class PredmetMode : public QWidget
{
    Q_OBJECT
private:
    sqlDataBase *bd;
    QAnimationGroup *menu;
    QSqlQuery *qyer;
    QSqlQueryModel *model;
    QComboBox *groups;
    QComboBox *prefix;
    QLabel *beginRangeL,*endRangeL;
    QDateEdit *beginRange,*endRange;
    QComboBox *predmets;
    QComboBox *times;
    QPushButton *print_;
    QPushButton *change;
    QTableView *table;
    void createContextMenu();
    void updateTable(const QString &name, const QString &time);
    void updateTable(const QString &name);
public:
    explicit PredmetMode(sqlDataBase*bd, QWidget *parent = 0);
    ~PredmetMode();
private slots:
    void keyPressEvent(QKeyEvent* event);
    void ComboWrite(int);
    void updateGroups();
    void updateTable();
    void changeClick();
    void PrintClick();
    void GroupListChanged(int);
protected slots:
    void contextMenuEvent(QContextMenuEvent *event);
signals:
public slots:
    void Editing();
    //void BDOpened(QString);
};

#endif // PredmetMode_H
