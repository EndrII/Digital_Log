#include "DateMaster.h"
DateMaster::DateMaster(sqlDataBase *bd, const QString &tablename, QWidget * ptr):
    QDialog(ptr)
{
    tableTimeName=tablename;
    this->setModal(true);
    this->setMinimumWidth(330);
    database=bd;
    QVBoxLayout *box=new QVBoxLayout();
    QHBoxLayout *tempBox=new QHBoxLayout();
    BeginDate=new QLineEdit("00.00.0000");
    EndDate=new QLineEdit("00.00.0000");
    tempBox->addWidget(BeginDate);
    tempBox->addWidget(EndDate);
    box->addLayout(tempBox);
    tempBox=new QHBoxLayout();
    modeTab=new QTabWidget();
    QWidget *temp=new QWidget();
    QVBoxLayout *boxL=new QVBoxLayout();
    intervalFromMode1=new QSpinBox();
    intervalFromMode1->setMinimum(1);
    intervalFromMode1->setMaximum(365);
    boxL->addWidget(intervalFromMode1);
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
    boxL->addWidget(dayNames);
    temp->setLayout(boxL);
    modeTab->addTab(temp,ELanguage::getWord(STATIC_MODE));
    temp=new QWidget();
    QVBoxLayout *boxH=new QVBoxLayout();

    numberFirstDate=new QSpinBox();
    numberFirstDate->setMinimum(0);
    boxH->addWidget(new QLabel(ELanguage::getWord(DAY_OF_BEGIN)));
    boxH->addWidget(numberFirstDate);
    temp->setLayout(boxH);
    modeTab->addTab(temp,ELanguage::getWord(DATE_MODE));
    tempBox->addWidget(modeTab);
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
    connect(ok,SIGNAL(clicked(bool)),this,SLOT(ok_(bool)));
    connect(cancel,SIGNAL(clicked(bool)),this,SLOT(cancle_(bool)));
}
void DateMaster::ok_(bool){
    if(!modeTab->currentIndex()){
        QDate date=QDate::fromString(BeginDate->text(),"dd.MM.yyyy"),
                end=QDate::fromString(EndDate->text(),"dd.MM.yyyy");
        bar->setMaximum(date.daysTo(end)/intervalFromMode1->value());
        if(bar->maximum()>1000||!date.isValid()||!end.isValid()){
            QMessageBox::information(this,ELanguage::getWord(ERROR),ELanguage::getWord(ERROR_DATE));
            return;
        }
        int progress=0;
        while(date<=end){
            date=date.addDays(intervalFromMode1->value());
            int i=0;
            while(i<dayNames->selectionModel()->selectedRows().size()&&
                  date.dayOfWeek()!=dayNames->selectionModel()->selectedRows().at(i).row()+1){
                i++;
            }
            if(i==dayNames->selectionModel()->selectedRows().size())
                database->Query_no_update("INSERT INTO "+tableTimeName+
                                         "(Даты) VALUES (STR_TO_DATE('"+
                                          date.toString("dd.MM.yyyy")+"','%d.%m.%Y'))");
            bar->setValue(++progress);
        }
    }else{
        QDate date=QDate::fromString(BeginDate->text(),"dd.MM.yyyy"),
                end=QDate::fromString(EndDate->text(),"dd.MM.yyyy");
        bar->setMaximum(date.daysTo(end)/30);
        if(bar->maximum()>1000||!date.isValid()||!end.isValid()){
            QMessageBox::information(this,ELanguage::getWord(ERROR),ELanguage::getWord(ERROR_DATE));
            return;
        }
        int progress=0;
        date.setDate(date.year(),date.month(),1);
        while(date<=end){
            date=date.addMonths(1);
            database->Query_no_update("INSERT INTO "+tableTimeName+
                                     "(Даты) VALUES (STR_TO_DATE('"+
                                      date.addDays(numberFirstDate->value()-1).toString("dd.MM.yyyy")+"','%d.%m.%Y'))");

            bar->setValue(++progress);
        }
    }
    this->close();
}
void DateMaster::cancle_(bool){
    this->close();
}
DateMaster::~DateMaster(){

}
