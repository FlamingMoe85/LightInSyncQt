#include "Position.h"
#include "ui_Position.h"


Position::Position(QWidget *parent, I_Client* client, QString name, bool _defaultCheck) :
    QWidget(parent),
    ui(new Ui::Position)
{
    ui->setupUi(this);
    ui->title->setText(name);
    if(_defaultCheck)ui->checkBox->setCheckState(Qt::CheckState::Checked);
    QObject::connect(&ctPosition, &ClientServer_Top::RequestValue, this, &Position::Slot_GetValue);
    if(client != nullptr)ctPosition.RegisterCLient(client);
}

Position::~Position()
{
    delete ui;
}

void Position::Ping(int itteration)
{
    if(ui->checkBox->isChecked())
    {
        int v = ui->horizontalSlider_Position->value();
        v += (ui->horizontalSlider_Speed->value());
        if(v > ui->horizontalSlider_Position->maximum()) v=0;
        ui->horizontalSlider_Position->setSliderPosition(v);
    }
    //ctPosition.Request(itteration);
}

bool Position::GetValue(float &_value)
{
    if(ui->checkBox->isChecked())
    {
        _value = (float)ui->horizontalSlider_Position->value() / (float)ui->horizontalSlider_Position->maximum();
        return true;
    }
    else return false;
}

void Position::SetValue(float _value)
{
    if(!ui->checkBox->isChecked())
    ui->horizontalSlider_Position->setSliderPosition(ui->horizontalSlider_Position->maximum()*_value);
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
