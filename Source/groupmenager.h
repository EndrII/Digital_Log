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
#include "PredmeMenager.h"
#include <QDateEdit>
#include <QComboBox>
class GroupMenager : public QDialog
{
    Q_OBJECT
private:
    QAction *deleteStudents,*renameStudents;
    sqlDataBase *bd;
    QDateEdit *receipt_date,*expiration_date;
    QComboBox *cb;
    QTableView *tableGroub;
    QPushButton *add,*dell,*addU,*predmets;
    void ComboWrite();
    void createContextMenu();
    void save_();
private slots:
    void contextMenuEvent(QContextMenuEvent *event);
    void ComboChsnged(int);
    void Renam(bool);
    void del();
    void delu(bool);
    void addu();
    void callPredmets();
    void add_();
public:
    explicit GroupMenager(sqlDataBase *bd_,QWidget *parent = 0);
    ~GroupMenager();
signals:

public slots:
};

#endif // GROUPMENAGER_H
