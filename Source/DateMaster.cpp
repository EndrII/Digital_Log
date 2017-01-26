#include "DateMaster.h"
DateMaster::DateMaster(sqlDataBase *bd, const QString &tablename, const QDate &last, QWidget * ptr):
    QDialog(ptr)
{
    lastDate=last;
    tableTimeName=tablename;
    this->setModal(true);
    this->setMinimumWidth(330);
    database=bd;
    QVBoxLayout *box=new QVBoxLayout();
    QHBoxLayout *tempBox=new QHBoxLayout();
    BeginDate=new QDateEdit(QDate::currentDate());
    BeginDate->setDisplayFormat("dd.MM.yyyy");
    EndDate=new QDateEdit(QDate::currentDate().addYears(1));
    EndDate->setDisplayFormat("dd.MM.yyyy");
    tempBox->addWidget(BeginDate);
    tempBox->addWidget(EndDate);
    EvriMouth=new QCheckBox(ELanguage::getWord(EVRYMOUNTH));

    box->addLayout(tempBox);
    tempBox=new QHBoxLayout();
    //modeTab=new QTabWidget();
    QWidget *temp=new QWidget();
    QVBoxLayout *boxL=new QVBoxLayout();
    intervalFromMode1=new QSpinBox();
    intervalFromMode1->setMinimum(1);
    intervalFromMode1->setMaximum(365);
    boxL->addLayout(new QHBoxLayout());
    interval=new QLabel(ELanguage::getWord(INTERVAL));
    ((QHBoxLayout*)boxL->children().last())->addWidget(EvriMouth );
    ((QHBoxLayout*)boxL->children().last())->addWidget(interval );
    ((QHBoxLayout*)boxL->children().last())->addWidget(intervalFromMode1);
    dayNames=new QTableView();
    QStringList list;
    list<<ELanguage::getWord(MONDAY);
    list<<ELanguage::getWord(TUESDAY);
    list<<ELanguage::getWord(WEBNESDAY);
    list<<ELanguage::getWord(THURSDAY);
    list<<ELanguage::getWord(FRIDAY);
    list<<ELanguage::getWord(SATURDAY);
    list<<ELanguage::getWord(SUNDAY);
    dayNames->setModel(new QStringListModel(list));
    dayNames->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    boxL->addWidget(new QLabel(ELanguage::getWord(SELECT_DEYS)));
    boxL->addWidget(dayNames);
    temp->setLayout(boxL);
    //modeTab->addTab(temp,ELanguage::getWord(STATIC_MODE));
    tempBox->addWidget(temp);

    /*temp=new QWidget();
    QVBoxLayout *boxH=new QVBoxLayout();

    numberFirstDate=new QSpinBox();
    numberFirstDate->setMinimum(0);
    boxH->addWidget(new QLabel(ELanguage::getWord(DAY_OF_BEGIN)));
    boxH->addWidget(numberFirstDate);
    temp->setLayout(boxH);*/
  //  modeTab->addTab(temp,ELanguage::getWord(DATE_MODE));
    box->addLayout(tempBox);
    bar=new QProgressBar();
    bar->setValue(0);
    box->addWidget(bar);
    tempBox=new QHBoxLayout();
    cancel=new QPushButton(ELanguage::getWord(BUTTON_CANCLE));
    tempBox->addWidget(cancel);
    ok=new QPushButton(ELanguage::getWord(BUTTON_OK));
    tempBox->addWidget(ok);
    box->addLayout(tempBox);
    this->setLayout(box);
    connect(EvriMouth,SIGNAL(stateChanged(int)),this,SLOT(stateChange(int)));
    connect(ok,SIGNAL(clicked(bool)),this,SLOT(ok_(bool)));
    connect(cancel,SIGNAL(clicked(bool)),this,SLOT(cancle_(bool)));
}
void DateMaster::stateChange(int i){
    if(!i){
        interval->setText(ELanguage::getWord(INTERVAL));
        dayNames->setEnabled(true);
        intervalFromMode1->setMaximum(365);
    }else{
        interval->setText(ELanguage::getWord(FIRST_DATE));
        dayNames->setEnabled(false);
        intervalFromMode1->setMaximum(30);
    }
}
void DateMaster::ok_(bool){
    if(!EvriMouth->isChecked()){
        QDate date=BeginDate->date();
        bar->setMaximum(date.daysTo(EndDate->date())/intervalFromMode1->value());
        if(bar->maximum()>1000){
            QMessageBox::information(this,ELanguage::getWord(ERROR),ELanguage::getWord(ERROR_DATE));
            return;
        }
        if(date<=lastDate){
            QMessageBox::information(this,ELanguage::getWord(ERROR),ELanguage::getWord(ERROR_DATE_BACK));
            return;
        }
        int progress=0;
        while(date<=EndDate->date()){
            date=date.addDays(intervalFromMode1->value());
            int i=0;
            while(i<dayNames->selectionModel()->selectedRows().size()&&
                  date.dayOfWeek()!=dayNames->selectionModel()->selectedRows().at(i).row()+1){
                i++;
            }
            if(i!=dayNames->selectionModel()->selectedRows().size())
                database->Query_no_update("INSERT INTO "+tableTimeName+
                                         "(Даты) VALUES (STR_TO_DATE('"+
                                          date.toString("dd.MM.yyyy")+"','%d.%m.%Y'))");
            bar->setValue(++progress);
        }
    }else{
        QDate date=BeginDate->date();
        bar->setMaximum(date.daysTo(EndDate->date())/30);
        if(bar->maximum()>1000){
            QMessageBox::information(this,ELanguage::getWord(ERROR),ELanguage::getWord(ERROR_DATE));
            return;
        }
        if(date<=lastDate){
            QMessageBox::information(this,ELanguage::getWord(ERROR),ELanguage::getWord(ERROR_DATE_BACK));
            return;
        }
        int progress=0;
        date.setDate(date.year(),date.month(),1);
        while(date<=EndDate->date()){
            date=date.addMonths(1);
            database->Query_no_update("INSERT INTO "+tableTimeName+
                                     "(Даты) VALUES (STR_TO_DATE('"+
                                      date.addDays(intervalFromMode1->value()-1).toString("dd.MM.yyyy")+"','%d.%m.%Y'))");

            bar->setValue(++progress);
        }
    }
    database->Query_no_update("update "+tableTimeName+" set  id=("
                              "SELECT @number_:= @number_ + 1 FROM"
                              "(SELECT @number_:= 0) as tbl);");
    this->close();
}
void DateMaster::cancle_(bool){
    this->close();
}
DateMaster::~DateMaster(){

}
