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
            topCansWhite.RegisterClient(devices[indexAbs]->GetWhiteMapper());

            // build Level 0
            colWheel[indexAbs].SetRgbDevice((devices[indexAbs]));
            colWheel[indexAbs].GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
            bsmSection[section].RegisterClientToItem(device, &(colWheel[indexAbs]));
            bsmSection[section].GetBundleSeries(device)->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
            bsmSection[section].GetBundleSeries(device)->SetSerParamShift(&shiftDeviceTop);
            bsmSection[section].GetBundleSeries(device)->SetSerParamSpanMin(&spanMinTop);
            bsmSection[section].GetBundleSeries(device)->SetSerParamSpanMax(&spanMaxTop);

            bsDimm.RegisterClient(bsmDimm.GetBundleSeries(indexAbs));
            bsmDimm.GetBundleSeries(indexAbs)->SetSerParamShift(&dimmShift);
            bsmDimm.GetBundleSeries(indexAbs)->GetFuncCont()->ClearSections();
            bsmDimm.GetBundleSeries(indexAbs)->GetFuncCont()->AddFunctionSectionByParams(0.5, 0.0, 1.0, 0.0);//0.25);
            bsmDimm.GetBundleSeries(indexAbs)->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.5, 0.0, 1.0);//0.25, 1.0);
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

    MovingHead_RGBWA_UV_t headInit;

            headInit.red = 3;
             headInit.green = 4;
             headInit.blue = 5;
             headInit.white = 6;
             headInit.dimm = 2;
             headInit.amber = 7;
             headInit.uv = 8;
             headInit.x = 0;
             headInit.y = 1;
             headInit.xy = 9;

    topHeadsColor.RegisterClient(&bsMasterHeads);
    topHeadsMove.RegisterClient(&bsHeadsPan);
    topHeadsDimm.RegisterClient(&bsHeadsDimm);
    //topHeadsShift

    bsMasterHeads.GetFuncCont()->ClearSections();
    bsMasterHeads.GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
    bsmHeads.GenerateBundleSeries(AMT_DEVICES);
    bsmHeadsSpan.GenerateBundleSeries(AMT_DEVICES);
    for(int i=0; i<AMT_DEVICES; i++)
    {
        bsmHeads.GetBundleSeries(i)->GetFuncCont()->ClearSections();
        bsmHeads.GetBundleSeries(i)->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
        bsMasterHeads.RegisterClient(bsmHeads.GetBundleSeries(i));
        movingHead[i] = new MovingHead_RGBWA_UV(universum);
        headInit.adr = 97+(i*10);
        movingHead[i]->Init(headInit);
        movingHead[i]->GetDimmMapper()->GetFuncCont()->ClearSections();
        movingHead[i]->GetDimmMapper()->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 0.5, 0.0);

        movingHead[i]->GetPanMapper()->GetFuncCont()->ClearSections();
        if(i & 1)
        {
            movingHead[i]->GetPanMapper()->GetFuncCont()->AddFunctionSectionByParams(0.5, 0.0, 0.4, 0.6);
            movingHead[i]->GetPanMapper()->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.5, 0.6, 0.4);
        }
        else
        {
            movingHead[i]->GetPanMapper()->GetFuncCont()->AddFunctionSectionByParams(0.5, 0.0, 0.6, 0.4);
            movingHead[i]->GetPanMapper()->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.5, 0.4, 0.6);
        }

        movingHead[i]->GetTiltMapper()->GetFuncCont()->ClearSections();
        movingHead[i]->GetTiltMapper()->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 0.1, 0.1);


        colWheelHeads[i].SetRgbDevice(movingHead[i]);
        colWheelHeads[i].GetFuncCont()->ClearSections();
        colWheelHeads[i].GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
        bsmHeads.GetBundleSeries(i)->RegisterClient(bsmHeadsSpan.GetBundleSeries(i));

        bsmHeadsSpan.GetBundleSeries(i)->RegisterClient(&colWheelHeads[i]);
        bsmHeadsSpan.SetSerParamShiftToItems(&topHeadsShift);
        bsmHeadsSpan.GetBundleSeries(i)->GetFuncCont()->ClearSections();
        bsmHeadsSpan.GetBundleSeries(i)->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
        bsmHeadsSpan.GetBundleSeries(i)->SetSerParamSpanMax(&spanMaxTopHeads);
        bsmHeadsSpan.GetBundleSeries(i)->SetSerParamSpanMin(&spanMinTopHeads);

        bsHeadsPan.GetFuncCont()->ClearSections();
        bsHeadsPan.GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
        bsHeadsPan.RegisterClient(movingHead[i]->GetPanMapper());

        bsmHeads.GetBundleSeries(i)->RegisterClient(movingHead[i]->GetTiltMapper());

        bsHeadsDimm.GetFuncCont()->ClearSections();
        bsHeadsDimm.GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
        bsHeadsDimm.RegisterClient(movingHead[i]->GetDimmMapper());
    }

    topHeadsColorSpeaker.RegisterClient(&bsMasterHeadsSpeaker);
    topHeadsMoveSpeaker.RegisterClient(&bsHeadsPanSpeaker);
    topHeadsDimmSpeaker.RegisterClient(&bsHeadsDimmSpeaker);

    bsMasterHeadsSpeaker.GetFuncCont()->ClearSections();
    bsMasterHeadsSpeaker.GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
    bsmHeadsSpeaker.GenerateBundleSeries(AMT_SPEAKER_HEADS);

    bsmHeadsSpanSpeaker.GenerateBundleSeries(AMT_SPEAKER_HEADS);
    for(int i=0; i<AMT_SPEAKER_HEADS; i++)
    {
        bsmHeadsSpeaker.GetBundleSeries(i)->GetFuncCont()->ClearSections();
        bsmHeadsSpeaker.GetBundleSeries(i)->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
        bsMasterHeadsSpeaker.RegisterClient(bsmHeadsSpeaker.GetBundleSeries(i));
        movingHeadSpeaker[i] = new MovingHead_RGBWA_UV(universum);
        headInit.adr = 137+(i*10);
        movingHeadSpeaker[i]->Init(headInit);
        movingHeadSpeaker[i]->GetDimmMapper()->GetFuncCont()->ClearSections();
        movingHeadSpeaker[i]->GetDimmMapper()->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 0.5, 0.0);

        movingHeadSpeaker[i]->GetPanMapper()->GetFuncCont()->ClearSections();
        movingHeadSpeaker[i]->GetTiltMapper()->GetFuncCont()->ClearSections();
        /**/
        if(i & 1)
        {
        movingHeadSpeaker[i]->GetTiltMapper()->GetFuncCont()->AddFunctionSectionByParams(0.5, 0.0, 1.0, 0.0);
        movingHeadSpeaker[i]->GetTiltMapper()->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.5, 0.0, 1.0);
        }
        else
        {
        movingHeadSpeaker[i]->GetTiltMapper()->GetFuncCont()->AddFunctionSectionByParams(0.5, 0.0, 0.0, 1.0);
        movingHeadSpeaker[i]->GetTiltMapper()->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.5, 1.0, 0.0);
        }


        movingHeadSpeaker[i]->GetPanMapper()->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 0.25, 0.25);





        colWheelHeadsSpeaker[i].SetRgbDevice(movingHeadSpeaker[i]);
        colWheelHeadsSpeaker[i].GetFuncCont()->ClearSections();
        colWheelHeadsSpeaker[i].GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
        bsmHeadsSpeaker.GetBundleSeries(i)->RegisterClient(bsmHeadsSpanSpeaker.GetBundleSeries(i));

        bsmHeadsSpanSpeaker.GetBundleSeries(i)->RegisterClient(&colWheelHeadsSpeaker[i]);
        bsmHeadsSpanSpeaker.GetBundleSeries(i)->GetFuncCont()->ClearSections();
        bsmHeadsSpanSpeaker.GetBundleSeries(i)->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
        bsmHeadsSpanSpeaker.GetBundleSeries(i)->SetSerParamSpanMax(&spanMaxTopHeadsSpeaker);
        bsmHeadsSpanSpeaker.GetBundleSeries(i)->SetSerParamSpanMin(&spanMinTopHeadsSpeaker);

        bsmHeadsSpeaker.GetBundleSeries(i)->RegisterClient(movingHeadSpeaker[i]->GetPanMapper());

        bsHeadsPanSpeaker.GetFuncCont()->ClearSections();
        bsHeadsPanSpeaker.GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
        bsHeadsPanSpeaker.RegisterClient(movingHeadSpeaker[i]->GetTiltMapper());

        bsHeadsDimmSpeaker.GetFuncCont()->ClearSections();
        bsHeadsDimmSpeaker.GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
        bsHeadsDimmSpeaker.RegisterClient(movingHeadSpeaker[i]->GetDimmMapper());
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
    //QObject::connect(&(spanOffsetTopSection[2]), &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanOffsetSection_3);
    QObject::connect(&dimmValueTop, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetDimmValue);
    QObject::connect(&dimmShift, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetDimmShift);
    QObject::connect(&spanMinTop, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanMin);
    QObject::connect(&spanMaxTop, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanMax);

    QObject::connect(&topHeadsColor, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetHeadColorPos);
    QObject::connect(&topHeadsShift, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetHeadsShift);
    QObject::connect(&topHeadsMove, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetHeadMovePos);
    QObject::connect(&spanMinTopHeads, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanMinHeads);
    QObject::connect(&spanMaxTopHeads, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanMaxHeads);
    QObject::connect(&topHeadsDimm, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetDimmHeads);

    QObject::connect(&topHeadsColorSpeaker, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetHeadColorPosSpeaker);
    QObject::connect(&topHeadsMoveSpeaker, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetHeadMovePosSpeaker);
    QObject::connect(&spanMinTopHeadsSpeaker, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanMinHeadsSpeaker);
    QObject::connect(&spanMaxTopHeadsSpeaker, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanMaxHeadsSpeaker);
    QObject::connect(&topHeadsDimmSpeaker, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetDimmHeadsSpeaker);

    QObject::connect(&cT, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetValue);
    QObject::connect(&topCansWhite, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetCanWhite);

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
    colWheelHeads[0].GetRequested(itteration);
    bsHeadsPan.GetRequested(itteration);
    bsHeadsDimm.GetRequested(itteration);

    colWheelHeadsSpeaker[0].GetRequested(itteration);
    bsHeadsPanSpeaker.GetRequested(itteration);
    bsHeadsDimmSpeaker.GetRequested(itteration);

    bsDimm.GetRequested(itteration);
    devices[0]->GetWhiteMapper()->GetRequested(itteration);


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

    if(ui->checkBox_AutoIncHeadColor->isChecked())
    {
        int v = ui->horizontalSlider_PositionHeadsColor->value();
        v += (ui->horizontalSlider_SpeedHeadsColor->value());
        if(v > ui->horizontalSlider_PositionHeadsColor->maximum()) v=0;
        ui->horizontalSlider_PositionHeadsColor->setSliderPosition(v);
    }

    if(ui->checkBox_AutoIncHeadMove->isChecked())
    {
        int v = ui->horizontalSlider_PositionHeadsMove->value();
        v += (ui->horizontalSlider_SpeedHeadsMove->value());
        if(v > ui->horizontalSlider_PositionHeadsMove->maximum()) v=0;
        ui->horizontalSlider_PositionHeadsMove->setSliderPosition(v);
    }
    ///////////
    if(ui->checkBox_AutoIncHeadColorSpeaker->isChecked())
    {
        int v = ui->horizontalSlider_PositionHeadsColorSpeaker->value();
        v += (ui->horizontalSlider_SpeedHeadsColorSpeaker->value());
        if(v > ui->horizontalSlider_PositionHeadsColorSpeaker->maximum()) v=0;
        ui->horizontalSlider_PositionHeadsColorSpeaker->setSliderPosition(v);
    }

    if(ui->checkBox_AutoIncHeadMoveSpeaker->isChecked())
    {
        int v = ui->horizontalSlider_PositionHeadsMoveSpeaker->value();
        v += (ui->horizontalSlider_SpeedHeadsMoveSpeaker->value());
        if(v > ui->horizontalSlider_PositionHeadsMoveSpeaker->maximum()) v=0;
        ui->horizontalSlider_PositionHeadsMoveSpeaker->setSliderPosition(v);
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
    qDebug() << " ";
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


void Widget::Slot_GetCanWhite(ClientServer_Top *b, int itterration)
{
    float tmpF = (float)ui->verticalSlider_CansWhite->value() / (float)ui->verticalSlider_CansWhite->maximum();
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


void Widget::Slot_GetSpanMin(ClientServer_Top *b, int itterration)
{
    float tmpF = ((float)ui->horizontalSlider_SpanMin->value() / (float)ui->horizontalSlider_SpanMin->maximum());
    b->Serve(itterration,tmpF);
}

void Widget::Slot_GetSpanMax(ClientServer_Top *b, int itterration)
{
    float tmpF = ((float)ui->horizontalSlider_SpanMax->value() / (float)ui->horizontalSlider_SpanMax->maximum());
    b->Serve(itterration,tmpF);
}

void Widget::Slot_GetHeadColorPos(ClientServer_Top *b, int itterration)
{
    float tmpF = (float)ui->horizontalSlider_PositionHeadsColor->value() / (float)ui->horizontalSlider_PositionHeadsColor->maximum();
    ui->label_PosHeadColor->setText(QString::number(tmpF));
    b->Serve(itterration,tmpF);
}


void Widget::Slot_GetHeadsShift(ClientServer_Top *b, int itterration)
{
    float tmpF = ((float)ui->horizontalSlider_HeadsShift->value() / (float)ui->horizontalSlider_HeadsShift->maximum());
    tmpF = tmpF * (float)(ui->horizontalSlider_SpeedHeadsShift->value());
    b->Serve(itterration,tmpF);
}

void Widget::Slot_GetHeadMovePos(ClientServer_Top *b, int itterration)
{
    float tmpF = (float)ui->horizontalSlider_PositionHeadsMove->value() / (float)ui->horizontalSlider_PositionHeadsMove->maximum();
    b->Serve(itterration,tmpF);
}


void Widget::Slot_GetSpanMinHeads(ClientServer_Top *b, int itterration)
{
    float tmpF = ((float)ui->horizontalSlider_SpanMinHeads->value() / (float)ui->horizontalSlider_SpanMinHeads->maximum());
    b->Serve(itterration,tmpF);
}

void Widget::Slot_GetSpanMaxHeads(ClientServer_Top *b, int itterration)
{
    float tmpF = ((float)ui->horizontalSlider_SpanMaxHeads->value() / (float)ui->horizontalSlider_SpanMaxHeads->maximum());
    b->Serve(itterration,tmpF);
}


void Widget::Slot_GetDimmHeads(ClientServer_Top *b, int itterration)
{
    float tmpF = ((float)(ui->horizontalSlider_HeadsDimm->value()-1) / (float)ui->horizontalSlider_HeadsDimm->maximum());
    b->Serve(itterration,tmpF);
}

//////////

void Widget::Slot_GetHeadColorPosSpeaker(ClientServer_Top *b, int itterration)
{
    float tmpF = (float)ui->horizontalSlider_PositionHeadsColorSpeaker->value() / (float)ui->horizontalSlider_PositionHeadsColorSpeaker->maximum();
    b->Serve(itterration,tmpF);
}

void Widget::Slot_GetHeadMovePosSpeaker(ClientServer_Top *b, int itterration)
{
    float tmpF = (float)ui->horizontalSlider_PositionHeadsMoveSpeaker->value() / (float)ui->horizontalSlider_PositionHeadsMoveSpeaker->maximum();
    b->Serve(itterration,tmpF);
}


void Widget::Slot_GetSpanMinHeadsSpeaker(ClientServer_Top *b, int itterration)
{
    float tmpF = ((float)ui->horizontalSlider_SpanMinHeadsSpeaker->value() / (float)ui->horizontalSlider_SpanMinHeadsSpeaker->maximum());
    b->Serve(itterration,tmpF);
}

void Widget::Slot_GetSpanMaxHeadsSpeaker(ClientServer_Top *b, int itterration)
{
    float tmpF = ((float)ui->horizontalSlider_SpanMaxHeadsSpeaker->value() / (float)ui->horizontalSlider_SpanMaxHeadsSpeaker->maximum());
    b->Serve(itterration,tmpF);
}


void Widget::Slot_GetDimmHeadsSpeaker(ClientServer_Top *b, int itterration)
{
    float tmpF = ((float)(ui->horizontalSlider_HeadsDimmSpeaker->value()-1) / (float)ui->horizontalSlider_HeadsDimmSpeaker->maximum());
    b->Serve(itterration,tmpF);
}
