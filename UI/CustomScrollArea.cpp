#include "CustomScrollArea.h"
#include "ui_CustomScrollArea.h"

CustomScrollArea::CustomScrollArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomScrollArea)
{
    ui->setupUi(this);

    scrollWidget = new QWidget(this);
    vLayout = new QVBoxLayout();
    scrollWidget->setLayout(vLayout);
    ui->scrollArea->setWidget(scrollWidget);
    height = 0;
    h = 0;
}

CustomScrollArea::~CustomScrollArea()
{
    delete ui;
}

void CustomScrollArea::AddWidget(QWidget* w)
{
    scrollWidget->layout()->addWidget(w);
    if(h == 0)height += w->geometry().height();
    else height = scrollWidget->layout()->count()*h;
    scrollWidget->setFixedHeight(height);
}

void CustomScrollArea::RemoveWidget(QWidget* w)
{
    scrollWidget->layout()->removeWidget(w);
    if(h == 0)height -= w->geometry().height();
    else height = scrollWidget->layout()->count()*h;
    //height -= w->geometry().height();
    scrollWidget->setFixedHeight(height);
}

void CustomScrollArea::SetHeightOfItems(int _h)
{
    h = _h;
    height = scrollWidget->layout()->count()*h;
    scrollWidget->setFixedHeight(height);
}

void CustomScrollArea::SetWidthOfItems(int _w)
{
    scrollWidget->setFixedWidth(_w);
}
