#include "Position.h"
#include "ui_Position.h"


Position::Position(QWidget *parent, ClientServerBase* client, QString name) :
    QWidget(parent),
    ui(new Ui::Position)
{
    ui->setupUi(this);
    ui->title->setText(name);
    QObject::connect(&ctPosition, &ClientServer_Top::RequestValue, this, &Position::Slot_GetValue);
    if(client != nullptr)ctPosition.RegisterCLient(client);
}

Position::~Position()
{
    delete ui;
}

void Position::Ping(int itteration)
{
    ctPosition.Request(itteration);
}

void Position::Slot_GetValue(ClientServer_Top *b, int itterration)
{
    float tmpF = (float)ui->horizontalSlider_Position->value() / (float)ui->horizontalSlider_Position->maximum();

    if(ui->checkBox->isChecked())
    {
        int v = ui->horizontalSlider_Position->value();
        v += (ui->horizontalSlider_Speed->value());
        if(v > ui->horizontalSlider_Position->maximum()) v=0;
        ui->horizontalSlider_Position->setSliderPosition(v);
    }

    b->Serve(itterration,tmpF);
}
