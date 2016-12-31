#ifndef LIMITWORKSWIDGET_H
#define LIMITWORKSWIDGET_H
#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "sqldatabase.h"
#include "SpaceEngineComponents/System/ELanguage.h"
#include "languageindex.h"
class LimitWorksWidget:public QDialog
{
    Q_OBJECT
private:
    QPushButton *ok;
    QString group_,predmet_;

    QPushButton *cancle;
    QSpinBox *LC,*PC,*KRC,*RGRC;
    sqlDataBase *bd_;
private slots:
    void clickOk();
public:
    LimitWorksWidget(const QString &group, const QString&predmet, sqlDataBase*bd, QWidget*ptr=NULL);

    ~LimitWorksWidget();
};

#endif // LIMITWORKSWIDGET_H
