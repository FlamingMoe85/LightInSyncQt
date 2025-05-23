#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent)
    :   QWidget(parent),
        ui(new Ui::Widget)
{
    ui->setupUi(this);

    //ui->verticalLayout_Player->addWidget(&audioPlayerFrontEnd);

    for(int i=0; i<UNIV_LENGTH; i++)
    {
        universum.push_back(&(buf[i]));
    }



    bundleSeriesDevice.GenerateBundleSeries(AMT_DEVICES);
    bsmDimm.GenerateBundleSeries(AMT_DEVICES);
    bsmWheel.GenerateBundleSeries(AMT_DEVICES);
    bsmWheel.SetSerParamSpanMinToItems(&spanMinTop);
    bsmWheel.SetSerParamSpanMaxToItems(&spanMaxTop);
    bsmWheel.SetSerParamSpanOffsetToItems(&spanOffsetTop);
    bsmWheel.SetSerParamSpeedToItems(&shiftSpeed);

    RGBW_16B_Dimm_Init_t rgbw16B_Init;
    rgbw16B_Init.red = 0;
    rgbw16B_Init.green = 2;
    rgbw16B_Init.blue = 4;
    rgbw16B_Init.white = 6;

    cT.RegisterClient(&bsmMaster);
    bsmMaster.GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
    bsmOfSections.GenerateBundleSeries(AMT_SECTIONS);
    for(int section=0; section<AMT_SECTIONS; section++)
    {
        //
        bsmSection[section].GenerateBundleSeries(AMT_DEVS_PER_SECTION);
        for(int device=0; device<AMT_DEVS_PER_SECTION; device++)
        {
            int indexAbs = (section*AMT_DEVS_PER_SECTION)+device;

            // create devices
            rgbw16Bdevices[indexAbs] = new RGBW_16B_Dimm(universum);
            rgbw16B_Init.adr = 1+(indexAbs*CHNLS_PER_CAN);
            rgbw16Bdevices[indexAbs]->Init(rgbw16B_Init);
            rgbw16Bdevices[indexAbs]->SetDimmChnlVal(255);

            // build Level 0
            colWheel[indexAbs].SetRgbDevice((rgbw16Bdevices[indexAbs]));
            colWheel[indexAbs].GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
            bsmSection[section].RegisterClientToItem(device, &(colWheel[indexAbs]));
            bsmSection[section].GetBundleSeries(device)->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);

            // build Level 1
            bsmOfSections.RegisterClientToItem(section, (bsmSection[section].GetBundleSeries(device)));
        }

        //build Level 2
        bsmMaster.RegisterClient(bsmOfSections.GetBundleSeries(section));
        bsmOfSections.GetBundleSeries(section)->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
    }

    serial.setPortName("COM5");
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.open(QIODevice::ReadWrite);

    /*
    QObject::connect(&shiftTop, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetShift);
    QObject::connect(&spanMinTop, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanMin);
    QObject::connect(&spanMaxTop, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanMax);
    QObject::connect(&spanOffsetTop, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanOffset);
    QObject::connect(&shiftSpeed, &ClientServer_Top::RequestValue, this, &Widget::Slot_ShiftSpeed);
    */

    QObject::connect(&cT, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetValue);
    QObject::connect(&timer, &QTimer::timeout, this, &Widget::Slot_TimerExpired);



    timer.setInterval(100);
    timer.start();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::Slot_SendMsg()
{
    static QString sendMsg;

    sendMsg.clear();
    sendMsg.append('0'-48);

    //serial.write(sendMsg.toLatin1());

}

void Widget::Slot_TimerExpired()
{
    itteration++;
    float dimm = (float)ui->horizontalSlider_rgbDimm->value() / (float)ui->horizontalSlider_rgbDimm->maximum();
    for(int i=0; i<AMT_SECTIONS*AMT_DEVS_PER_SECTION; i++)
    {
        rgbw16Bdevices[i]->SetDimmChnlVal((uint8_t)(255.0*dimm));
    }
    colWheel[0].GetRequested(itteration);
    /*
    for(int m=0; m<AMT_DEVICES; m++)
    {
        colWheel[m].GetRequested(itteration);
    }
    */

    /*
    if(ui->checkBox->isChecked())
    {
        int v = ui->horizontalSlider->value();
        v -= v%(ui->horizontalSlider_Speed->value());
        v += (ui->horizontalSlider_Speed->value());
        if(v > ui->horizontalSlider->maximum()) v=0;
        ui->horizontalSlider->setSliderPosition(v);
    }
    if(ui->checkBox_Shift->isChecked())
    {
        if(dir == 0)
        {

           int v = ui->horizontalSlider_2->value();
                v -= v%4;
                v += 4;
                if(v > ui->horizontalSlider_2->maximum()) dir = 1;
                ui->horizontalSlider_2->setSliderPosition(v);
        }
        else
        {
           int v = ui->horizontalSlider_2->value();
                    v += v%2;
                    v -= 2;
                    if(v == 0) dir = 0;
                    ui->horizontalSlider_2->setSliderPosition(v);
        }
    }

    if(ui->checkBox_Offset->isChecked())
    {
        int v = ui->horizontalSlider_SpanOffset->value();
        v += 1;
        if(v > ui->horizontalSlider->maximum()) v=0;
        ui->horizontalSlider_SpanOffset->setSliderPosition(v);
    }
*/

    static QString sendMsg, debugMsg;
    debugMsg.clear();
    sendMsg.clear();
    for(uint8_t* v : universum)
    {
        sendMsg.append((*v));
        debugMsg += " " + QString::number(*v);
        *v = 0;
    }
    qDebug() << debugMsg.toLatin1();
    //qDebug() << sendMsg.toLatin1();
    if(serial.isOpen())
    {
      serial.write(sendMsg.toLatin1());
    }
    //
}


void Widget::Slot_GetValue(ClientServer_Top *b, int itterration)
{
    float tmpF = (float)ui->horizontalSlider_MainPosition->value() / (float)ui->horizontalSlider_MainPosition->maximum();
    b->Serve(itterration,tmpF);
}
/*
void Widget::Slot_GetShift(ClientServer_Top *b, int itterration)
{
    float tmpF = (float)ui->horizontalSlider_2->value() / (float)ui->horizontalSlider_2->maximum();
    b->Serve(itterration,tmpF);
}
void Widget::Slot_GetSpanMin(ClientServer_Top *b, int itterration)
{
    float m, n;
    m = GetSpanMin();
    n = GetSpanMax();
    b->Serve(itterration,m);
    ui->label_SpanMin->setText(QString::number(m));
    ui->label_SpanDiff->setText(QString::number(n-m));
}

void Widget::Slot_GetSpanMax(ClientServer_Top *b, int itterration)
{
    float m, n;
    m = GetSpanMin();
    n = GetSpanMax();
    b->Serve(itterration,n);
    ui->label_SpanMax->setText(QString::number(n));
    ui->label_SpanDiff->setText(QString::number(n-m));
}

void Widget::Slot_GetSpanOffset(ClientServer_Top *b, int itterration)
{
    float o = GetSpanOffset();
    b->Serve(itterration, o);
    ui->label_SpanOffset->setText(QString::number(o));
}

void Widget::Slot_ShiftSpeed(ClientServer_Top *b, int itterration)
{
    float f = ((float)ui->horizontalSlider_ShiftSpeed->value() / (float)ui->horizontalSlider_ShiftSpeed->maximum());
    b->Serve(itterration, f);
}

float Widget::GetSpanMin(){return (float)ui->horizontalSlider_SpanMin->value() / (float)ui->horizontalSlider_SpanMin->maximum();}
float Widget::GetSpanMax(){return (float)ui->horizontalSlider_SpanMax->value() / (float)ui->horizontalSlider_SpanMax->maximum();}
float Widget::GetSpanOffset(){return (float)ui->horizontalSlider_SpanOffset->value() / (float)ui->horizontalSlider_SpanOffset->maximum();}
*/
inline static bool x_less_than(const QPointF &p1, const QPointF &p2)
{
    return p1.x() < p2.x();
}

