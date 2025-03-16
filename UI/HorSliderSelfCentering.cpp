#include "HorSliderSelfCentering.h"
#include "ui_HorSliderSelfCentering.h"

HorSliderSelfCentering::HorSliderSelfCentering(QWidget *parent, int _min, int _max) :
    QWidget(parent),
    ui(new Ui::HorSliderSelfCentering)
{
    ui->setupUi(this);
    ui->horizontalSlider->setMinimum(_min);
    ui->horizontalSlider->setMaximum(_max);

    connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(Slot_ValueChanged(int)));
    connect(ui->horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(Slot_HandleReleased()));
    QObject::connect(&timer, &QTimer::timeout, this, &HorSliderSelfCentering::Slot_TimerExpired);
    timer.setInterval(100);
}

HorSliderSelfCentering::~HorSliderSelfCentering()
{
    delete ui;
}

void HorSliderSelfCentering::Slot_ValueChanged(int _pos)
{
    timer.start();
    posAbs = _pos;
    posRel = (float)posAbs / (float)(ui->horizontalSlider->maximum()-ui->horizontalSlider->minimum());
    ValueChangedAbs(posAbs);
    ValueChangedRel(posRel);
}

void HorSliderSelfCentering::Slot_HandleReleased()
{
    timer.stop();
    ui->horizontalSlider->setValue(ui->horizontalSlider->minimum() + ((ui->horizontalSlider->maximum()-ui->horizontalSlider->minimum())/2));
}
void HorSliderSelfCentering::Slot_TimerExpired()
{
    ValueChangedAbs(posAbs);
    ValueChangedRel(posRel);
}
