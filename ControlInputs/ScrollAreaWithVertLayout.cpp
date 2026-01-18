#include "ScrollAreaWithVertLayout.h"
#include "ui_ScrollAreaWithVertLayout.h"

ScrollAreaWithVertLayout::ScrollAreaWithVertLayout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScrollAreaWithVertLayout)
{
    ui->setupUi(this);
    name = "no name";
}

ScrollAreaWithVertLayout::~ScrollAreaWithVertLayout()
{
    delete ui;
}

void ScrollAreaWithVertLayout::AddToScrollArea(QWidget* _widget)
{
    ui->scrollAreaWidgetContents->layout()->addWidget(_widget);
}
