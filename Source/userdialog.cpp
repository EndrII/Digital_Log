#include "userdialog.h"
#include "ui_userdialog.h"
#include <QInputDialog>
#include <QMessageBox>
UserDialog::UserDialog(sqlDataBase *database, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserDialog)
{
    ui->setupUi(this);
    db=database;
    users =new Users(4);
    if(db->getUserInformation(*users)){
        ui->PassAdmin->setCheckState(Qt::CheckState(!(*users)[(int)ut::Admin].pass.isEmpty()*2));
        ui->PassHeadman->setCheckState(Qt::CheckState(!(*users)[(int)ut::Headman].pass.isEmpty()*2));
        ui->PassStudent->setCheckState(Qt::CheckState(!(*users)[(int)ut::Student].pass.isEmpty()*2));
        ui->PassTeacher->setCheckState(Qt::CheckState(!(*users)[(int)ut::Teacher].pass.isEmpty()*2));
        connect(ui->PassStudent,SIGNAL(stateChanged(int)),SLOT(StudentChange(int)));
        connect(ui->PassHeadman,SIGNAL(stateChanged(int)),SLOT(HeadmanChange(int)));
        connect(ui->PassTeacher,SIGNAL(stateChanged(int)),SLOT(TeacherChange(int)));
        connect(ui->PassAdmin,SIGNAL(stateChanged(int)),SLOT(AdminChange(int)));

    }else{
        QMessageBox::information(this,"Отказ в доступе","У вас не достаточно прав для смены паролей");
        this->close();
    }
}
void UserDialog::StudentChange(int i){
    if(i){
        (*users)[(int)ut::Student].pass=(QInputDialog::getText(this,"Новый пароль","Введите новый пароль для пользовотеля \"Студент\"",QLineEdit::Normal));
        if(!(*users)[(int)ut::Teacher].pass.isEmpty()&&!db->setUserPass((*users)[(int)ut::Student])){
            QMessageBox::information(this,"Отказ в доступе","У вас не достаточно прав для смены пароля");
        }
    }else{
        (*users)[(int)ut::Student].pass="";
        if(!db->setUserPass((*users)[(int)ut::Student])){
            QMessageBox::information(this,"Отказ в доступе","У вас не достаточно прав для смены пароля");
        }
    }
}
void UserDialog::TeacherChange(int i){
    if(i){
        (*users)[(int)ut::Teacher].pass=(QInputDialog::getText(this,"Новый пароль","Введите новый пароль для пользовотеля \"Преподаватель\"",QLineEdit::Normal));
        if(!(*users)[(int)ut::Teacher].pass.isEmpty()&&!db->setUserPass((*users)[(int)ut::Teacher])){
            QMessageBox::information(this,"Отказ в доступе","У вас не достаточно прав для смены пароля");
        }
    }else{
        (*users)[(int)ut::Teacher].pass="";
        if(!db->setUserPass((*users)[(int)ut::Teacher])){
            QMessageBox::information(this,"Отказ в доступе","У вас не достаточно прав для смены пароля");
        }
    }
}
void UserDialog::HeadmanChange(int i){
    if(i){
        (*users)[(int)ut::Headman].pass=(QInputDialog::getText(this,"Новый пароль","Введите новый пароль для пользовотеля \"Старогста\"",QLineEdit::Normal));
        if(!(*users)[(int)ut::Headman].pass.isEmpty()&&!db->setUserPass((*users)[(int)ut::Headman])){
            QMessageBox::information(this,"Отказ в доступе","У вас не достаточно прав для смены пароля");
        }
    }else{
        (*users)[(int)ut::Headman].pass="";
        if(!db->setUserPass((*users)[(int)ut::Headman])){
            QMessageBox::information(this,"Отказ в доступе","У вас не достаточно прав для смены пароля");
        }
    }
}
void UserDialog::AdminChange(int i){
    if(i){
        (*users)[(int)ut::Admin].pass=(QInputDialog::getText(this,"Новый пароль","Введите новый пароль для пользовотеля \"Администратор\"",QLineEdit::Normal));
        if(!(*users)[(int)ut::Admin].pass.isEmpty()&&!db->setUserPass((*users)[(int)ut::Admin])){
            QMessageBox::information(this,"Отказ в доступе","У вас не достаточно прав для смены пароля");
        }
    }else{
        (*users)[(int)ut::Admin].pass="";
        if(!db->setUserPass((*users)[(int)ut::Admin])){
            QMessageBox::information(this,"Отказ в доступе","У вас не достаточно прав для смены пароля");
        }
    }
}
UserDialog::~UserDialog()
{
    delete ui;
    users->clear();
    delete users;
}
