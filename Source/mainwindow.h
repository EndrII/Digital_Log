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
#include "settings.h"
#include "about.h"
#include <QDebug>
#include "help.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    GroupMenager *grm;
    settings conf;
    sqlDataBase *bd;
    StarastaMode *MS;
    QTabWidget *tab;
    QLabel *state;
    QTextEdit *LogPanel;
    QAction *mset,*bases,*grs,*connect_m,*closeA;
    void createMenu();
private slots:
    void Error(int,QString);
    void stateChanged(state_BD);
    void about(bool);
    void help(bool);
    void setings(bool);
    //void AchiveOk(QString);
    void connect_bd(bool);
    void Exit(bool);
    void BASES(bool);
    void GRM(bool);
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
