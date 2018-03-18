#include <QVBoxLayout>
#include <QFileInfo>//用于保存附件地址
#include <QDateTime>//用于生成附件uid
#include <QIcon>
#include "maileditor.h"

MailEditor::MailEditor(QWidget *parent) : QScrollArea(parent)
{
    textEdit = new QTextEdit;
    attachmentArea = new QWidget;
    bgArea = new QWidget;
    textEdit->setFrameStyle(QFrame::NoFrame);
    textEdit->setStyleSheet("background:transparent;"
                            "border-width:0;"
                            "border-style:outset;"
                            "font: 10pt");
    attachmentLayout = new FlowLayout;
    attachmentArea->setLayout(attachmentLayout);
    QVBoxLayout *bgLayout = new QVBoxLayout(bgArea);
    bgLayout->addWidget(textEdit);
    bgLayout->addWidget(attachmentArea,1);
    bgLayout->setMargin(0);
    bgLayout->setSpacing(0);
    bgArea->setLayout(bgLayout);

    this->setWidgetResizable(true);
    this->setWidget(bgArea);
    this->setFrameStyle(QFrame::NoFrame);

    connect(this->textEdit,SIGNAL(textChanged()),this,SLOT(resizeTextEditHeight()));
}

void MailEditor::addItem(QString &attachmentPath)
{
    QFileInfo fileInfo(attachmentPath);
    QString fileName = fileInfo.fileName();
    QString suffix = fileInfo.suffix().toLower();
    QIcon itemIcon;
    if(suffix == "png" || suffix == "jpg" || suffix == "jpeg")
    {
        itemIcon.addFile(":/icon/image/img.png");
    }
    else
    {
        itemIcon.addFile(":/icon/image/file.png");
    }
    uint newID = (QDateTime::currentDateTime().toTime_t() << 2) + attachmentList.size();
    ItemButton *item = new ItemButton(newID,fileName);
    item->setIcon(itemIcon);
    attachmentList.append(item);
    attachmentLayout->addWidget(item);
    connect(item,SIGNAL(remove(uint)),this,SLOT(delItem(uint)));
}

void MailEditor::delItem(uint id)
{
    ItemButton *item = Q_NULLPTR;
    for(auto i : attachmentList)
    {
        if(i->getID() == id)
        {
            item = i;
        }
    }
    if(!item)
        return;

    attachmentLayout->removeWidget(item);
    attachmentList.removeOne(item);
    delete item;
}

void MailEditor::resizeTextEditHeight()
{
    QTextDocument *doc = this->textEdit->document();
    if(doc)
    {
        int docHeight = doc->size().height();
        this->textEdit->setMinimumHeight(docHeight + 10);
    }
}
