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
#include "LimitWorksWidget.h"
#include "MySqlQueryColorModel.h"
class PredmetMode : public QWidget
{
    Q_OBJECT
private:
    sqlDataBase *bd;
    QCheckBox * showCritikal,*showWarning,*showNormal;

    QAnimationGroup *menu;
    QSqlQuery *qyer;
    QSqlQueryModel *model;
    QStringList listColumnHaider;
    QAction *sortUP,*sortDOWN,*enter;
    QComboBox *groups;
    QComboBox *prefix;
    QLabel *beginRangeL,*endRangeL;
    QDateEdit *beginRange,*endRange;
    QComboBox *predmets;
    QComboBox *times;
    QPushButton *print_,*norma;
    /*
     * ('LC_','успеваемость выполненно лаб'),
('PC_','успеваемость выполненно практических работ'),
('KRC_','успеваемость выполненно контрольных работ'),
('RGRC_','успеваемасть выполненно РГР'),
('LD_','успеваемость зашищенно лаб'),
('KRD_','успеваемость зашищенно практических работ'),
('RGRD_','успеваемость зашищенно РГР');
*/
    QSpinBox *limit_LC,*limit_PC,*limit_KRC,*limit_RGRC;
    QTableView *table;
    void createContextMenu();
    void updateLimits();
    void updateTable(const QString &name, const QString &time);
    void updateTable(const QString &name, const short index=0, bool DESC=false);
public:
    explicit PredmetMode(sqlDataBase*bd, QWidget *parent = 0);
    ~PredmetMode();
private slots:
    void Enter();
    void normaClick();
    void sortTableU();
    void sortTableD();
    void filterUpdate();
    void keyPressEvent(QKeyEvent* event);
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
