#ifndef SQLDATABASE_H
#define SQLDATABASE_H
#include "core.h"
#include <QtSql>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStandardItemModel>
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
    void error_msg(const QString &Messag="none");
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
     * @brief transformQuery transformation a date of the query to standartItemModel
     * @param columnData select column of the data
     * @param rowData select row of the data
     * @param valueData select value from the table
     * @return feedback
     */
    bool transformQuery(const QString& columnData,const QString& rowData,const QString& valueData,QStandardItemModel*);
    /**
     * @brief writeLine write a line into a selected table if this line created then update this
     * @param tableName name of table
     * @param column name of the column column
     * @param row name of the row column
     * @value value name of the value column
     * @param columnData data of column
     * @param rowData data of row
     * @param valueData the inserted value
     * @return return boolean
     * @example writeLine(Graf,x,y,val,1,1,1.333);
     */
    bool writeLine(const QString& tableName,const QString& column,const QString& row,const QString& value,const QString& columnData,const QString& rowData,const QString& valueData);
    /**
     * @brief writeTableModel this function write a table into connectiondatabase;
     * @param tableName name of table with datas
     * @param columnData name of header of the columns
     * @param rowData name of header of the rows
     * @param valueData name of dataitems
     * @return true if all done else false
     */
    bool writeTableModel(const QString& tableName,const QString& columnData,const QString& rowData,const QString& valueData,QStandardItemModel*);
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
    void createGroup(QString GroupName);
    /**
     * @brief createPredmet создаст предмет
     * @param PredemtName имя нового предмета
     */
    void createPredmet(QString PredemtName);
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
     * @brief sumCount пересчитает суммы для указанной группы по предмету.
     * @param group группа для которой будет выполнен пересчёт
     * @param predmet предмет для которого будет выполнятся пересчёт
     * @param index индекс префикса для которого будет выполнен пересчёт
     */
    void sumCount(const QString & group, const QString& predmet,const int &index);
    /**
     * @brief sumCount пересчитает суммы для указанной группы по предмету.
     * @param group группа для которой будет выполнен пересчёт
     * @param predmet предмет для которого будет выполнятся пересчёт
     * @param index индекс префикса для которого будет выполнен пересчёт
     * @param name имя запеси для которой будет выполнятся обновление
     */
    void sumCount(const QString & group, const QString& predmet,const int &index,const QString & name);

  /*
     * @brief sumCount пересчитает суммы для указанной группы по предмету.
     * @param group группа для которой будет выполнен пересчёт
     * @param predmet предмет для которого будет выполнятся пересчёт
     *
    void sumCount(const QString & group, const QString& predmet);*/
    /**
     * @brief sumCount пересчитает суммы для указанной группы.
     * @param group - имя группы дял которой буду перещетанны столбцы.
     */
    void sumCount(const QString & group);
    /**
     * @brief sumCount
     * @param group
     * @param name имя запеси
     */
    void sumCount(const QString & group,const QString &name);
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
     * @param beginRange первая дата
     * @param endRange  конечная дата
     * @return веренет список всех дат для пропусков
     */
    QStringList& getDateListP(const QDate& beginRange=QDate(0,0,0),const QDate& endRange=QDate::currentDate());//propuski
    /**
     * @brief getDateListU
     * @param beginRange первая дата
     * @param endRange  конечная дата
     * @return вернет список всех дат для предметов
     */
    QStringList& getDateListU(const QDate &beginRange=QDate(0,0,0), const QDate &endRange=QDate::currentDate());//uspevaimost
    /**
     * @brief getDateUCount получит количесвто дат по успеваемости
     * @return количество дат по успеваемости
     */
    int getDateUCount(bool all=false);
    /**
     * @brief getDatePCount получит количество по пропускам
     * @return количесвто дат по пропускам
     */
    int getDatePCount(bool all=false);
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
    /**
     * @brief getGroupLimit вернет лимит пропусков выбранной группы
     * @param group имя группы
     * @return вернет количество пропусков
     */
    int getGroupLimit(const QString& group);
    /**
     * @brief setGroupLimit тоже самое но для успеваемости
     * @param group группа
     * @param prefix префикс
     * @param predmet предмет
     * @return вернет количество пропусков
     */
    int getGroupLimit(const QString &group,const QString &prefix,const QString &predmet);
    /**
     * @brief setGroupLimit устатновит новый лимит группе
     * @param group группа которой будет установлен новый лимит
     * @param limit новый лимит
     */
    void setGroupLimit(const QString& group,const int& limit);
    /**
     * @brief setGroupLimit тоже самое но для успеваемости
     * @param group группа
     * @param prefix префикс
     * @param predmet предмет
     * @param limit новый лимит
     */
    void setGroupLimit(const QString &group,const QString &prefix,const QString &predmet, const int &limit);
    /**
     * @brief removeFirstAndLastChars удалит в конце и в начале строки казанные символы.
     * @param item символы которые будут удалятся
     * @param data стока которая будет обрабатыватся
     */

    static void removeFirstAndLastChars(const QChar item,QString& data);
    QSqlQuery * getQuery();

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

