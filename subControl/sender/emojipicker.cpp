#include "emojipicker.h"
#include "utils/flowlayout.h"
#include <QToolButton>
#include <QScrollArea>

EmojiPicker::EmojiPicker(QWidget *parent) : QTabWidget(parent)
{
    addEmojiSection(0x1F600,0x1F64F,"Emoji");
}

void EmojiPicker::addEmojiSection(uint unicodeStart, uint unicodeEnd,const QString &label)
{
    QWidget* page = new QWidget;
    QScrollArea *scrollArea = new QScrollArea;
    FlowLayout *pageLayoyt = new FlowLayout(page);
    for(uint i = unicodeStart; i <=unicodeEnd; i++)
    {
        QToolButton *b = new QToolButton;
        b->setText(getEmoji(i));
        connect(b,SIGNAL(clicked(bool)),this,SLOT(emojiClick()));
        pageLayoyt->addWidget(b);
    }
    pageLayoyt->setSpacing(0);
//    page->setFixedWidth(30);//一行放10个按钮
    scrollArea->setWidget(page);
    scrollArea->setWidgetResizable(true);
    this->addTab(scrollArea,label);
}

void EmojiPicker::emojiClick()
{
    QToolButton *bt = qobject_cast<QToolButton*>(QObject::sender());
    emit pickEmoji(bt->text());
}

QString EmojiPicker::getEmoji(uint unicode)
{
    unicode -= 0x10000;
    uint codeH = (unicode >> 10) + 0xD800;
    uint codeL = (unicode & 0x3FF) + 0xDC00;
    QChar pairH(codeH);
    QChar pairL(codeL);
    QString emoji;
    emoji.append(pairH);
    emoji.append(pairL);
    return emoji;
}
