#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent)
    :   QWidget(parent),
        ui(new Ui::Widget),
        device(universum)
{
    ui->setupUi(this);

    //ui->verticalLayout_Player->addWidget(&audioPlayerFrontEnd);

    for(int i=0; i<UNIV_LENGTH; i++)
    {
        universum.push_back(&(buf[i]));
        buf[i] = 0;
    }




    Device_t deviceInit;
    deviceInit.red = 0;
    deviceInit.green = 2;
    deviceInit.blue = 4;
    deviceInit.white = 6;

    cT.RegisterClient(&bsmMaster);
    dimmValueTop.RegisterClient(&bsDimm);

    bsmMaster.GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
    bsDimm.GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
    bsmOfSections.GenerateBundleSeries(AMT_SECTIONS);
    bsmDimm.GenerateBundleSeries(AMT_SECTIONS*AMT_DEVS_PER_SECTION);
    for(int section=0; section<AMT_SECTIONS; section++)
    {
        //
        bsmSection[section].GenerateBundleSeries(AMT_DEVS_PER_SECTION);
        for(int device=0; device<AMT_DEVS_PER_SECTION; device++)
        {
            int indexAbs = (section*AMT_DEVS_PER_SECTION)+device;

            // create devices
            devices[indexAbs] = new Device(universum);
            deviceInit.adr = 1+(indexAbs*CHNLS_PER_CAN);
            devices[indexAbs]->Init(deviceInit);

            // build Level 0
            colWheel[indexAbs].SetRgbDevice((devices[indexAbs]));
            colWheel[indexAbs].GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
            bsmSection[section].RegisterClientToItem(device, &(colWheel[indexAbs]));
            bsmSection[section].GetBundleSeries(device)->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
            bsmSection[section].GetBundleSeries(device)->SetSerParamShift(&shiftDeviceTop);

            bsDimm.RegisterClient(bsmDimm.GetBundleSeries(indexAbs));
            bsmDimm.GetBundleSeries(indexAbs)->SetSerParamShift(&dimmShift);
            bsmDimm.GetBundleSeries(indexAbs)->GetFuncCont()->AddFunctionSectionByParams(0.5, 0.0, 1.0, 0.25);
            bsmDimm.GetBundleSeries(indexAbs)->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.5, 0.25, 1.0);
            bsmDimm.GetBundleSeries(indexAbs)->RegisterClient(devices[indexAbs]->GetRgbDimmMapper());

            // build Level 1
            bsmOfSections.RegisterClientToItem(section, (bsmSection[section].GetBundleSeries(device)));
        }

        // build Level 2
        bsmMaster.RegisterClient(bsmOfSections.GetBundleSeries(section));
        bsmOfSections.GetBundleSeries(section)->SetSerParamShift(&shiftSectionTop);
        bsmOfSections.GetBundleSeries(section)->SetSerParamSpanOffset(&(spanOffsetTopSection[section]));
        bsmOfSections.GetBundleSeries(section)->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
    }

    serial.setPortName("COM5");
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.open(QIODevice::ReadWrite);

    QObject::connect(&shiftSectionTop, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSectionShift);
    QObject::connect(&shiftDeviceTop, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetDeviceShift);

    QObject::connect(&(spanOffsetTopSection[0]), &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanOffsetSection_1);
    QObject::connect(&(spanOffsetTopSection[1]), &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanOffsetSection_2);
    QObject::connect(&(spanOffsetTopSection[2]), &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanOffsetSection_3);
    QObject::connect(&dimmValueTop, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetDimmValue);
    QObject::connect(&dimmShift, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetDimmShift);

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
    /*
    float dimm = (float)ui->horizontalSlider_rgbDimm->value() / (float)ui->horizontalSlider_rgbDimm->maximum();
    for(int i=0; i<AMT_SECTIONS*AMT_DEVS_PER_SECTION; i++)
    {
        devices[i]->GetRgbDimmMapper()->Consume(itteration, dimm);
    }
    */
    colWheel[0].GetRequested(itteration);
    bsDimm.GetRequested(itteration);


    if(ui->checkBox_AutoIncMainPos->isChecked())
    {
        int v = ui->horizontalSlider_MainPosition->value();
        v += (ui->horizontalSlider_SpeedMainPos->value());
        if(v > ui->horizontalSlider_MainPosition->maximum()) v=0;
        ui->horizontalSlider_MainPosition->setSliderPosition(v);
    }

    if(ui->checkBox_AutoIncSectionShift->isChecked())
    {
        if(dirSectionShift == 0)
        {

           int v = ui->horizontalSlider_SectionShift->value();
                v += ui->horizontalSlider_SpeedSectionShift->value();
                if(v > ui->horizontalSlider_SectionShift->maximum()) dirSectionShift = 1;
                ui->horizontalSlider_SectionShift->setSliderPosition(v);
        }
        else
        {
           int v = ui->horizontalSlider_SectionShift->value();
                    if(v < ui->horizontalSlider_SpeedSectionShift->value())
                    {
                        dirSectionShift = 0;
                        v = 0;
                    }
                    else
                    {
                        v -= ui->horizontalSlider_SpeedSectionShift->value();
                    }
                    ui->horizontalSlider_SectionShift->setSliderPosition(v);
        }
    }

    if(ui->checkBox_AutoIncDevicesShift->isChecked())
    {
        if(dirDeviceShift == 0)
        {

           int v = ui->horizontalSlider_DeviceShift->value();
                v += ui->horizontalSlider_SpeedDevicesShift->value();
                if(v > ui->horizontalSlider_DeviceShift->maximum()) dirDeviceShift = 1;
                ui->horizontalSlider_DeviceShift->setSliderPosition(v);
        }
        else
        {
           int v = ui->horizontalSlider_DeviceShift->value();
                    if(v < ui->horizontalSlider_SpeedDevicesShift->value())
                    {
                        dirDeviceShift = 0;
                        v = 0;
                    }
                    else
                    {
                        v -= ui->horizontalSlider_SpeedDevicesShift->value();
                    }
                    ui->horizontalSlider_DeviceShift->setSliderPosition(v);
        }
    }

    if(ui->checkBox_AutoIncRgbDimmShift->isChecked())
    {
        if(dirDimmShift == 0)
        {

           int v = ui->horizontalSlider_DimmShift->value();
           v += 5;
                if(v >= ui->horizontalSlider_DimmShift->maximum()) dirDimmShift = 1;
                ui->horizontalSlider_DimmShift->setSliderPosition(v);
        }
        else
        {
           int v = ui->horizontalSlider_DimmShift->value();
           v -= 5;
           if(v <= ui->horizontalSlider_DimmShift->minimum())
           {
               dirDimmShift = 0;
               v = ui->horizontalSlider_DimmShift->minimum();
           }
           ui->horizontalSlider_DimmShift->setSliderPosition(v);
        }
    }

    if(ui->checkBox_AutoIncRgbDimm->isChecked())
    {
        if(dirDimm == 0)
        {

           int v = ui->horizontalSlider_rgbDimm->value();
           v += ui->horizontalSlider_SpeedRgbDimm->value();
                if(v >= ui->horizontalSlider_rgbDimm->maximum()) dirDimm = 1;
                ui->horizontalSlider_rgbDimm->setSliderPosition(v);
        }
        else
        {
           int v = ui->horizontalSlider_rgbDimm->value();
           v -= ui->horizontalSlider_SpeedRgbDimm->value();
           if(v <= ui->horizontalSlider_rgbDimm->minimum())
           {
               dirDimm = 0;
               v = ui->horizontalSlider_rgbDimm->minimum();
           }
           ui->horizontalSlider_rgbDimm->setSliderPosition(v);
        }
    }

 /*   if(ui->checkBox_Offset->isChecked())
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

void Widget::Slot_GetDimmValue(ClientServer_Top *b, int itterration)
{
    float tmpF = (float)ui->horizontalSlider_rgbDimm->value() / (float)ui->horizontalSlider_rgbDimm->maximum();
    b->Serve(itterration,tmpF);
}

void Widget::Slot_GetValue(ClientServer_Top *b, int itterration)
{
    float tmpF = (float)ui->horizontalSlider_MainPosition->value() / (float)ui->horizontalSlider_MainPosition->maximum();
    ui->label_MainPosition->setText(QString::number(tmpF));
    b->Serve(itterration,tmpF);
}

void Widget::Slot_GetSectionShift(ClientServer_Top *b, int itterration)
{
    float tmpF = ((float)ui->horizontalSlider_SectionShift->value() / (float)ui->horizontalSlider_SectionShift->maximum());
    tmpF = tmpF * (float)(ui->horizontalSlider_sectionShiftMulti->value());
    ui->label_SectionShift->setText(QString::number(tmpF));
    b->Serve(itterration,tmpF);
}



void Widget::Slot_GetDeviceShift(ClientServer_Top *b, int itterration)
{
    float tmpF = ((float)ui->horizontalSlider_DeviceShift->value() / (float)ui->horizontalSlider_DeviceShift->maximum());
    tmpF = tmpF * (float)(ui->horizontalSlider_SpeedDevicesShift->value());
    ui->label_DeviceShift->setText(QString::number(tmpF));
    b->Serve(itterration,tmpF);
}

void Widget::Slot_GetSpanOffsetSection_1(ClientServer_Top *b, int itterration)
{
    float tmpF = ((float)ui->horizontalSlider_OffsetSection_1->value() / (float)ui->horizontalSlider_OffsetSection_1->maximum());
    b->Serve(itterration,tmpF);
}

void Widget::Slot_GetSpanOffsetSection_2(ClientServer_Top *b, int itterration)
{
    float tmpF = ((float)ui->horizontalSlider_OffsetSection_2->value() / (float)ui->horizontalSlider_OffsetSection_2->maximum());
    b->Serve(itterration,tmpF);
}

void Widget::Slot_GetSpanOffsetSection_3(ClientServer_Top *b, int itterration)
{
    float tmpF = ((float)ui->horizontalSlider_OffsetSection_3->value() / (float)ui->horizontalSlider_OffsetSection_3->maximum());
    b->Serve(itterration,tmpF);
}
void Widget::Slot_GetDimmShift(ClientServer_Top *b, int itterration)
{
    float tmpF = ((float)ui->horizontalSlider_DimmShift->value() / (float)ui->horizontalSlider_DimmShift->maximum());
    tmpF = tmpF * (float)(ui->horizontalSlider_dimmShiftMulti->value());
    b->Serve(itterration,tmpF);
}

