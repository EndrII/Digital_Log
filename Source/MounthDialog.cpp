#include "MounthDialog.h"

MounthDialog::MounthDialog(bool *mounthData, QWidget *ptr):
    QDialog(ptr)
{
    this->setModal(true);
    mounth=mounthData;
    QVBoxLayout *box=new QVBoxLayout;
    January=new QCheckBox("Январь");
    January->setCheckState((Qt::CheckState)(mounth[0]*2));
    box->addWidget(January);

    February=new QCheckBox("Февраль");
    February->setCheckState((Qt::CheckState)(mounth[1]*2));
    box->addWidget(February);

    March=new QCheckBox("Март");
    March->setCheckState((Qt::CheckState)(mounth[2]*2));
    box->addWidget(March);

    April=new QCheckBox("Апрель");
    April->setCheckState((Qt::CheckState)(mounth[3]*2));
    box->addWidget(April);

    May=new QCheckBox("Май");
    May->setCheckState((Qt::CheckState)(mounth[4]*2));
    box->addWidget(May);

    June=new QCheckBox("Июнь");
    June->setCheckState((Qt::CheckState)(mounth[5]*2));
    box->addWidget(June);

    July=new QCheckBox("Июль");
    July->setCheckState((Qt::CheckState)(mounth[6]*2));
    box->addWidget(July);

    August=new QCheckBox("Август");
    August->setCheckState((Qt::CheckState)(mounth[7]*2));
    box->addWidget(August);

    September=new QCheckBox("Сентябрь");
    September->setCheckState((Qt::CheckState)(mounth[8]*2));
    box->addWidget(September);

    October=new QCheckBox("Октябрь");
    October->setCheckState((Qt::CheckState)(mounth[9]*2));
    box->addWidget(October);

    November=new QCheckBox("Ноябрь");
    November->setCheckState((Qt::CheckState)(mounth[10]*2));
    box->addWidget(November);

    December=new QCheckBox("Декабрь");
    December->setCheckState((Qt::CheckState)(mounth[11]*2));
    box->addWidget(December);
    //connect
    ok=new QPushButton("Применить");
    box->addWidget(ok);
    connect(ok,SIGNAL(clicked(bool)),this,SLOT(apply(bool)));
    this->setLayout(box);
}
void MounthDialog::apply(bool){
    mounth[0]=January->checkState()==Qt::Checked;
    mounth[1]=February->checkState()==Qt::Checked;
    mounth[2]=March->checkState()==Qt::Checked;
    mounth[3]=April->checkState()==Qt::Checked;
    mounth[4]=May->checkState()==Qt::Checked;
    mounth[5]=June->checkState()==Qt::Checked;
    mounth[6]=July->checkState()==Qt::Checked;
    mounth[7]=August->checkState()==Qt::Checked;
    mounth[8]=September->checkState()==Qt::Checked;
    mounth[9]=October->checkState()==Qt::Checked;
    mounth[10]=November->checkState()==Qt::Checked;
    mounth[11]=December->checkState()==Qt::Checked;
    this->close();
}
MounthDialog::~MounthDialog(){

}
