#ifndef IMAP_H
#define IMAP_H

#include <QObject>
#include <QSslSocket>
#include <QTcpSocket>
#include <QList>
#include <QMap>
#include <QString>
#include <QRegularExpression>
#include "utils/type.h"

#define IMAP4_PORT 143
#define IMAP4_SSL_PORT 993
#define MAX_LINE 100000

class IMAPResp
{
public:
    IMAPResp() {}
    QString tag;
    QString type;
    QString data;
};



class IMAP : public QObject
{
    Q_OBJECT
    typedef void (IMAP::*Handler) (const IMAPResp &resp);//服务器响应处理函数指针
public:
    explicit IMAP(QObject *parent = nullptr);
    void connectServer(const QString hostName = QString(),
                       quint16 port = IMAP4_PORT, bool ssl = false);
    void setDebug(bool value);

    //IMAP 命令
    //任意状态命令
    void capability();
    void noop();
    void logout();
    //未认证状态 NONAUTH
    void login(const QString &user, const QString &pass);
    //认证状态  AUTH
    void list(const QString &dir = QString(), const QString &pattern = "*");
    void select(const QString &mailBox);
    void fetch(const QString &mailSet, const QString &mailPart);
    void search(const QString &criteria);
signals:
    void IMAPError(QString error);
    void stateChanged(QString curState);
    void connected();
    void loginFinish();
    void listFinish(QList<IMAPDir> imapDirs);
    void selectFinish(QString dir,uint mails,uint recents);
    void fetchFinish(int mailindex,QString mail);
    void searchFinish(QList<int> searchList);
public slots:

private slots:
    void socketEncrypted();
    void socketConnected();
    void socketError(QAbstractSocket::SocketError error);
    void sslErrors(const QList<QSslError> &errors);
    void socketStateChanged(QAbstractSocket::SocketState);
    void socketReadyRead();
private:
    QMap<QString,QStringList> CommandList = {
        {"APPEND",{"AUTH", "SELECTED"}},
        {"AUTHENTICATE",{"NONAUTH"}},
        {"CAPABILITY",{"NONAUTH", "AUTH", "SELECTED", "LOGOUT"}},
        {"CHECK",{"SELECTED",}},
        {"CLOSE",{"SELECTED",}},
        {"COPY",{"SELECTED",}},
        {"CREATE",{"AUTH", "SELECTED"}},
        {"DELETE",{"AUTH", "SELECTED"}},
        {"DELETEACL",{"AUTH", "SELECTED"}},
        {"ENABLE",{"AUTH", }},
        {"EXAMINE",{"AUTH", "SELECTED"}},
        {"EXPUNGE",{"SELECTED",}},
        {"FETCH",{"SELECTED",}},
        {"GETACL",{"AUTH", "SELECTED"}},
        {"GETANNOTATION",{"AUTH", "SELECTED"}},
        {"GETQUOTA",{"AUTH", "SELECTED"}},
        {"GETQUOTAROOT",{"AUTH", "SELECTED"}},
        {"MYRIGHTS",{"AUTH", "SELECTED"}},
        {"LIST",{"AUTH", "SELECTED"}},
        {"LOGIN",{"NONAUTH",}},
        {"LOGOUT",{"NONAUTH", "AUTH", "SELECTED", "LOGOUT"}},
        {"LSUB",{"AUTH", "SELECTED"}},
        {"NAMESPACE",{"AUTH", "SELECTED"}},
        {"NOOP",{"NONAUTH", "AUTH", "SELECTED", "LOGOUT"}},
        {"PARTIAL",{"SELECTED",}},
        {"PROXYAUTH",{"AUTH",}},
        {"RENAME",{"AUTH", "SELECTED"}},
        {"SEARCH",{"SELECTED",}},
        {"SELECT",{"AUTH", "SELECTED"}},
        {"SETACL",{"AUTH", "SELECTED"}},
        {"SETANNOTATION",{"AUTH", "SELECTED"}},
        {"SETQUOTA",{"AUTH", "SELECTED"}},
        {"SORT",{"SELECTED",}},
        {"STARTTLS",{"NONAUTH",}},
        {"STATUS",{"AUTH", "SELECTED"}},
        {"STORE",{"SELECTED",}},
        {"SUBSCRIBE",{"AUTH", "SELECTED"}},
        {"THREAD",{"SELECTED",}},
        {"UID",{"SELECTED",}},
        {"UNSUBSCRIBE",{"AUTH", "SELECTED"}},
    };
    QRegularExpression taggedRE;
    QRegularExpression untaggedRE;
    QRegularExpression untaggedNumRE;
    QRegularExpression responseCodeRE;

    QSslSocket *sslSocket;
    QTcpSocket *tcpSocket;
    QByteArray remains;

    bool debug = false;
    bool ssl = false;
    bool hasGreeting = false;
    bool hasConnected = false;//第一次执行capability调用一下
    int tagNum = 0;
    QString tag;
    QString state;
    QMap<QString,QString> taggedCommand;//发送的命令的tag
    QMap<QString,QString> untaggedResponse;//存储非标签回复的数据
    QMap<QString,Handler> respHanderList;
    QList<IMAPDir> dirList;//imap目录列表
    QString curDir;
    uint mailCount;
    uint recentMailCount;
    QString literalBuf;//服务器传输大量文本（比如邮件信息）时的缓存.
    int fetchMailIndex;
    bool startLiteral = false;
    int literalSize;
    QList<int> searchList;

    void sendCommand(const QString &command, const QString &arg);
    void respHandler(const IMAPResp &resp);
    void addRespHandler(const QString &command, const Handler handler);
    QString int2AP(int num);
    QString newTag();
    IMAPResp respPraser(const QString &resp);
    void printMsg(const QString &msg);
    void printSendMsg(const QString &msg);
    void printRecvMsg(const QString &msg);
    void stateChangedProcess();

    //服务器响应
    //状态响应
    void okHandler(const IMAPResp &resp);
    void badHandler(const IMAPResp &resp);
    void noHandler(const IMAPResp &resp);
//    void byeHandler(const IMAPResp &resp);
    //服务器和邮箱响应
    void capabilityHandler(const IMAPResp &resp);
    void listHandler(const IMAPResp &resp);
    //邮箱大小
    void recentHandler(const IMAPResp &resp);
    void existsHandler(const IMAPResp &resp);
    //邮件状态
//    void expungeHandler(const IMAPResp &resp);
    void fetchHandler(const IMAPResp &resp);
    void searchHandler(const IMAPResp &resp);
};

#endif // IMAP_H
