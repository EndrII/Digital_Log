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
#ifndef SETINGS_H
#define SETINGS_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "sqldatabase.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>
#include <QFileDialog>
#include "datewidget.h"
#define STR_NULL "NULL"
class Setings : public QDialog
{
    Q_OBJECT
private:
    DateWidget *dateB1,*dateE1,*dateB2,*dateE2;
    sqlDataBase *Bd;
    QCheckBox *autoSave,*autoSem;
    QPushButton *complit,*default_,*cancle,*patchButton,*passChang,*passClear,*ArhivButton;
    QLineEdit *patch,*patchArhov;
private slots:
    void autoSemChang(int);
    void autocomplitClick(bool);
    void autodefault_Click(bool);
    void autocancleClick(bool);
    void patchButtonClick(bool);
    void ArhiveButtonClick(bool);
    void passChanged(bool);
    void passDelete(bool);
   // void textChanged(QString);
public:
    explicit Setings(sqlDataBase *bd,QWidget *parent = 0);
    static void write(const QString&);
    static QString read();
    ~Setings();
signals:

public slots:
};

#endif // SETINGS_H
