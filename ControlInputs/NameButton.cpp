#include "NameButton.h"
#include "ui_NameButton.h"

NameButton::NameButton(QWidget *parent, QString _name) :
    QWidget(parent),
    ui(new Ui::NameButton)
{
    ui->setupUi(this);
    ui->pushButton->setText(_name);
    connect(ui->pushButton, &QPushButton::clicked, this, &NameButton::Slot_Clicked);
}

NameButton::~NameButton()
{
    delete ui;
}

void NameButton::Slot_Clicked()
{
    static QString emitName;

    emitName = ui->pushButton->text();
    emit Sig_SendName(emitName);
}
