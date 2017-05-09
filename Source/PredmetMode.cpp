/****************************************************************************
**
** Copyright (C) 2016 A.N.Yankovich.
** Contact: https://vk.com/id223266985
**
** $QT_BEGIN_LICENSE:GPLv3$
** This file is distributed under the GPLv3 license.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "PredmetMode.h"

PredmetMode::PredmetMode(sqlDataBase *bd_, QWidget *parent) : QWidget(parent)
{
    bd=bd_;
    groups=new QComboBox();
    predmets=new QComboBox();
    times=new QComboBox();
    print_=new QPushButton(ELanguage::getWord(PRINT));
    print_->setShortcut(Qt::Key_Print);
    showCritikal=new QCheckBox(ELanguage::getWord(SHOW_CRITICAL));showCritikal->setCheckState(Qt::Checked);
    showWarning=new QCheckBox(ELanguage::getWord(SHOW_WARNING));showWarning->setCheckState(Qt::Checked);
    showNormal=new QCheckBox(ELanguage::getWord(SHOW_NORMAL));showNormal->setCheckState(Qt::Checked);
    //change=new QPushButton(ELanguage::getWord(LIMIT));
    limit_LC=new QSpinBox();
    limit_PC=new QSpinBox();
    limit_KRC=new QSpinBox();
    limit_RGRC=new QSpinBox();
    limit_LC->setMinimum(0);
    limit_PC->setMinimum(0);
    limit_KRC->setMinimum(0);
    limit_RGRC->setMinimum(0);
    table=new QTableView(this);
    prefix=new QComboBox();
    prefix->addItem(ELanguage::getWord(LABS_V));
    prefix->addItem(ELanguage::getWord(PRACT_V));
    prefix->addItem(ELanguage::getWord(KONTR_V));
    prefix->addItem(ELanguage::getWord(RGR_V));
    prefix->addItem(ELanguage::getWord(LABS_Z));
    prefix->addItem(ELanguage::getWord(KONTR_Z));
    prefix->addItem(ELanguage::getWord(RGR_Z));
    beginRange=new QDateEdit();
    beginRange->setDisplayFormat("dd.MM.yyyy");
    endRange=new QDateEdit(QDate::currentDate());
    endRange->setDisplayFormat("dd.MM.yyyy");
    beginRangeL=new QLabel(ELanguage::getWord(DATE_RANGE)+" "+ELanguage::getWord(BEGIN)+":");
    endRangeL=new QLabel(ELanguage::getWord(END)+":");
    //table->setFocusPolicy(Qt::NoFocus);
    norma=new QPushButton(ELanguage::getWord(NORMA_YAHR));
    qyer=bd->registration();
    model=new MySqlQueryColorModel();
    //model->setQuery(*qyer);
    table->setModel(model);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->setMinimumSize(800,480);
    createContextMenu();
    QVBoxLayout *box=new QVBoxLayout();
    QHBoxLayout *list=new QHBoxLayout();
    QVBoxLayout *stack=new QVBoxLayout();
    stack->addWidget(new QLabel(ELanguage::getWord(FILTERS)));
    stack->addWidget(showNormal);
    stack->addWidget(showWarning);
    stack->addWidget(showCritikal);
    list->addLayout(stack);

    stack=new QVBoxLayout();
    stack->addWidget(new QLabel(ELanguage::getWord(SELECTOR)));
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(BUTTON_GROUP)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(groups);
    list->addLayout(stack);

    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(PREDMET_MOD)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(predmets);
    list->addLayout(stack);


    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(DATE_MODE)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(times);
    list->addLayout(stack);

    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(WORK)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(prefix);
    list->addLayout(stack);

    stack=new QVBoxLayout();
    stack->addWidget(new QLabel(ELanguage::getWord(DATE_RANGE)));
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(BEGIN)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(beginRange);
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(END)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(endRange);
    stack->addWidget(print_);
    list->addLayout(stack);

    stack=new QVBoxLayout();
    stack->addWidget(new QLabel(ELanguage::getWord(PLAN_WORK_NOW)));
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(NORMA_YAHR)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(norma);
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(LC_WORKS)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(limit_LC);
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(PC_WORKS)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(limit_PC);
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(KRC_WORKS)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(limit_KRC);
    stack->addLayout(new QHBoxLayout());
    ((QHBoxLayout*)stack->children().back())->addWidget(new QLabel(ELanguage::getWord(RGRC_WORKS)+":"));
    ((QHBoxLayout*)stack->children().back())->addWidget(limit_RGRC);
    list->addLayout(stack);
    box->addLayout(list);
    box->addWidget(table);
    this->setLayout(box);
    //connect(bd,SIGNAL(ChangedBD()),this,SLOT(updateGroups()));
    connect(print_,SIGNAL(clicked(bool)),this,SLOT(PrintClick()));
    connect(limit_LC,SIGNAL(valueChanged(int)),this,SLOT(changed_limit_LC(int)));
    connect(limit_PC,SIGNAL(valueChanged(int)),this,SLOT(changed_limit_PC(int)));
    connect(limit_KRC,SIGNAL(valueChanged(int)),this,SLOT(changed_limit_KRC(int)));
    connect(limit_RGRC,SIGNAL(valueChanged(int)),this,SLOT(changed_limit_RGRC(int)));
    connect(times,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
    connect(predmets,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
    connect(groups,SIGNAL(currentIndexChanged(int)),this,SLOT(ComboWrite(int)));
    connect(prefix,SIGNAL(currentIndexChanged(int)),this,SLOT(GroupListChanged(int)));
    connect(beginRange,SIGNAL(dateChanged(QDate)),SLOT(updateTable()));
    connect(endRange,SIGNAL(dateChanged(QDate)),SLOT(updateTable()));
    connect(norma,SIGNAL(clicked(bool)),SLOT(normaClick()));
    connect(showNormal,SIGNAL(stateChanged(int)),this,SLOT(filterUpdate()));
    connect(showWarning,SIGNAL(stateChanged(int)),this,SLOT(filterUpdate()));
    connect(showCritikal,SIGNAL(stateChanged(int)),this,SLOT(filterUpdate()));
}
void PredmetMode::updateGroups(){
    groups->clear();
    groups->addItems(bd->getGroupList());
    ComboWrite(0);
}
void PredmetMode::keyPressEvent(QKeyEvent *event){
    if(times->currentText()!="*"&&(event->key()==Qt::Key_Return||event->key()==Qt::Key_Enter)){
        Enter();
    }
}
void PredmetMode::PrintClick(){
    ( new PrinterDialog(table,this))->exec();
}
void PredmetMode::GroupListChanged(int){
    QString name=predmets->currentText()+groups->currentText();
    QString time=times->currentText().replace('-','_');
    qyer->exec("call update_table('LC_"+name+"','времяУроки')");
    qyer->exec("call update_table('PC_"+name+"','времяУроки')");
    qyer->exec("call update_table('KRC_"+name+"','времяУроки')");
    qyer->exec("call update_table('RGRC_"+name+"','времяУроки')");
    qyer->exec("call update_table('LD_"+name+"','времяУроки')");
    qyer->exec("call update_table('KRD_"+name+"','времяУроки')");
    qyer->exec("call update_table('RGRD_"+name+"','времяУроки')");
    updateLimits();
    if(times->currentText()=="*"){
        updateTable(name);
        prefix->setEnabled(true);
        beginRange->setEnabled(true);
        beginRangeL->setEnabled(true);
        endRange->setEnabled(true);
        endRangeL->setEnabled(true);
    }else{
        updateTable(name,time);
        prefix->setEnabled(false);
        beginRange->setEnabled(false);
        beginRangeL->setEnabled(false);
        endRange->setEnabled(false);
        endRangeL->setEnabled(false);
    }
}
void PredmetMode::ComboWrite(int){
    predmets->clear();
    times->clear();
    times->addItem("*");
    times->addItems(bd->getDateListU());
    predmets->addItems(bd->getGroupPredmetsList(groups->currentText()));
    updateLimits();
    }
/*
void PredmetMode::Editing(){

}*/
void PredmetMode::updateTable(){
    updateTable(predmets->currentText()+groups->currentText());
}
void PredmetMode::updateTable(const QString& name,const short index, bool DESC){
    QString q="select ФИО as '"+ELanguage::getWord(NAME)+"'";
    listColumnHaider.clear();
    listColumnHaider.push_back("ФИО");
    QStringList temp=bd->getDateListU(beginRange->date(),endRange->date());

    for(QString i:temp){
        listColumnHaider.push_back(i);
        q+=","+i.replace('-','_')+ " as '"+QDate::fromString(i,"yyyy-MM-dd").toString("dd.MMMM.yyyy")+"'";
    }
    bool full=temp.size()==bd->getDateUCount();
    if(full){
        q+=",sum as '"+ELanguage::getWord(SUMM)+"'";
    }else{
        //table->repaint();
        listColumnHaider.push_back("sum");
    }
    QSpinBox * tempSpin;
        switch (prefix->currentIndex()) {
        case 0:
            q+=" from LC_"+name;
            ((MySqlQueryColorModelU*)model)->setLimit(limit_LC->value()*full,LC_,true);
            tempSpin=limit_LC;
            break;
        case 1:
            q+=" from PC_"+name;
            ((MySqlQueryColorModelU*)model)->setLimit(limit_PC->value()*full,PC_,true);
            tempSpin=limit_PC;
            break;
        case 2:
            q+=" from KRC_"+name;
            ((MySqlQueryColorModelU*)model)->setLimit(limit_KRC->value()*full,KRC_,true);
            tempSpin=limit_KRC;
            break;
        case 3:
            q+=" from RGRC_"+name;
            ((MySqlQueryColorModelU*)model)->setLimit(limit_RGRC->value()*full,RGRC_,true);
            tempSpin=limit_RGRC;
            break;
        case 4:
            q+=" from LD_"+name;
            ((MySqlQueryColorModelU*)model)->setLimit(limit_LC->value()*full,LC_,true);
            tempSpin=limit_LC;
            break;
        case 5:
            q+=" from KRD_"+name;
            ((MySqlQueryColorModelU*)model)->setLimit(limit_KRC->value()*full,KRC_,true);
            tempSpin=limit_KRC;
            break;
        case 6:
            q+=" from RGRD_"+name;
            ((MySqlQueryColorModelU*)model)->setLimit(limit_RGRC->value()*full,RGRC_,true);
            tempSpin=limit_RGRC;
            break;
        default:
            break;
        }
        if(!showNormal->isChecked()){
            q+=" where sum <= "+QString::number(tempSpin->value());
        }
        if(!showWarning->isChecked()){
            q+=" where NOT(sum <"+QString::number(tempSpin->value())+" and  sum>="+QString::number(tempSpin->value()/2)+")";
        }
        if(!showCritikal->isChecked()){
            q+=" where NOT sum< "+QString::number(tempSpin->value()/2);
        }
    q+=" ORDER BY "+listColumnHaider[index]+((DESC)?" DESC ":"");

    qDebug()<<qyer->exec(q)<<q;
    //model->setQuery(*qyer);
}
void PredmetMode::updateLimits(){
    float k;
    if(times->currentIndex()>0){
        k=1.0/(1+bd->getDateUCount()-times->currentIndex());
    }else{
        k=(float)bd->getDateUCount()/bd->getDateUCount(true);
    }
    limit_LC->setValue(bd->getGroupLimit(groups->currentText(),"LC_",predmets->currentText())*k);
    limit_PC->setValue(bd->getGroupLimit(groups->currentText(),"PC_",predmets->currentText())*k);
    limit_KRC->setValue(bd->getGroupLimit(groups->currentText(),"KRC_",predmets->currentText())*k);
    limit_RGRC->setValue(bd->getGroupLimit(groups->currentText(),"RGRC_",predmets->currentText())*k);
}
void PredmetMode::updateTable(const QString& name,const QString&time){
    QString q="select "+
            groups->currentText()+".ФИО as '"+ELanguage::getWord(NAME)+"', "+
            "PC_"+name+"."+time+" as '"+ELanguage::getWord(PRACT_V)+"', "+
            "LC_"+name+"."+time+" as '"+ELanguage::getWord(LABS_V)+"', "+
            "KRC_"+name+"."+time+" as '"+ELanguage::getWord(KONTR_V)+"', "+
            "RGRC_"+name+"."+time+" as '"+ELanguage::getWord(RGR_V)+"', "+
            "LD_"+name+"."+time+" as '"+ELanguage::getWord(LABS_Z)+"', "+
            "KRD_"+name+"."+time+" as '"+ELanguage::getWord(KONTR_Z)+"', "+
            "RGRD_"+name+"."+time+" as '"+ELanguage::getWord(RGR_Z)+"' "
            "from "+
            groups->currentText()+",LC_"+name+","+"PC_"+name+","+"KRC_"+name+","+
          "RGRC_"+name+","+"LD_"+name+","+"KRD_"+name+","+"RGRD_"+name+" "+
            "where "+
             groups->currentText()+".ФИО="+"LC_"+name+".ФИО AND "+
             groups->currentText()+".ФИО="+"PC_"+name+".ФИО AND "+
             groups->currentText()+".ФИО="+"KRC_"+name+".ФИО AND "+
             groups->currentText()+".ФИО="+"RGRC_"+name+".ФИО AND "+
             groups->currentText()+".ФИО="+"LD_"+name+".ФИО AND "+
             groups->currentText()+".ФИО="+"KRD_"+name+".ФИО AND "+
             groups->currentText()+".ФИО="+"RGRD_"+name+".ФИО ";
    if(!showNormal->isChecked()){
        q+=" and( LC_"+name+"."+time+"<"+QString::number(limit_LC->value())+
           " or PC_"+name+"."+time+"<"+QString::number(limit_PC->value())+
           " or KRC_"+name+"."+time+"<"+QString::number(limit_KRC->value())+
           " or RGRC_"+name+"."+time+"<"+QString::number(limit_RGRC->value())+
           " or LD_"+name+"."+time+"<"+QString::number(limit_LC->value())+
           " or KRD_"+name+"."+time+"<"+QString::number(limit_KRC->value())+
           " or RGRD_"+name+"."+time+"<"+QString::number(limit_RGRC->value())+") ";
    }
    if(!showWarning->isChecked()){
        q+=" and NOT(LD_"+name+"."+time+"<"+QString::number(limit_LC->value())+
           " or KRD_"+name+"."+time+"<"+QString::number(limit_KRC->value())+
           " or RGRD_"+name+"."+time+"<"+QString::number(limit_RGRC->value())+") ";
    }
    if(!showCritikal->isChecked()){
        q+=" and NOT(LC_"+name+"."+time+"<"+QString::number(limit_LC->value())+
           " or PC_"+name+"."+time+"<"+QString::number(limit_PC->value())+
           " or KRC_"+name+"."+time+"<"+QString::number(limit_KRC->value())+
           " or RGRC_"+name+"."+time+"<"+QString::number(limit_RGRC->value())+") ";
    }
    qDebug()<<q<<qyer->exec(q);

    ((MySqlQueryColorModelU*)model)->setLimit(limit_LC->value(),LC_);
    ((MySqlQueryColorModelU*)model)->setLimit(limit_PC->value(),PC_);
    ((MySqlQueryColorModelU*)model)->setLimit(limit_KRC->value(),KRC_);
    ((MySqlQueryColorModelU*)model)->setLimit(limit_RGRC->value(),RGRC_);
    //model->setQuery(*qyer);
}
void PredmetMode::Enter(){
  /*  bool ok;
    short indexRow=table->currentIndex().row();
    short indexColumn=table->currentIndex().column();
 //   QString tempName=model->data(model->index(indexRow,0)).toString();
    int tempData=QInputDialog::getInt(this,ELanguage::getWord(ENTER_VALUE),ELanguage::getWord(ENTER_FROM)+
                                      "("+model->headerData(indexColumn,Qt::Horizontal).toString()+") "+
                                      tempName,0,0,56,1,&ok);
    if(ok){
        if(indexColumn>4&&tempData>model->data(model->index(indexRow,indexColumn-3)).toInt()){
            QMessageBox::warning(this,ELanguage::getWord(WARNING),ELanguage::getWord(WRITE_WRONG));
            return;
        }
        QStringList prefix;
        prefix<<"NONE"<<"PC_"<<"LC_"<<"KRC_"<<"RGRC_"<<"LD_"<<"KRD_"<<"RGRD_";
        QString name=predmets->currentText()+groups->currentText();
        QString time=times->currentText().replace('-','_');
        qyer->exec("UPDATE "+prefix[table->currentIndex().column()]+name+" "
                   "SET "+time+"="+QString::number(tempData)+" WHERE ФИО='"+tempName+"'");
        //bd->sumCount(groups->currentText(),predmets->currentText(),indexColumn-1,model->data(model->index(indexRow,0)).toString());
        updateTable(name,time);
        if(++indexRow>=model->rowCount()){
            indexColumn++;
            indexRow=0;
        }
        table->selectionModel()->setCurrentIndex(model->index(indexRow,indexColumn),QItemSelectionModel::Select);
    }*/
}
void PredmetMode::normaClick(){
    (new LimitWorksWidget(groups->currentText(),predmets->currentText(),bd))->exec();
}
void PredmetMode::sortTableU(){
    updateTable(predmets->currentText()+groups->currentText(),table->selectionModel()->currentIndex().column());
}
void PredmetMode::sortTableD(){
    updateTable(predmets->currentText()+groups->currentText(),table->selectionModel()->currentIndex().column(),true);
}
void PredmetMode::filterUpdate(){
    QString name=predmets->currentText()+groups->currentText();
    QString time=times->currentText().replace('-','_');
    if(times->currentIndex()){
        updateTable(name,time);
    }else{
        updateTable(name);
    }
}
void PredmetMode::createContextMenu(){
    sortUP=new QAction(ELanguage::getWord(SORT_UP));
    connect(sortUP,SIGNAL(triggered(bool)),this,SLOT(sortTableU()));
    sortDOWN=new QAction(ELanguage::getWord(SORT_DOWN));
    connect(sortDOWN,SIGNAL(triggered(bool)),this,SLOT(sortTableD()));
    enter=new  QAction(ELanguage::getWord(ENTER));
    connect(enter,SIGNAL(triggered(bool)),this,SLOT(Enter()));
}
void PredmetMode::changed_limit_LC(int i){
    ((MySqlQueryColorModelU*)model)->setLimit(i,LC_,times->currentText()=="*");
    table->repaint();
}
void PredmetMode::changed_limit_PC(int i){
    ((MySqlQueryColorModelU*)model)->setLimit(i,PC_,times->currentText()=="*");
    table->repaint();
}
void PredmetMode::changed_limit_KRC(int i){
    ((MySqlQueryColorModelU*)model)->setLimit(i,KRC_,times->currentText()=="*");
    table->repaint();
}
void PredmetMode::changed_limit_RGRC(int i){
    ((MySqlQueryColorModelU*)model)->setLimit(i,RGRC_,times->currentText()=="*");
    table->repaint();
}
void PredmetMode::contextMenuEvent(QContextMenuEvent *event){
    QMenu menu(this);
    if(!times->currentIndex()){
        menu.addAction(sortUP);
        menu.addAction(sortDOWN);
    }else{
        menu.addAction(enter);
    }
    menu.exec(event->globalPos());
}
PredmetMode::~PredmetMode(){

}
