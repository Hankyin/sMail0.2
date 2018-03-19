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
    QList<MailInfo> getMailList(const QString &mailbox, const QString &dir);
    QByteArray getMailByID(int index, const QString &mailbox);
    void insertNewUser(const UserInfo &newUser);
    void insertNewMail(const MailInfo &mailinfo, const QString &mail);
signals:
    void error();
public slots:

private:
    QSqlDatabase db;
    QString lastErrorString;
};

#endif // DATABASE_H
