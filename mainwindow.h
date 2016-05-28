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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <database.h>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenuBar>
#include <QVBoxLayout>
#include "sourcetable.h"
#include <QTableWidget>
#include <QPushButton>
#include <QTableWidget>
#include "starastamode.h"
#include "groupmenager.h"
#include "readarchiv.h"
#include "setings.h"
#include "about.h"
#include "help.h"
//#include <QRadioButton>
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
   // SourceTable *tbl;
    GroupMenager *grm;
    QString tempPatch;
    DataBase *bd;
    StarastaMode *MS;
    QTabWidget *tab;
    Setings *st;
    QLabel *state;
    QAction *arh,*openarh,*sv,*star,*mset,*grs,*crea;
   // QMenu *mset,*grs;
   // QRadioButton *StarastaMode;
   // QRadioButton *PrepodMode;
    void createGroupMenu();
    void CreateLogMenu();
    void createFileMenu();
    void CreateMenu();
    void CreateInterface();
private slots:
    void Error(int,QString);
    void DataBaseCreated(QString);
    void Save(bool);
    void about(bool);
    void help(bool);
    void setings(bool);
    void AchiveOk(QString);
    void StateBDChanged(StateDataBase stat);
   // void Grmclosed(QObject*);
    //void GroupChanged(Group*);
    //void GroupOpened(GroupVoid*);
    //void GroupCreated(Group*);
    //void controlGroupCreated(GroupVoid*);
    void ControlGroupDeleted(Group*);
    void start(bool);
    void CreateDataBase(bool);
    void OpenDB(bool);
    void Exit(bool);
    void GRM(bool);
    void toArhiv(bool);
    void closeArhiv(bool);
    void arhive(bool);
    void closeEvent(QCloseEvent *);
  //  void modeStarasta();
 //   void modePrepod();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
