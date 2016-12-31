#include "LimitWorksWidget.h"

LimitWorksWidget::LimitWorksWidget(const QString &group, const QString &predmet, sqlDataBase *bd, QWidget *ptr):
    QDialog(ptr)
{
    bd_=bd;
    ok=new QPushButton(ELanguage::getWord(BUTTON_OK));
    cancle=new QPushButton(ELanguage::getWord((BUTTON_CANCLE)));
    this->setModal(true);
    QVBoxLayout *box=new QVBoxLayout();
    LC=new QSpinBox();
    LC->setMinimum(0);
    group_=group;
    predmet_=predmet;
    LC->setValue(bd->getGroupLimit(group,"LC_",predmet));
    PC=new QSpinBox();
    PC->setMinimum(0);
    PC->setValue(bd->getGroupLimit(group,"PC_",predmet));
    KRC=new QSpinBox();
    KRC->setMinimum(0);
    KRC->setValue(bd->getGroupLimit(group,"KRC_",predmet));
    RGRC=new QSpinBox();
    RGRC->setMinimum(0);
    RGRC->setValue(bd->getGroupLimit(group,"RGRC_",predmet));
    box->addWidget(new QLabel(ELanguage::getWord(PLAN_WORK_YAHR)));
    QHBoxLayout *line=new QHBoxLayout();
    line->addWidget(new QLabel(ELanguage::getWord(LC_WORKS)));
    line->addWidget(LC);
    box->addLayout(line);
    line=new QHBoxLayout();
    line->addWidget(new QLabel(ELanguage::getWord(PC_WORKS)));
    line->addWidget(PC);
    box->addLayout(line);
    line=new QHBoxLayout();
    line->addWidget(new QLabel(ELanguage::getWord(KRC_WORKS)));
    line->addWidget(KRC);
    box->addLayout(line);
    line=new QHBoxLayout();
    line->addWidget(new QLabel(ELanguage::getWord(RGRC_WORKS)));
    line->addWidget(RGRC);
    box->addLayout(line);
    line=new QHBoxLayout();
    line->addWidget(cancle);
    line->addWidget(ok);
    box->addLayout(line);
    this->setLayout(box);
    connect(cancle,SIGNAL(clicked(bool)),SLOT(close()));
    connect(ok,SIGNAL(clicked(bool)),SLOT(clickOk()));
}
void LimitWorksWidget::clickOk(){
    bd_->setGroupLimit(group_,"LC_",predmet_,LC->value());
    bd_->setGroupLimit(group_,"PC_",predmet_,PC->value());
    bd_->setGroupLimit(group_,"KRC_",predmet_,KRC->value());
    bd_->setGroupLimit(group_,"RGRC_",predmet_,RGRC->value());
}
LimitWorksWidget::~LimitWorksWidget(){

}
