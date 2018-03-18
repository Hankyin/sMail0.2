#include "mailsender.h"
#include <QVBoxLayout>
#include <QStringList>
#include <QStringListModel>
#include <QCompleter>
#include <QPushButton>
#include <QFileDialog>//获取附件
#include <QDir>
#include "utils/translator.h"

MailSender::MailSender(QWidget *parent)
    : QWidget(parent)
{
    from = new LineEdit(tr("发件人:"));
    to = new LineEdit(tr("收件人:"),true);
    subject = new LineEdit(tr("主题： "));
    from->setReadOnly(true);
    from->setFocusPolicy(Qt::NoFocus);
    QStringListModel *model = new QStringListModel;
    QStringList list;
    list<<"yin <loufand@163.com>"<<"微软 <store@microsoft.com"<<"loufand@outlook.com";
    model->setStringList(list);
    QCompleter *toCompleter = new QCompleter(model);
    toCompleter->setFilterMode(Qt::MatchContains);
    to->setComplater(toCompleter);
    to->setFocus();
    editor = new MailEditor;
    translator = new Translator(this);
    cmdBar = new CmdBar;
    emojiPicker = nullptr;
    cmdBar->setTranslateSupportLang(translator->getLanguageList());
    QVBoxLayout *contentLayout = new QVBoxLayout();
    contentLayout->addWidget(from);
    contentLayout->addWidget(to);
    contentLayout->addWidget(subject);
    contentLayout->addWidget(editor);
    contentLayout->addWidget(cmdBar);
    this->setLayout(contentLayout);
    this->resize(400,500);

    connect(this->cmdBar,SIGNAL(attachmentClick()),this,SLOT(addAttachment()));
    connect(this->cmdBar,SIGNAL(translateClick(QString)),this,SLOT(translate(QString)));
    connect(this->cmdBar,SIGNAL(emojiClick()),this,SLOT(showEmojiPicker()));
    connect(this->translator,SIGNAL(translateResult(QString)),this,SLOT(setTranslation(QString)));
}

MailSender::~MailSender()
{

}

void MailSender::addAttachment()
{
    QString attachmentPath = QFileDialog::getOpenFileName(this,"选择附件",QDir::homePath());
    if(attachmentPath.isEmpty())
        return;
    this->editor->addItem(attachmentPath);

}

void MailSender::translate(const QString language)
{
    translator->requireTranslate(this->editor->toPlainText(),language);
}

void MailSender::setTranslation(const QString result)
{
    if(result.isEmpty())
        return;
    this->editor->setText(result);
}

void MailSender::showEmojiPicker()
{
    if(emojiPicker == nullptr)
    {
        emojiPicker = new EmojiPicker(this);
    }
    emojiPicker->show();

}
