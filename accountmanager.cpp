#include "accountmanager.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QSpinBox>
#include <QCheckBox>

AccountManager::AccountManager(sMail *smail) : QStackedWidget(nullptr)
{
    this->smail = smail;
    userInfoPage = new QWidget;
    serverInfoPage = new QWidget;

    labSetMail = new QLabel("请设置您的账户");
    leName = new QLineEdit;
    leMail = new QLineEdit;
    lePass = new QLineEdit;
    btNext = new QPushButton;
    leName->setPlaceholderText("姓名");
    leMail->setPlaceholderText("邮箱");
    lePass->setPlaceholderText("密码");
    lePass->setEchoMode(QLineEdit::Password);
    btNext->setText("下一步");
    QVBoxLayout *uLayout = new QVBoxLayout;
    uLayout->addWidget(labSetMail);
    uLayout->addWidget(leName);
    uLayout->addWidget(leMail);
    uLayout->addWidget(lePass);
    QHBoxLayout *ubLayout = new QHBoxLayout;
    ubLayout->addStretch();
    ubLayout->addWidget(btNext);
    uLayout->addLayout(ubLayout);
    userInfoPage->setLayout(uLayout);

    labSetServer = new QLabel("请设置您的邮箱服务器信息");
    boxIMAP = new QGroupBox;
    leIMAPServer = new QLineEdit;
    spIMAPPort = new QSpinBox;
    chIMAPSSL = new QCheckBox;
    boxSMTP = new QGroupBox;
    leSMTPServer = new QLineEdit;
    spSMTPPort = new QSpinBox;
    chSMTPSSL = new QCheckBox;
    btPrevious = new QPushButton;
    btFinish = new QPushButton;
    boxIMAP->setTitle("IMAP设置");
    leIMAPServer->setPlaceholderText("IMAP服务器地址");
    spIMAPPort->setMaximum(65535);
    spIMAPPort->setMinimum(0);
    spIMAPPort->setValue(993);
    chIMAPSSL->setText("加密传输");
    chIMAPSSL->setChecked(true);
    boxSMTP->setTitle("SMTP设置");
    leSMTPServer->setPlaceholderText("SMTP服务器地址");
    spSMTPPort->setMaximum(65535);
    spSMTPPort->setMinimum(0);
    spSMTPPort->setValue(465);
    chSMTPSSL->setText("加密传输");
    chSMTPSSL->setChecked(true);
    btPrevious->setText("上一步");
    btFinish->setText("设置完成");
    QVBoxLayout *sLayout = new QVBoxLayout;
    sLayout->addWidget(labSetServer);
    QVBoxLayout *siLayout = new QVBoxLayout;
    siLayout->addWidget(leIMAPServer);
    siLayout->addWidget(spIMAPPort);
    siLayout->addWidget(chIMAPSSL);
    boxIMAP->setLayout(siLayout);
    QVBoxLayout *ssLayout = new QVBoxLayout;
    ssLayout->addWidget(leSMTPServer);
    ssLayout->addWidget(spSMTPPort);
    ssLayout->addWidget(chSMTPSSL);
    boxSMTP->setLayout(ssLayout);
    QHBoxLayout *glayout = new QHBoxLayout;
    glayout->addWidget(boxIMAP);
    glayout->addWidget(boxSMTP);
    sLayout->addLayout(glayout);
    QHBoxLayout *sbLayout = new QHBoxLayout;
    sbLayout->addWidget(btPrevious);
    sbLayout->addWidget(btFinish);
    sLayout->addLayout(sbLayout);
    serverInfoPage->setLayout(sLayout);

    addWidget(userInfoPage);
    addWidget(serverInfoPage);

    connect(btNext,SIGNAL(clicked(bool)),this,SLOT(toServerPage()));
    connect(btPrevious,SIGNAL(clicked(bool)),this, SLOT(toUserPage()));
    connect(btFinish,SIGNAL(clicked(bool)),this,SLOT(finish()));
}

void AccountManager::toUserPage()
{
    setCurrentIndex(0);
}

void AccountManager::toServerPage()
{
    setCurrentIndex(1);
}

void AccountManager::finish()
{
    user.name = leName->text();
    user.mail = leMail->text();
    user.pass = lePass->text();
    user.IMAPServer = leIMAPServer->text();
    user.IMAPPort = spIMAPPort->value();
    user.isIMAPSSL = chIMAPSSL->isChecked();
    user.SMTPServer = leSMTPServer->text();
    user.SMTPPort = spSMTPPort->value();
    user.isSMTPSSL = chSMTPSSL->isChecked();
    emit userInfoReady(user);
    this->hide();
}
