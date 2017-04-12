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
#include "groupmenager.h"
GroupMenager::GroupMenager(sqlDataBase *bd_, QWidget *parent) : QDialog(parent)
{
    bd=bd_;
    this->setModal(true);
    tableGroub=new QTableView(this);
    tableGroub->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableGroub->setModel(bd->getModel());
    bd->getModel()->clear();
    add=new QPushButton(ELanguage::getWord(BUTTON_CREATE),this);     add->setFocusPolicy(Qt::NoFocus);
    dell=new QPushButton(ELanguage::getWord(BUTTON_DELETE),this);    dell->setFocusPolicy(Qt::NoFocus);
    addU=new QPushButton(ELanguage::getWord(BUTTON_ADD_STUDENT),this);
    predmets=new QPushButton(ELanguage::getWord(PREDMET_EDITOR),this); predmets->setFocusPolicy(Qt::NoFocus);
    receipt_date=new QDateEdit();
    expiration_date=new QDateEdit();
    cb=new QComboBox(this);
    ComboWrite();
    connect(cb,SIGNAL(activated(int)),this,SLOT(ComboChsnged(int)));
    this->setMinimumSize(640,480);
    connect(addU,SIGNAL(clicked(bool)),this,SLOT(addu()));
    connect(add,SIGNAL(clicked(bool)),this,SLOT(add_()));
    connect(dell,SIGNAL(clicked(bool)),this,SLOT(del()));
    connect(predmets,SIGNAL(clicked(bool)),this,SLOT(callPredmets()));
    if(cb->count()>0)ComboChsnged(0);
    QVBoxLayout *box=new QVBoxLayout();
    QHBoxLayout *list=new QHBoxLayout();
    list->addWidget(cb);
    list->addWidget(add);
    list->addWidget(dell);
    list->addWidget(addU);
    list->addWidget(predmets);
    box->addLayout(list);
    list=new QHBoxLayout();
    list->addWidget(new QLabel(ELanguage::getWord(RECEIPT_DATE)));
    list->addWidget(receipt_date);
    box->addLayout(list);
    list=new QHBoxLayout();
    list->addWidget(new QLabel(ELanguage::getWord(EXPIRATION_DATE)));
    list->addWidget(expiration_date);
    box->addLayout(list);
    box->addWidget(tableGroub);
    this->setLayout(box);
    createContextMenu();
}
void GroupMenager::addu(){
    bd->addStudent(cb->currentText(),QInputDialog::getText(this,ELanguage::getWord(BUTTON_ADD_STUDENT),
                                          ELanguage::getWord(ADD_STUDENT_ABOUT)));
    bd->Query(QString("select name as '%0' from students where _group=(select id from groups where name='%1')").arg(
                  ELanguage::getWord(NAME)).arg(cb->currentText()));
}
void GroupMenager::Renam(bool){
    for(QModelIndex index :tableGroub->selectionModel()->selectedRows()){
        bd->Query_no_update(QString("update students set name='%0' where name='%1'").
                            arg(QInputDialog::getText(this,ELanguage::getWord(BUTTON_RENAME),
                                                      ELanguage::getWord(RENAME_SUDENTS_MSG))).
                            arg(tableGroub->model()->data(index).toString()));
    }
    bd->Query(QString("select name as '%0' from students where _group=(select id from groups where name='%1')").arg(
                  ELanguage::getWord(NAME)).arg(cb->currentText()));
}
void GroupMenager::del(){
    bd->deleteGrpoup(cb->itemText(cb->currentIndex()));
    this->ComboWrite();
}
void GroupMenager::add_(){
    bd->createGroup(QInputDialog::getText(this,ELanguage::getWord(NEW_GROUP_TITLE),
                                          ELanguage::getWord(NEW_GROUP_ABOUT)));
    this->ComboWrite();
}
void GroupMenager::callPredmets(){
    PredmeMenager *gr=new PredmeMenager(bd,cb->currentText(),this);
    gr->show();
}
void GroupMenager::createContextMenu(){
    deleteStudents=new QAction(ELanguage::getWord(BUTTON_DELETE));
    deleteStudents->setStatusTip(ELanguage::getWord(DELETE_STUDENTS_MSG));
    connect(deleteStudents,SIGNAL(triggered(bool)),this,SLOT(delu(bool)));

    renameStudents=new QAction(ELanguage::getWord(BUTTON_RENAME));
    renameStudents->setStatusTip(ELanguage::getWord(RENAME_SUDENTS_MSG));
    connect(renameStudents,SIGNAL(triggered(bool)),this,SLOT(Renam(bool)));

}
void GroupMenager::delu(bool){
    if(!QMessageBox::question(this,ELanguage::getWord(WARNING),
                             ELanguage::getWord(DELETE_STUDENTS_MSG),
                              ELanguage::getWord(YES), ELanguage::getWord(NO))){
        for(QModelIndex index :tableGroub->selectionModel()->selectedRows()){
            bd->deleteStudent(cb->currentText(),tableGroub->model()->data(index).toString());
        }
    }
}
void GroupMenager::ComboWrite(){
    cb->clear();
    cb->addItems(bd->getGroupList());
}
void GroupMenager::contextMenuEvent(QContextMenuEvent *event){
    QMenu menu(this);
    menu.addAction(renameStudents);
    menu.addAction(deleteStudents);
    menu.exec(event->globalPos());
}
void GroupMenager::ComboChsnged(int i){
    bd->Query(QString("select name as '%0' from students where _group=(select id from groups where name='%1')").arg(
                  ELanguage::getWord(NAME)).arg(cb->itemText(i)));
}
GroupMenager::~GroupMenager(){
}

