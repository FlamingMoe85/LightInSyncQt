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
}

CustomScrollArea::~CustomScrollArea()
{
    delete ui;
}

void CustomScrollArea::AddWidget(QWidget* w)
{
    scrollWidget->layout()->addWidget(w);
    height += w->geometry().height();
    scrollWidget->setFixedHeight(height);
}

void CustomScrollArea::RemoveWidget(QWidget* w)
{
    scrollWidget->layout()->removeWidget(w);
    height -= w->geometry().height();
    scrollWidget->setFixedHeight(height);
}

