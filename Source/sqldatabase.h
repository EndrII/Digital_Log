#ifndef SQLDATABASE_H
#define SQLDATABASE_H
#include "core.h"
#include <QtSql>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#define PORT 2277
/**
 * @brief The state_BD enum
 * состояние базы данных
 * disconected - отключена
 * stoped - подключена, но не ведет отчет
 * conectd - подключена и ведет отчет
 */
enum state_BD{disconected,stoped,conectd};
/**
 * @brief The sqlDataBase class
 * класс упровления базами данных
 */
class sqlDataBase:public QObject, protected QSqlDatabase{
    Q_OBJECT
private:
    QStringList tempList;
    QSqlQuery *qer;
    state_BD state;
    QSqlQueryModel *model;
private slots:
protected:
    /**
     * @brief stateChang проверка состояния базы данных.
     */
    void stateChang();
    /**
     * @brief error_msg
     * выброс сообщения об ошибке
     */
    void error_msg();
    /**
     * @brief SqlExec - чтение файла базы данных, преобразует его в запросы
     * @param sqlFile путь к файлу
     * @return вернет истену если всё прошло успешно в пративном случае лож
     */
    bool SqlExec(QSqlQuery*,const QString &sqlFile);

public:
    /**
     * @brief sqlDataBase констуктор базы данных
     */
    explicit sqlDataBase();
    /**
     * @brief registration регистрация дополнительного запроса для этой базы данных
     * ПРЕДУПРЕЖДЕНИЕ - зарегистрированные запросы не отслеживаются на ошибки!
     * @return вернет новый запрос.
     */
    QSqlQuery* registration();
    /**
     * @brief GetState
     * @return вернет статус базы данных
     */
    state_BD GetState();
    /**
     * @brief connect_to подключит оболочку к базе дынных, если вызвать метод без пораметров то подключение произойдет с прошлыми данными.
     * @param user логин пользовотеля
     * @param pass пароль
     * @param host хост размешения базы данных
     * @param port порт работы базы данных
     */
    void connect_to(const QString&user="", const QString&pass="", const QString &host="",const QString& port="");
    /**
     * @brief openDB откроет базу данных аналог use
     * @param BaseName имя базы данных
     */
    void openDB(const QString &BaseName);
    /**
     * @brief createGroup создаст группу
     * @param GroupName имя группы
     */
    void createGroup(const QString &GroupName);
    /**
     * @brief createPredmet создаст предмет
     * @param PredemtName имя нового предмета
     */
    void createPredmet(const QString &PredemtName);
    /**
     * @brief createDB создаст новую базу данных, и подготовит её к работе
     * @param BaseName имя
     * @return  вернет истину если всё прошло успешно лож в пративном случае.
     */
    bool createDB(const QString &BaseName);
    /**
     * @brief addPredmetGroup добавит предмет группе
     * @param gr группа
     * @param pred предмет
     */
    void addPredmetGroup(const QString&gr,const QString&pred);
    /**
     * @brief removePredmetGroup удалит предмет из группы
     * @param gr группа
     * @param pred предмет
     */
    void removePredmetGroup(const QString&gr,const QString&pred);
    /**
     * @brief deleteGrpoup удалит группу
      * @param name имя группы
     */
    void deleteGrpoup(const QString& name);
    /**
     * @brief deletePredmet удалит глобальный предмет
      * @param name имя
     */
    void deletePredmet(const QString& name);
    /**
     * @brief deleteStudent удалит студента из группы
     * @param group группа
     * @param name имя студента
     */
    void deleteStudent(const QString &group, const QString name);
    /**
     * @brief getModel
     * @return вернет стандартную модель предстовления стандартного запроса
     */
    QSqlQueryModel* getModel()const;
    /**
     * @brief getGroupList
     * @return вернет список всех групп
     */
    QStringList& getGroupList();
    /**
     * @brief getGroupPredmetsList вернет список предметов для группы.
     * @param group группа для которой будет получн список предметов.
     * @return вернет список предметов группы.
     */
    QStringList& getGroupPredmetsList(const QString & group);
    /**
     * @brief getDateListP
     * @return веренет список всех дат для пропусков
     */
    QStringList& getDateListP();//propuski
    /**
     * @brief getDateListU
     * @return вернет список всех дат для предметов
     */
    QStringList& getDateListU();//uspevaimost
    /**
     * @brief Query выполнить запрос
     */
    void Query(const QString&);
    /**
     * @brief StartDB запустит базу данных (начнет отчет)
     * @param dayU новый день отчетности успеваемости (1-28)
     * @param dayP новый день недели отчетности пропусков, (0-6)
     */
    void StartDB(const char dayU=-1,const char dayP=-1);
    /**
     * @brief StopBD остоновит базу данных
     * @param toArhive следует ли переносить базу данных в архив.
     */
    void StopBD(const bool toArhive=false);
    /**
     * @brief Query_no_update выполнить запрос не обновляя модель запроса
     * @return верент истену если всё прошло успешно
     */
    bool Query_no_update(const QString&);
    /**
     * @brief addStudent добавит группе студента
     * @param group группа
     * @param name имя студента
     */
    void addStudent(const QString& group,const QString name);

    ~sqlDataBase();

signals:
    /*
     *
     * 2 - Важный текст
     * 1 - простой текст
     * 0 - простая ошибка с текстом
    */
    /**
     * @brief stateChanged сигнализирует о изменении статуса базы данных.
     */
    void stateChanged(state_BD);
    /**
     * @brief Message передает сообщение от базы данных
     */
    void Message(int,QString);
    /**
     * @brief ChangedBD если в базе данных произошли изменения
     */
    void ChangedBD();
};
#endif // SQLDATABASE_H

