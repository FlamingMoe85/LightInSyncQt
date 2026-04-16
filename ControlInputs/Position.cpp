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
        sliderRates[i].id = i;
        sliderRates[i++].mySlider = slider;
    }

    connect(ui->pushButton_PosToMiddle, SIGNAL(clicked()), this, SLOT(Slot_SetPosToMiddle()));
    connect(ui->pushButton_SpeedDec, SIGNAL(clicked()), this, SLOT(Slot_SpeedDec()));
    connect(ui->pushButton_SpeedInc, SIGNAL(clicked()), this, SLOT(Slot_SpeedInc()));
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

void Position::Slot_SpeedDec()
{
    int v = ui->horizontalSlider_Speed->value();
    if(v > 1) v--;
    ui->horizontalSlider_Speed->setValue(v);
}

void Position::Slot_SpeedInc()
{
    int v = ui->horizontalSlider_Speed->value();
    v++;
    if(v > ui->horizontalSlider_Speed->maximum()) v = ui->horizontalSlider_Speed->maximum();
    ui->horizontalSlider_Speed->setValue(v);
}

void Position::Ping(int itteration)
{
    if(ui->checkBox->isChecked())
    {
        int v = ui->horizontalSlider_Position->value();
        v *= (ui->horizontalSlider_Speed->value());
        if(v > ui->horizontalSlider_Position->maximum()) v=0;
        ui->horizontalSlider_Position->setSliderPosition(v);
    }
}

void Position::PingUi(BundleSeries* bsPtr)
{
    bsPtr->SetShift(sliderRates[1].GetRelExtValue((float)ui->horizontalSlider_Shift->value() / (float)ui->horizontalSlider_Shift->maximum(), myInst));// * (float)ui->horizontalSlider_Mul->value());
    bsPtr->SetSpanMax(sliderRates[2].GetRelExtValue((float)ui->horizontalSlider_SpanMax->value() / (float)ui->horizontalSlider_SpanMax->maximum(), myInst));
    bsPtr->SetSpanMin(sliderRates[3].GetRelExtValue((float)ui->horizontalSlider_SpanMin->value() / (float)ui->horizontalSlider_SpanMin->maximum(), myInst));
    bsPtr->SetMaxSpeedMultiplier(sliderRates[4].GetAbsExtValue((float)ui->horizontalSlider_Speed->value(), myInst));

    ui->lcdNumber_Speed->display(ui->horizontalSlider_Speed->value());
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
        if(oldOverride)sliderRates[0].Load(_value,200);
        _value = sliderRates[0].GetRelValue(_value);
    }
    else
    {
        _value = sliderRates[0].GetRelExtValue((float)ui->horizontalSlider_Position->value() / (float)ui->horizontalSlider_Position->maximum(), myInst*100);
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

void Position::SetFadeIn(qint64 _fadeIn)
{
    for(int i=0; i<AMT_SLIDER_RATES; i++)
    {
        sliderRates[i].SetFadeIn(_fadeIn);
    }
}

void Position::SpanMinMaxToCentreWidth(int _min, int _max, int &_centre, int &_width)
{

    if(_max >= _min)
    {
        _width = _max - _min;
        _centre = _min + ((_width)/2);
    }
    else
    {
        _width = -((1000 - _min) + _max);
        _centre = _min - ((_width)/2); //_width gonne be negativ here, so subtracting it effectivly adds
        if(_centre > 1000)
        {
            _centre -= 1000;
        }
    }
}

void Position::SpanCentreWidthToMinMax(int _centre, int _width, int &_min, int &_max)
{
    _min = _centre + (_width/2);
    _max = _centre - (_width/2);
    if(_min < 0) _min = 1000  + _min;
    if(_max > 1000) _max -= 1000;

    if(_width > 0)
    {
    }
    else
    {
    }
}

void Position::on_horizontalSlider_SpanMin_valueChanged(int value)
{
    int centre, width;
    SpanMinMaxToCentreWidth(value, ui->horizontalSlider_SpanMax->value(), centre, width);
    disconnect(ui->horizontalSlider_SpanCenter, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanCenter_valueChanged(int)));
    disconnect(ui->horizontalSlider_SpanWidth, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanWidth_valueChanged(int)));
    ui->horizontalSlider_SpanCenter->setValue(centre);
    ui->horizontalSlider_SpanWidth->setValue(width);
    connect(ui->horizontalSlider_SpanCenter, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanCenter_valueChanged(int)));
    connect(ui->horizontalSlider_SpanWidth, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanWidth_valueChanged(int)));
}


void Position::on_horizontalSlider_SpanMax_valueChanged(int value)
{
    int centre, width;
    SpanMinMaxToCentreWidth(ui->horizontalSlider_SpanMin->value(), value, centre, width);
    disconnect(ui->horizontalSlider_SpanCenter, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanCenter_valueChanged(int)));
    disconnect(ui->horizontalSlider_SpanWidth, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanWidth_valueChanged(int)));
    ui->horizontalSlider_SpanCenter->setValue(centre);
    ui->horizontalSlider_SpanWidth->setValue(width);
    connect(ui->horizontalSlider_SpanCenter, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanCenter_valueChanged(int)));
    connect(ui->horizontalSlider_SpanWidth, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanWidth_valueChanged(int)));
}


void Position::on_horizontalSlider_SpanCenter_valueChanged(int value)
{
    int min, max;
    SpanCentreWidthToMinMax(value, ui->horizontalSlider_SpanWidth->value(), min, max);
    disconnect(ui->horizontalSlider_SpanMin, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanMin_valueChanged(int)));
    disconnect(ui->horizontalSlider_SpanMax, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanMax_valueChanged(int)));
    ui->horizontalSlider_SpanMin->setValue(min);
    ui->horizontalSlider_SpanMax->setValue(max);
    connect(ui->horizontalSlider_SpanMin, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanMin_valueChanged(int)));
    connect(ui->horizontalSlider_SpanMax, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanMax_valueChanged(int)));
}


void Position::on_horizontalSlider_SpanWidth_valueChanged(int value)
{
    int min, max;
    SpanCentreWidthToMinMax(ui->horizontalSlider_SpanCenter->value(), value, min, max);
    disconnect(ui->horizontalSlider_SpanMin, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanMin_valueChanged(int)));
    disconnect(ui->horizontalSlider_SpanMax, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanMax_valueChanged(int)));
    ui->horizontalSlider_SpanMin->setValue(min);
    ui->horizontalSlider_SpanMax->setValue(max);
    connect(ui->horizontalSlider_SpanMin, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanMin_valueChanged(int)));
    connect(ui->horizontalSlider_SpanMax, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_SpanMax_valueChanged(int)));
}

