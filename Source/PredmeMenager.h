#ifndef PREDMEMENAGER_H
#define PREDMEMENAGER_H
#include <QTableView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QInputDialog>
#include "sqldatabase.h"
#include <QDialog>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QHeaderView>

class PredmeMenager:public QDialog
{
    Q_OBJECT
private:
    QAction *deletePredmet,*renamePredmet;
    sqlDataBase *bd;
    QSqlQuery *querL,*querR;
    QSqlQueryModel *modelL,*modelR;
    QString group;
    QTableView *tableGlobal,*tableLocal;
    QPushButton *add,*toGroup, *remove;
    void createContextMenu();
private slots:
    void contextMenuEvent(QContextMenuEvent *event);
    void Renam(bool);
    void del();
    void toGr();
    void dell(bool);
    void add_();
public:
    explicit PredmeMenager(sqlDataBase *bd_, const QString &gr, QWidget *parent = 0);
    ~PredmeMenager();
signals:

public slots:
};

#endif // PREDMEMENAGER_H
