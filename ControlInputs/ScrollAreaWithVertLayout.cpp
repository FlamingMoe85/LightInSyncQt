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

void ScrollAreaWithVertLayout::RemoveFromScrollArea(QWidget* _widget)
{
    ui->scrollAreaWidgetContents->layout()->removeWidget(_widget);
    _widget->hide();

}


void ScrollAreaWithVertLayout::RemoveAllWidgetsFromLayout()
{
   while (ui->scrollAreaWidgetContents->layout()->itemAt(0) != 0)
   {
       ui->scrollAreaWidgetContents->layout()->takeAt(0);
   }
}
