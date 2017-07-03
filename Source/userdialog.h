#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>
#include "sqldatabase.h"

namespace Ui {
class UserDialog;
}

class UserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserDialog(sqlDataBase* database, QWidget *parent = 0);
    ~UserDialog();

private:
    Users *users;
    sqlDataBase *db;
    Ui::UserDialog *ui;
private slots:
    void StudentChange(int);
    void TeacherChange(int);
    void HeadmanChange(int);
    void AdminChange(int);
};

#endif // USERDIALOG_H
