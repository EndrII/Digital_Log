#include "DateEditor.h"

DateEditor::DateEditor(sqlDataBase *database, QWidget *ptr):
    QDialog(ptr)
{
    this->setModal(true);
    db=database;
    QHBoxLayout *mainBox=new QHBoxLayout();
    QVBoxLayout *box=new QVBoxLayout();
    autoP=new QPushButton(ELanguage::getWord(AUTO_DATE_P));
    tableP=new QTableView();
    modelP=new QSqlQueryModel();
    qyerP=db->registration();
    tableP->setModel(modelP);
    tableP->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    box->addWidget(new QLabel(ELanguage::getWord(DATE_P)));
    box->addWidget(autoP);
    box->addWidget(tableP);
    mainBox->addLayout(box);

    box=new QVBoxLayout();
    autoU=new QPushButton(ELanguage::getWord(AUTO_DATE_U));
    tableU=new QTableView();
    modelU=new QSqlQueryModel();
    qyerU=db->registration();
    tableU->setModel(modelU);
    tableU->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    box->addWidget(new QLabel(ELanguage::getWord(DATE_U)));
    box->addWidget(autoU);
    box->addWidget(tableU);
    mainBox->addLayout(box);
    this->setLayout(mainBox);
    updateTables();
    createContextMenu();
    connect(autoP,SIGNAL(clicked(bool)),this,SLOT(DateMasterP(bool)));
    connect(autoU,SIGNAL(clicked(bool)),this,SLOT(DateMasterU(bool)));
}
void DateEditor::DateMasterP(bool){
    if(modelP->rowCount()>0){
        (new DateMaster(db,"attendance",modelP->data(modelP->index(modelP->rowCount()-1,0)).toDate(),this))->exec();
    }else{
        (new DateMaster(db,"attendance",QDate(),this))->exec();
    }
    updateTables();
}
void DateEditor::DateMasterU(bool){
    if(modelU->rowCount()>0){
        (new DateMaster(db,"performance",modelU->data(modelU->index(modelU->rowCount()-1,0)).toDate(),this))->exec();
    }else{
        (new DateMaster(db,"performance",QDate(),this))->exec();
    }
    updateTables();
}
void DateEditor::updateTables(){
    qyerP->exec("select point from dates where date_group=1");
    modelP->setQuery(*qyerP);
    qyerU->exec("select point from dates where date_group=2");
    modelU->setQuery(*qyerU);

}
void DateEditor::contextMenuEvent(QContextMenuEvent *event){
    QMenu menu(this);
    menu.addAction(AddDate);
    menu.addAction(deleteDate);
    menu.exec(event->globalPos());
}
void DateEditor::AddDate_(bool){
    if(this->focusWidget()==tableP){
        QString q="INSERT INTO времяПропуски(Даты) VALUES (STR_TO_DATE('";
        QString ok;
        ok= QInputDialog::getText(this,ELanguage::getWord(DATE_P),
                              ELanguage::getWord(DATE_P_ABOUT));
        ok.remove(' ');
        if(ok.isEmpty())
            return;
        if(modelP->rowCount()>0 &&
           (QDate::fromString(ok,"dd.MM.yyyy")<modelP->data(modelP->index(modelP->rowCount()-1,0)).toDate()||
           QDate::fromString(ok,"dd.MM.yyyy")<QDate::currentDate())){
                QMessageBox::information(this,ELanguage::getWord(ERROR),ELanguage::getWord(ERROR_DATE_BACK));
                return;
        }
        q+=ok;
        q+="', '%d.%m.%Y'))";
        qDebug()<<q<<" result="<<qyerP->exec(q);
        qyerP->exec("select Даты  from времяПропуски");
        modelP->setQuery(*qyerP);
        db->Query_no_update("update времяПропуски set  id=("
                                  "SELECT @number_:= @number_ + 1 FROM"
                                  "(SELECT @number_:= 0) as tbl);");
    }
    if(this->focusWidget()==tableU){
        QString q="INSERT INTO времяУроки(Даты) VALUES (STR_TO_DATE('";
        QString ok;
        ok= QInputDialog::getText(this,ELanguage::getWord(DATE_U),
                              ELanguage::getWord(DATE_U_ABOUT));
        ok.remove(' ');
        if(ok.isEmpty())
            return;
        if(modelU->rowCount()>0 &&
           (QDate::fromString(ok,"dd.MM.yyyy")<modelU->data(modelU->index(modelU->rowCount()-1,0)).toDate()||
            QDate::fromString(ok,"dd.MM.yyyy")<QDate::currentDate())){
                QMessageBox::information(this,ELanguage::getWord(ERROR),ELanguage::getWord(ERROR_DATE_BACK));
                return;
        }

        q+=ok;        q+="', '%d.%m.%Y'))";

        qDebug()<<q<<" result="<<qyerU->exec(q);
        qyerU->exec("select Даты  from времяУроки");
        modelU->setQuery(*qyerU);
        db->Query_no_update("update времяУроки set  id=("
                                  "SELECT @number_:= @number_ + 1 FROM"
                                  "(SELECT @number_:= 0) as tbl);");
    }
}
void DateEditor::deleteDate_(bool){
    if(this->focusWidget()==tableP){
        for(QModelIndex index :tableP->selectionModel()->selectedRows()){
            if(tableP->model()->data(index).toDate()>QDate::currentDate()){
                QString temp=QString("DELETE FROM dates WHERE point='%0' and date_group=1").
                        arg(tableP->model()->data(index).toDate().toString("yyyy-MM-dd"));
                qDebug()<<qyerP->exec(temp)<<": "+temp;
            }
        }
        qyerP->exec("select point from dates where date_group=1");
        modelP->setQuery(*qyerP);
    }
    if(this->focusWidget()==tableU){
        for(QModelIndex index :tableU->selectionModel()->selectedRows()){
            if(tableU->model()->data(index).toDate()>QDate::currentDate())
            qyerU->exec(QString("DELETE FROM dates WHERE point='%0' and date_group=2").
                        arg(tableU->model()->data(index).toDate().toString("yyyy-MM-dd")));
        }
        qyerU->exec("select point from dates where date_group=2");
        modelU->setQuery(*qyerU);
    }
}
void DateEditor::createContextMenu(){
    deleteDate=new QAction(ELanguage::getWord(BUTTON_DELETE));
    connect(deleteDate,SIGNAL(triggered(bool)),this,SLOT(deleteDate_(bool)));
    AddDate=new QAction(ELanguage::getWord(BUTTON_ADD_DATE));
    connect(AddDate,SIGNAL(triggered(bool)),this,SLOT(AddDate_(bool)));
}
DateEditor::~DateEditor(){
    delete modelP;
    delete modelU;
    delete qyerP;
    delete qyerU;
    delete deleteDate;
    delete AddDate;
}
