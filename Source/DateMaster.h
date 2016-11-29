#ifndef DATAMASTER_H
#define DATAMASTER_H
#include <QDialog>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QTabWidget>
#include <QComboBox>
#include <QAction>
#include <QHBoxLayout>
#include <QTableView>
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QProgressBar>
#include <QMessageBox>
#include <QStringListModel>
#include "SpaceEngineComponents/System/ELanguage.h"
#include "sqldatabase.h"
class DateMaster: public QDialog
{
    Q_OBJECT
private:
    QProgressBar *bar;
    QString tableTimeName;
    sqlDataBase *database;
    QTabWidget *modeTab;
    QPushButton * ok,*cancel;
    QLineEdit *BeginDate,*EndDate;
    QTableView *dayNames;
    QSpinBox *intervalFromMode1,*numberFirstDate;
    //QComboBox *intervalFromMode2;
private slots:
    void cancle_(bool);
    void ok_(bool);
public:
    DateMaster(sqlDataBase *bd,const QString& tablename,QWidget * ptr=NULL);
    ~DateMaster();
};

#endif // DATAMASTER_H
