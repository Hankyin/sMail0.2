#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include "utils/type.h"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    QList<UserInfo> getUserList();
    QList<DirInfo> getDirList(const QString &mail);
    QList<MailInfo> getMailList(const QString &mail,const QString &dir);
    void insertNewUser(const UserInfo &newUser);
    void insertNewDir(const DirInfo &newDir);
    void insertNewMail(int index, const QString &mail);
signals:
    void error();
public slots:

private:
    QSqlDatabase db;
    QString lastErrorString;
};

#endif // DATABASE_H
