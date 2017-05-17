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
//#include "LimitWorksWidget.h"
#include "MySqlQueryColorModel.h"
#define _LC_     "Выполнено Лабораторных Работ"
#define _PC_     "Выполнено Практических Работ"
#define _KRC_    "Выполнено Контрольных Работ"
#define _RGRC_   "Выполнено Расчетно-графических Работ"
#define _LD_     "Сдано Лабораторных Работ"
#define _KRD_    "Сдано Контрольных Работ"
#define _RGRD_   "Сдано Расчетно-графически Работ"
class PredmetMode : public QWidget
{
    Q_OBJECT
private:
    sqlDataBase *bd;
    QCheckBox * showCritikal,*showWarning,*showNormal;

    QAnimationGroup *menu;
    QSqlQuery *qyer;
    MySqlQueryColorModelU *model;
    QStringList listColumnHaider;
    QAction *sortUP,*sortDOWN,*enter;
    QComboBox *groups;
    QComboBox *prefix;
    QLabel *beginRangeL,*endRangeL;
    QDateEdit *beginRange,*endRange;
    QComboBox *predmets;
    QComboBox *times;
    QPushButton *print_;//*norma;
    QSpinBox *limit_LC,*limit_PC,*limit_KRC,*limit_RGRC;
    QTableView *table;
    void createContextMenu();
    void updateLimits();
    void updateTable(bool isAll);
public:
    explicit PredmetMode(sqlDataBase*bd, QWidget *parent = 0);
    ~PredmetMode();
private slots:
    void Enter(QModelIndex, QModelIndex, QVector<int>);
//    void normaClick();
    void sortTableU();
    void sortTableD();
    void filterUpdate();
    void ComboWrite(int);
    void updateGroups();
    void updateTable();
    void changed_limit_LC(int);
    void changed_limit_PC(int);
    void changed_limit_KRC(int);
    void changed_limit_RGRC(int);
    void PrintClick();
    void GroupListChanged(int);
protected slots:
    void contextMenuEvent(QContextMenuEvent *event);
signals:
public slots:
    //void BDOpened(QString);
};

#endif // PredmetMode_H
