#ifndef DATAMASTER_H
#define DATAMASTER_H
#include <QDialog>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QTabWidget>
#include <QComboBox>
#include <QCheckBox>
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
#include <QDateEdit>
class DateMaster: public QDialog
{
    Q_OBJECT
private:
    QProgressBar *bar;
    QString tableTimeName;
    sqlDataBase *database;
    QCheckBox *EvriMouth;
    QLabel *interval;
   // QTabWidget *modeTab;
    QPushButton * ok,*cancel;
    QDateEdit *BeginDate,*EndDate;
    QTableView *dayNames;
    QDate lastDate;
    QSpinBox *intervalFromMode1;
private slots:
    void cancle_(bool);
    void ok_(bool);
    void stateChange(int);
public:
    DateMaster(sqlDataBase *bd,const QString& tablename,const QDate &last,QWidget * ptr=NULL);
    ~DateMaster();
};

#endif // DATAMASTER_H
