#ifndef DATABASESMENAGER_H
#define DATABASESMENAGER_H
#include "sqldatabase.h"
#include <QTableView>
#include <QLabel>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardItemModel>
#include "TableModel.h"
class DataBasesMenager:public QDialog
{
    Q_OBJECT
private:
    sqlDataBase *bd;
    QTableView *ListdataBases;
     QStandardItemModel * model;
    QPushButton *Ok,*Cancle,*Create,*Delete;
    void white(bool);
    void showDataBases();
private slots:
    void okClick(bool);
    void cancleClick(bool);
    void createClick(bool);
    void deleteClick(bool);
protected:
public:
    DataBasesMenager(sqlDataBase *_bd, QWidget *ptr=NULL);
    ~DataBasesMenager();
};

#endif // DATABASESMENAGER_H
