#ifndef MAILVIEWER_H
#define MAILVIEWER_H

#include <QWidget>
#include "subControl/viewer/mailbrowser.h"
#include "subControl/viewer/sidebar.h"
#include "subControl/viewer/mailpage.h"

#include "netProtocol/imap.h"
#include "database.h"
#include "mailsender.h"
#include "utils/type.h"
#include <QTimer>
#include <QList>

class MailViewer : public QWidget
{
    Q_OBJECT

public:
    MailViewer(DataBase *db);
    ~MailViewer();
    void setMailList(const QList<MailInfo> &mailList);
    void setDirList(const QList<DirInfo> &dirList);
    void setUserList(const QList<UserInfo> &userList);

private slots:

    void writeMail();

    void IMAPError(QString error);
    void IMAPConnected();
    void IMAPLoginFinish();
    void IMAPListFinish(QList<IMAPDir> dirs);
    void IMAPSelectFinish(QString dir,uint mails,uint recents);
    void IMAPFetchFinsh(int index,QString mail);
    void IMAPSearchFinish(QList<int> searchList);

    void showMail(int i);
    void changeDir(QString dir);
    void changeUser(QString mail);
private:
    //界面
    SideBar *sideBar;
    MailPage *mailPage;
//    QWebView *webView;
    MailBrowser *mailBrowser;

    //邮件类
    QTimer *timer;
    MailSender *mailSender;
    IMAP *imap;
    DataBase *db;

    //运行时邮件数据
    UserInfo user;
    QString dir;//当前目录，在imap.select中修改
    QList<UserInfo> userList;
    QList<DirInfo> dirList;
    QList<MailInfo> mailList;

    void initUI();
    void initConnect();
};

#endif // MAILVIEWER_H
