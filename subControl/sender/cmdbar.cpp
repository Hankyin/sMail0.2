#include "cmdbar.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QIcon>
#include <QMenu>
#include <QAction>

CmdBar::CmdBar(QWidget *parent) : QWidget(parent)
{
    btSend = new QPushButton("发送邮件");
    btSend->setIcon(QIcon(":/icon/image/send.png"));
    btSend->setStyleSheet("QPushButton "
                          "{"
                          " border: 2px solid #8f8f91;"
                          " border-radius: 6px;"
                          " background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                          "                             stop: 0 #f6f7fa, stop: 1 #dadbde);"
                          " min-width: 80px;"
                          "}"
                          ""
                          " QPushButton:pressed"
                          "{"
                          " background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                          "                             stop: 0 #dadbde, stop: 1 #f6f7fa);"
                          "}"
                          ""
                          "QPushButton:flat "
                          "{"
                          " border: none;"
                          "}"
                          ""
                          "QPushButton:default "
                          "{"
                          " border-color: navy; "
                          "}");
    btTranslate = new QToolButton();
    btTranslate->setIcon(QIcon(":/icon/image/translate.png"));
    btTranslate->setAutoRaise(true);
    btTranslate->setToolTip("翻译邮件内容");
    btEmoji = new QToolButton();
    btEmoji->setIcon(QIcon(":/icon/image/emoji.png"));
    btEmoji->setAutoRaise(true);
    btEmoji->setToolTip("插入表情");
    btAttachment = new QToolButton();
    btAttachment->setIcon(QIcon(":/icon/image/attachment.png"));
    btAttachment->setAutoRaise(true);
    btAttachment->setToolTip("插入附件");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(btSend);
    layout->addWidget(btTranslate);
    layout->addWidget(btEmoji);
    layout->addStretch();
    layout->addWidget((btAttachment));
    this->setLayout(layout);

    connect(this->btSend,SIGNAL(clicked(bool)),this,SIGNAL(sendMailClick()));
    connect(this->btEmoji,SIGNAL(clicked(bool)),this,SIGNAL(emojiClick()));
    connect(this->btAttachment,SIGNAL(clicked(bool)),this,SIGNAL(attachmentClick()));
}

void CmdBar::setTranslateSupportLang(const QStringList &list)
{
    QMenu *translateMenu = new QMenu;
    for(auto i : list)
    {
        QAction *act = new QAction(translateMenu);
        act->setText(i);
        translateMenu->addAction(act);
        connect(act,SIGNAL(triggered(bool)),this,SLOT(selectTranslateLanguage()));
    }
    this->btTranslate->setMenu(translateMenu);
    this->btTranslate->setPopupMode(QToolButton::InstantPopup);
}

void CmdBar::selectTranslateLanguage()
{
    QAction *act = qobject_cast<QAction*>(QObject::sender());//使用sender()获取发送消息的action
    emit translateClick(act->text());
}

