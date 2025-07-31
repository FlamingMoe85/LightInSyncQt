#include "Position.h"
#include "ui_Position.h"

#include "QDebug"

static int posInstCntr = 0;

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


    mySlider.append(ui->horizontalSlider_Position);
    mySlider.append(ui->horizontalSlider_Shift);
    mySlider.append(ui->horizontalSlider_SpanMax);
    mySlider.append(ui->horizontalSlider_SpanMin);
    mySlider.append(ui->horizontalSlider_Speed);
    mySlider.append(ui->horizontalSlider_Mul);

    int i=0;
    for(QSlider* slider : mySlider)
    {
        sliderRates[i++].mySlider = slider;
    }

    connect(ui->pushButton_PosToMiddle, SIGNAL(clicked()), this, SLOT(Slot_SetPosToMiddle()));
    oldOverride = false;

    myInst = posInstCntr++;
    ui->lcdNumber->display(myInst);
}

Position::~Position()
{
    delete ui;
}

bool Position::SupposedToSave()
{
    return ui->checkBox_Save->isChecked();
}

void Position::DoSave(bool _save)
{
    if(_save)ui->checkBox_Save->setCheckState(Qt::CheckState::Checked);
    else ui->checkBox_Save->setCheckState(Qt::CheckState::Unchecked);
}

bool Position::GetOverride()
{
    return ui->checkBox->isChecked();
}

void Position::SetOverride(bool _override)
{
    if(_override)ui->checkBox->setCheckState(Qt::CheckState::Checked);
    else ui->checkBox->setCheckState(Qt::CheckState::Unchecked);
}

void Position::Slot_SetPosToMiddle()
{
    ui->horizontalSlider_Position->setSliderPosition(ui->horizontalSlider_Position->maximum()/2);
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
    bsPtr->SetShift(sliderRates[1].GetRelExtValue((float)ui->horizontalSlider_Shift->value(), myInst));// * (float)ui->horizontalSlider_Mul->value());
    bsPtr->SetSpanMax(sliderRates[2].GetRelExtValue((float)ui->horizontalSlider_SpanMax->value(), myInst));
    bsPtr->SetSpanMin(sliderRates[3].GetRelExtValue((float)ui->horizontalSlider_SpanMin->value(), myInst));
    bsPtr->SetMaxSpeedMultiplier(sliderRates[4].GetRelExtValue((float)ui->horizontalSlider_Speed->value(), myInst));
    /*
    bsPtr->SetShift(((float)ui->horizontalSlider_Shift->value() / (float)ui->horizontalSlider_Shift->maximum()) * (float)ui->horizontalSlider_Mul->value());
    bsPtr->SetSpanMax((float)ui->horizontalSlider_SpanMax->value() / (float)ui->horizontalSlider_SpanMax->maximum());
    bsPtr->SetSpanMin((float)ui->horizontalSlider_SpanMin->value() / (float)ui->horizontalSlider_SpanMin->maximum());
    bsPtr->SetMaxSpeedMultiplier((float)ui->horizontalSlider_Speed->value());
    */
}

void Position::GetValue(float &_value)
{
    if(!ui->checkBox->isChecked())
    {
        if(oldOverride)sliderRates[0].Load(_value);
        _value = sliderRates[0].GetRelValue(_value);
    }
    else
    {
        _value = sliderRates[0].GetRelExtValue((float)ui->horizontalSlider_Position->value() / (float)ui->horizontalSlider_Position->maximum(), myInst);
    }
    oldOverride = ui->checkBox->isChecked();
    /*
    if(ui->checkBox->isChecked())
    {
        _value = (float)ui->horizontalSlider_Position->value() / (float)ui->horizontalSlider_Position->maximum();
        //_value *= (float)ui->horizontalSlider_Mul->value();
    }
    else
    {
        ui->horizontalSlider_Position->setSliderPosition(_value*(float)ui->horizontalSlider_Position->maximum());
    }
    */
}

