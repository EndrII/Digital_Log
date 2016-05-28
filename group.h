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
#ifndef GROUP_H
#define GROUP_H
#include "core.h"
#include <QStringList>
typedef unsigned int ui;
/**
 * @brief The savedState enum
 * Editing группа создаеться
 * noCreated упровляющая группа ещё не создана
 * noSaved группа не сохранена
 * Saved группа сохраннена
 */
enum savedState{empty,Editing,noCreated,noSaved,Saved};
class Group:public QObject
{
    Q_OBJECT
private:
    QString name;
    savedState state;

protected:
      QVector<QString> students;
public:
    explicit Group(QObject* parent=NULL);
    explicit Group(const QStringList &base, const QString name_,QObject* parent=NULL);
    friend QDataStream &operator >>(QDataStream &stream,Group& group);
    friend QDataStream &operator <<(QDataStream &stream,Group& group);
    bool operator ==(const Group&A);
    bool operator !=(const Group&A);
    Group& operator=(const Group&);
    virtual int size()const;
    QStringList getList()const;
    QString& operator[](const ui&);
    virtual QString getName()const;
    bool find(const QString&)const;
    void clearStudents();
    savedState getSavedState();
    void setSavedState(const savedState&);
    virtual void addStudent(const QString& name, bool last=true);
    virtual bool deleteStudent(const ui& index);
    virtual bool deleteStudent(const QString& name);
    virtual ~Group();
signals:
    void changed(Group*);
};

#endif // GROUP_H
