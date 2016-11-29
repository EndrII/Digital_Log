#ifndef DATEEDITOR_H
#define DATEEDITOR_H
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
#include "DateMaster.h"
class DateEditor:public QDialog
{
    Q_OBJECT
private:
    QAction *deleteDate,*AddDate;
    sqlDataBase *db;
    QSqlQueryModel *modelP,*modelU;
    QPushButton *autoP,*autoU;
    QTableView *tableP,*tableU;
    QSqlQuery *qyerP,*qyerU;
    void createContextMenu();
    void updateTables();
private slots:
    void DateMasterU(bool);
    void DateMasterP(bool);
    void contextMenuEvent(QContextMenuEvent *event);
    void AddDate_(bool);
    void deleteDate_(bool);
protected:
public:
    DateEditor(sqlDataBase* database,QWidget *ptr=NULL);
    ~DateEditor();
};

#endif // DATEEDITOR_H
