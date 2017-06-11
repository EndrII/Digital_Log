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
#include <QCheckBox>
#include <QVector>
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QDateEdit>
#include "MySqlQueryColorModel.h"
#include "MounthDialog.h"
#include "PrinterDialog.h"
#include <QKeyEvent>
#include <QProgressBar>
class StarastaMode : public QWidget
{
    Q_OBJECT
private:
    void ComboWrite();
    QCheckBox * showCritikal,*showWarning,*showNormal;
    QStringList listColumnHaider;
    sqlDataBase *bd;
    QAction *sortUP,*sortDOWN;
    QSqlQuery *qyer;
    MySqlQueryColorModel *model;
    QComboBox *groups;
    QDateEdit *beginRange,*endRange;
    QPushButton *print_;
    QSpinBox *limit;
    QTableView *table;
    void createContextMenu();
public:
    explicit StarastaMode(sqlDataBase*bd, QWidget *parent = 0);
    ~StarastaMode();
private slots:
    void Enter(QModelIndex,QModelIndex,QVector<int>);
    void sortTableU();
    void sortTableD();
    void filterUpdate();
    void updateTable();
    void updateGroups();
    void PrintClick();
    void limitChange(int);
    void GroupListChanged(int);
protected slots:
    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // STARASTAMODE_H
