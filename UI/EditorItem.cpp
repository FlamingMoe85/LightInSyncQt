#include "EditorItem.h"
#include "ui_EditorItem.h"

EditorItem::EditorItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorItem)
{
    ui->setupUi(this);
}

EditorItem::~EditorItem()
{
    delete ui;
}

void EditorItem::SetWidget(QWidget* w)
{
    ui->verticalLayout_ShadeSpace->addWidget(w);
}

void EditorItem::SetTitle(QString _title)
{
    ui->label_Titel->setText(_title);
}
