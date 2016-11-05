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
//#include <database.h>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QTime>
#include "starastamode.h"
#include "groupmenager.h"
#include "DataBasesMenager.h"
#include "readarchiv.h"
#include "setings.h"
#include "about.h"
#include <QDebug>
#include "help.h"
//#include <QRadioButton>
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    GroupMenager *grm;

    QString tempPatch;
    sqlDataBase *bd;
    StarastaMode *MS;
    QTabWidget *tab;
    Setings *st;
    QLabel *state;
    QTextEdit *LogPanel;
    QAction *arh,*openarh,*sv,*star,*mset,*bases,*grs,*crea,*closeA;
    void createGroupMenu();
    void createDataBaseMenu();
    void CreateLogMenu();
    void createFileMenu();
    void CreateMenu();
    void CreateInterface();
private slots:
    void Error(int,QString);
    void DataBaseCreated(QString);
    void addFilter(bool);
    void clearFilter(bool);
    void Save(bool);
    void about(bool);
    void help(bool);
    void setings(bool);
    void AchiveOk(QString);
    //void StateBDChanged(StateDataBase stat);
   // void Grmclosed(QObject*);
    //void GroupChanged(Group*);
    //void GroupOpened(GroupVoid*);
    //void GroupCreated(Group*);
    //void controlGroupCreated(GroupVoid*);
    //void ControlGroupDeleted(Group*);
    void start(bool);
    void CreateDataBase(bool);
    void OpenDB(bool);
    void Exit(bool);
    void BASES(bool);
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