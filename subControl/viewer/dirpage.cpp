#include "dirpage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

DirPage::DirPage(QWidget *parent) : QWidget(parent)
{
    dirList = new DirListView;
    btAdd = new QToolButton;
    labDir = new QLabel;

    labDir->setText("目录");

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(labDir);
    hLayout->addWidget(btAdd);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addLayout(hLayout);
    vLayout->addWidget(dirList);
    setLayout(vLayout);

    connect(dirList,SIGNAL(mousePressChanged(DSimpleListItem*,int,QPoint)),
            this,SLOT(selectItem(DSimpleListItem*)));
}

void DirPage::setDirList(const QList<DirInfo> &dirList)
{
    this->dirList->setDirList(dirList);
}

void DirPage::selectItem(DSimpleListItem *item)
{
    DirItem *dirItem = static_cast<DirItem*> item;
    emit itemClick(dirItem->dirName);
}
