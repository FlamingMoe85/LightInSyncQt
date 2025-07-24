#include "Position.h"
#include "ui_Position.h"

#include "QDebug"


Position::Position(QWidget *parent, PositionInit_t _init) :
    QWidget(parent),
    ui(new Ui::Position)
{
    ui->setupUi(this);
    ui->title->setText(_init.name);
    ui->checkBox->setCheckState(_init.overridePos);
    ui->horizontalSlider_Shift->setEnabled(_init.enableShift);
    ui->horizontalSlider_SpanMax->setEnabled(_init.enableSpan);
    ui->horizontalSlider_SpanMin->setEnabled(_init.enableSpan);
    ui->horizontalSlider_Speed->setEnabled(_init.enableSpeed);
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
}

void Position::PingUi(BundleSeries* bsPtr)
{
    bsPtr->SetShift((float)ui->horizontalSlider_Shift->value() / (float)ui->horizontalSlider_Shift->maximum());
    bsPtr->SetSpanMax((float)ui->horizontalSlider_SpanMax->value() / (float)ui->horizontalSlider_SpanMax->maximum());
    bsPtr->SetSpanMin((float)ui->horizontalSlider_SpanMin->value() / (float)ui->horizontalSlider_SpanMin->maximum());
    bsPtr->SetMaxSpeedMultiplier((float)ui->horizontalSlider_Speed->value());
}

void Position::GetValue(float &_value)
{
    if(ui->checkBox->isChecked())
    {
        _value = (float)ui->horizontalSlider_Position->value() / (float)ui->horizontalSlider_Position->maximum();
        _value *= (float)ui->horizontalSlider_Mul->value();
    }
    else
    {
        ui->horizontalSlider_Position->setSliderPosition(_value*(float)ui->horizontalSlider_Position->maximum());
    }
}

