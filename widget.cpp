#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent)
    :   QWidget(parent),
        ui(new Ui::Widget),
        effectEditor(0,0,400,200, this),
        audioPlayerFrontEnd(audioPlayer, this)
{
    ui->setupUi(this);
    for(int i = ui->stackedWidget->count(); i >= 0; i--)
    {
        QWidget* widget = ui->stackedWidget->widget(i);
        ui->stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }
    ui->stackedWidget->addWidget(&audioPlayerFrontEnd);
    //ui->verticalLayout_Player->addWidget(&audioPlayerFrontEnd);

    for(int i=0; i<UNIV_LENGTH; i++)
    {
        universum.push_back(&(buf[i]));
    }

    RGBWA_UV_MiniMovingHead_Init_t RGBW_Dimm_MovingHead_Init;

    RGBW_Dimm_MovingHead_Init.red = 3;
    RGBW_Dimm_MovingHead_Init.green = 4;
    RGBW_Dimm_MovingHead_Init.blue = 5;
    RGBW_Dimm_MovingHead_Init.white = 6;
    RGBW_Dimm_MovingHead_Init.dimm = 2;
    RGBW_Dimm_MovingHead_Init.amber = 7;
    RGBW_Dimm_MovingHead_Init.uv = 8;
    RGBW_Dimm_MovingHead_Init.x = 0;
    RGBW_Dimm_MovingHead_Init.y = 1;
    RGBW_Dimm_MovingHead_Init.xy = 9;

    cT.RegisterClient(&bsmMaster);

    bundleSeriesDevice.GenerateBundleSeries(AMT_DEVICES);
    bsmPan.GenerateBundleSeries(AMT_DEVICES);
    bsmTilt.GenerateBundleSeries(AMT_DEVICES);
    bsmDimm.GenerateBundleSeries(AMT_DEVICES);
    bsmWheel.GenerateBundleSeries(AMT_DEVICES);
    bsmWheel.SetSerParamSpanMinToItems(&spanMinTop);
    bsmWheel.SetSerParamSpanMaxToItems(&spanMaxTop);
    bsmWheel.SetSerParamSpanOffsetToItems(&spanOffsetTop);
    bsmWheel.SetSerParamSpeedToItems(&shiftSpeed);


    cT.RegisterClient(&bsmMaster);
    bsmMaster.GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);

    std::vector<ProportionIncrementalDot*> inPanVecR;
    std::vector<Dot*> outPanDotVecR;
    inPanVecR.push_back(new ProportionIncrementalDot(1.0, 1.0));
    GenerateSequentialXvalues(inPanVecR, outPanDotVecR, 0.0);
    /*
    inPanVecR.push_back(new ProportionIncrementalDot(1.0, 0.75));
    inPanVecR.push_back(new ProportionIncrementalDot(1.0, 0.75));
    inPanVecR.push_back(new ProportionIncrementalDot(1.0, 0.25));
    inPanVecR.push_back(new ProportionIncrementalDot(1.0, 0.25));
    GenerateSequentialXvalues(inPanVecR, outPanDotVecR, 0.25);
    */

    std::vector<ProportionIncrementalDot*> inTiltVecR;
    std::vector<Dot*> outTiltDotVecR;
    inTiltVecR.push_back(new ProportionIncrementalDot(1.0, 1.0));
    GenerateSequentialXvalues(inTiltVecR, outTiltDotVecR, 0.0);
    /*
    inTiltVecR.push_back(new ProportionIncrementalDot(0.5, 0.0));
    inTiltVecR.push_back(new ProportionIncrementalDot(1.0, 1.0));
    inTiltVecR.push_back(new ProportionIncrementalDot(0.5, 1.0));
    inTiltVecR.push_back(new ProportionIncrementalDot(1.0, 0.0));
    GenerateSequentialXvalues(inTiltVecR, outTiltDotVecR, 0.0);
    */

    std::vector<ProportionIncrementalDot*> inPanVecL;
    std::vector<Dot*> outPanDotVecL;
    inPanVecL.push_back(new ProportionIncrementalDot(1.0, 1.0));
    GenerateSequentialXvalues(inPanVecL, outPanDotVecL, 0.0);
    /*
    inPanVecL.push_back(new ProportionIncrementalDot(1.0, 0.25));
    inPanVecL.push_back(new ProportionIncrementalDot(1.0, 0.25));
    inPanVecL.push_back(new ProportionIncrementalDot(1.0, 0.75));
    inPanVecL.push_back(new ProportionIncrementalDot(1.0, 0.75));
    GenerateSequentialXvalues(inPanVecL, outPanDotVecL, 0.75);
    */

    std::vector<ProportionIncrementalDot*> inTiltVecL;
    std::vector<Dot*> outTiltDotVecL;
    inTiltVecL.push_back(new ProportionIncrementalDot(1.0, 1.0));
    GenerateSequentialXvalues(inTiltVecL, outTiltDotVecL, 0.0);
    /*
    inTiltVecL.push_back(new ProportionIncrementalDot(0.5, 1.0));
    inTiltVecL.push_back(new ProportionIncrementalDot(1.0, 0.0));
    inTiltVecL.push_back(new ProportionIncrementalDot(0.5, 0.0));
    inTiltVecL.push_back(new ProportionIncrementalDot(1.0, 1.0));
    GenerateSequentialXvalues(inTiltVecL, outTiltDotVecL, 1.0);
    */

    vector<FunctionOwners *> funcCont_1, funcCont_2, funcCont_3,funcCont_4;
    vector<vector<FunctionOwners *> > funcContainerContainers;
    funcContainerContainers.push_back(funcCont_1);
    funcContainerContainers.push_back(funcCont_2);
    funcContainerContainers.push_back(funcCont_3);
    funcContainerContainers.push_back(funcCont_4);

    for(int m=0; m<AMT_DEVICES; m++)
    {
        movingHead[m] = new RGBWA_UV_MiniMovingHead(universum);
        RGBW_Dimm_MovingHead_Init.adr = (m*10)+1;
        movingHead[m]->Init(RGBW_Dimm_MovingHead_Init);
        colWheel[m].SetRgbDevice((movingHead[m]));
        //bundleSeriesManager.GenerateBundleSeries(1);
        //bundleSeriesManager.SetSerPosToItems(&cT);
        //bundleSeriesManager.RegisterClientToItem(0, &colWheel);

        bsmMaster.RegisterClient(bundleSeriesDevice.GetBundleSeries(m));
        //bundleSeriesDevice[m].SetSerPosToItems(&bsmMaster);
    /**/


        bundleSeriesDevice.GetBundleSeries(m)->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
        //bundleSeriesDevice.GetBundleSeries(m)->AddFunctionSectionByParams(0.5, 0.0, 1.0, 0.0);
        //bundleSeriesDevice.GetBundleSeries(m)->AddFunctionSectionByParams(1.0, 0.5, 0.0, 1.0);

        bundleSeriesDevice.GetBundleSeries(m)->SetSerParamShift(&shiftTop);

        bundleSeriesDevice.GetBundleSeries(m)->RegisterClient(movingHead[m]->GetPanMapper());
        movingHead[m]->GetPanMapper()->GetType()->append("Pan");
        funcContainerContainers[0].push_back((movingHead[m]->GetPanMapper()));
        //bsmPan.RegisterClientToItem(m, movingHead[m]->GetPanMapper());

        //bsmPan.RegisterClientToItem(m, movingHead[m]->GetTiltMapper());
        //bsmPan.GetBundleSeries(m)->AddFunctionSectionByParams(1.0, 0.0, 0.85, 0.15);

        if(m&1)
        {
            for(Dot* d: outPanDotVecR)
            {
                bsmPan.GetBundleSeries(m)->GetFuncCont()->AddFunctionSectionByParams(d->maxX, d->minX, d->maxY, d->minY);
            }
        }
        else
        {
            for(Dot* d: outPanDotVecL)
            {
                bsmPan.GetBundleSeries(m)->GetFuncCont()->AddFunctionSectionByParams(d->maxX, d->minX, d->maxY, d->minY);
            }
        }
        /*
         //#1
        bsmPan.GetBundleSeries(m)->AddFunctionSectionByParams(0.25, 0.0, 0.75, 0.5);
        bsmPan.GetBundleSeries(m)->AddFunctionSectionByParams(0.5, 0.25, 0.5, 0.75);
        bsmPan.GetBundleSeries(m)->AddFunctionSectionByParams(0.75, 0.5, 0.25, 0.5);
        bsmPan.GetBundleSeries(m)->AddFunctionSectionByParams(1.0, 0.75, 0.5, 0.25);
        */

        bundleSeriesDevice.GetBundleSeries(m)->RegisterClient(movingHead[m]->GetTiltMapper());
        movingHead[m]->GetTiltMapper()->GetType()->append("Tilt");
        funcContainerContainers[1].push_back((movingHead[m]->GetTiltMapper()));
        //bsmPan.RegisterClientToItem(m, movingHead[m]->GetTiltMapper());
        //bsmPan.GetBundleSeries(m)->AddFunctionSectionByParams(1.0, 0.0, 0.85, 0.15);

        //#1
        //bsmTilt.GetBundleSeries(m)->AddFunctionSectionByParams(0.5, 0.0, 1.0, 0.0);
        //bsmTilt.GetBundleSeries(m)->AddFunctionSectionByParams(1.0, 0.5, 0.0, 1.0);

       // bsmTilt.GetBundleSeries(m)->AddFunctionSectionByParams(0.75, 0.5, 0.4, 0.0);
        //bsmTilt.GetBundleSeries(m)->AddFunctionSectionByParams(1.0, 0.75, 0.0, 0.4);

        /**/
        if(m&1)
        {
            for(Dot* d: outTiltDotVecR)
            {
                bsmTilt.GetBundleSeries(m)->GetFuncCont()->AddFunctionSectionByParams(d->maxX, d->minX, d->maxY, d->minY);
            }
        }
        else
        {
            for(Dot* d: outTiltDotVecL)
            {
                bsmTilt.GetBundleSeries(m)->GetFuncCont()->AddFunctionSectionByParams(d->maxX, d->minX, d->maxY, d->minY);
            }
        }

        bundleSeriesDevice.GetBundleSeries(m)->RegisterClient(movingHead[m]->GetDimmMapper());
        movingHead[m]->GetDimmMapper()->GetType()->append("Dimm");
        funcContainerContainers[2].push_back((movingHead[m]->GetDimmMapper()));

        bsmDimm.GetBundleSeries(m)->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);
    /*
    #define BLINK_BEGIN 0.15
    #define BLINK_END 0.85
        bsmDimm.GetBundleSeries(m)->AddFunctionSectionByParams(BLINK_BEGIN, 0.0, 1.0, 1.0);
    #define BLINKS  11
        int i =0;
        bsmDimm.GetBundleSeries(m)->AddFunctionSectionByParams(BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)(i+1)), BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)i), 0.0, 0.0); i++;
        bsmDimm.GetBundleSeries(m)->AddFunctionSectionByParams(BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)(i+1)), BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)i), 1.0, 1.0); i++;
        bsmDimm.GetBundleSeries(m)->AddFunctionSectionByParams(BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)(i+1)), BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)i), 0.0, 0.0); i++;
        bsmDimm.GetBundleSeries(m)->AddFunctionSectionByParams(BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)(i+1)), BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)i), 1.0, 1.0); i++;
        bsmDimm.GetBundleSeries(m)->AddFunctionSectionByParams(BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)(i+1)), BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)i), 0.0, 0.0); i++;
        bsmDimm.GetBundleSeries(m)->AddFunctionSectionByParams(BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)(i+1)), BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)i), 1.0, 1.0); i++;
        bsmDimm.GetBundleSeries(m)->AddFunctionSectionByParams(BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)(i+1)), BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)i), 0.0, 0.0); i++;
        bsmDimm.GetBundleSeries(m)->AddFunctionSectionByParams(BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)(i+1)), BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)i), 1.0, 1.0); i++;
        bsmDimm.GetBundleSeries(m)->AddFunctionSectionByParams(BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)(i+1)), BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)i), 0.0, 0.0); i++;
        bsmDimm.GetBundleSeries(m)->AddFunctionSectionByParams(BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)(i+1)), BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)i), 1.0, 1.0); i++;
        bsmDimm.GetBundleSeries(m)->AddFunctionSectionByParams(BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)(i+1)), BLINK_BEGIN+(((BLINK_END-BLINK_BEGIN)/(float)BLINKS)*(float)i), 0.0, 0.0);
        bsmDimm.GetBundleSeries(m)->AddFunctionSectionByParams(1.0, BLINK_END, 1.0, 1.0);
    */

        bundleSeriesDevice.GetBundleSeries(m)->RegisterClient(bsmWheel.GetBundleSeries(m));
        bsmWheel.RegisterClientToItem(m, &(colWheel[m]));
        colWheel[m].GetType()->append("Color Wheel");
        funcContainerContainers[3].push_back(&(colWheel[m]));
        bsmWheel.GetBundleSeries(m)->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);

    }
/*
    for(int i=0; i<HEADS; i++)
    {
        RGBW_Dimm_MovingHead_Init.adr = (i*10)+1;
        RGBW_Dimm_MovingHead[i].Init(RGBW_Dimm_MovingHead_Init);
        wheel.RegisterDevice(&(RGBW_Dimm_MovingHead[i]));
        handTroughBright.RegisterDmxIndex(RGBW_Dimm_MovingHead[i].GetDimmChnlIndex());
        handTroughWhite.RegisterDmxIndex(RGBW_Dimm_MovingHead[i].GetWhiteChnlIndex());
        handTroughAmber.RegisterDmxIndex(RGBW_Dimm_MovingHead[i].GetAmberChnlIndex());
        handTroughUV.RegisterDmxIndex(RGBW_Dimm_MovingHead[i].GetUvChnlIndex());
        handTroughX.RegisterDmxIndex(RGBW_Dimm_MovingHead[i].GetXChnlIndex());
        handTroughY.RegisterDmxIndex(RGBW_Dimm_MovingHead[i].GetYChnlIndex());
        handTroughXY.RegisterDmxIndex(RGBW_Dimm_MovingHead[i].GetXYChnlIndex());
        oneChUpDown.RegisterDmxIndex(RGBW_Dimm_MovingHead[i].GetYChnlIndex());
        panArrHandTrough[i].RegisterDmxIndex(RGBW_Dimm_MovingHead[i].GetXChnlIndex());
        tiltArrHandTrough[i].RegisterDmxIndex(RGBW_Dimm_MovingHead[i].GetYChnlIndex());

    }*/

    serial.setPortName("COM5");
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.open(QIODevice::ReadWrite);

    QObject::connect(&shiftTop, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetShift);
    QObject::connect(&cT, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetValue);
    QObject::connect(&spanMinTop, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanMin);
    QObject::connect(&spanMaxTop, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanMax);
    QObject::connect(&spanOffsetTop, &ClientServer_Top::RequestValue, this, &Widget::Slot_GetSpanOffset);
    QObject::connect(&shiftSpeed, &ClientServer_Top::RequestValue, this, &Widget::Slot_ShiftSpeed);
    QObject::connect(&timer, &QTimer::timeout, this, &Widget::Slot_TimerExpired);

    QObject::connect(ui->pushButton_prv, &QPushButton::clicked, this, &Widget::Slot_PrvStackedIndex);
    QObject::connect(ui->pushButton_nxt, &QPushButton::clicked, this, &Widget::Slot_NxtStackedIndex);


    //ui->verticalLayout_Dots->addWidget(&effectEditor);
    ui->stackedWidget->addWidget(&effectEditor);
    effectEditor.SetBundSerMangr(funcContainerContainers);
    /*
    m_red_shade = new ShadeWidget(ShadeWidget::RedShade, this);
    panShade = new ShadeWidget(ShadeWidget::RedShade, this);

    ui->verticalLayout_Dots->addWidget(&cusScrollAr);
    cusScrollAr.AddWidget(m_red_shade);
    cusScrollAr.AddWidget(panShade);
    //ui->verticalLayout_Dots->addWidget(m_red_shade);
    //ui->verticalLayout_Dots->addWidget(panShade);

    connect(m_red_shade, SIGNAL(colorsChanged()), this, SLOT(pointsUpdated()));
    connect(m_red_shade, SIGNAL(colorsChanged()), this, SLOT(Slot_MouseRelease()));
    connect(panShade, SIGNAL(colorsChanged()), this, SLOT(Slot_PanRelease()));
    */

    //Slot_MouseRelease();

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

    audioPlayerFrontEnd.UpdateCurrentTime();
    itteration++;
    for(int m=0; m<AMT_DEVICES; m++)
    {
        movingHead[m]->Ping(itteration);
        colWheel[m].GetRequested(itteration);
    }
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
    qDebug() << sendMsg.toLatin1();
    if(serial.isOpen())
    {
      serial.write(sendMsg.toLatin1());
    }
    //
}

void Widget::Slot_GetValue(ClientServer_Top *b, int itterration)
{
    float tmpF = (float)ui->horizontalSlider->value() / (float)ui->horizontalSlider->maximum();
    b->Serve(itterration,tmpF);
    ui->label->setText(QString::number(tmpF));
    effectEditor.DrawPositionLine(tmpF);
    //m_red_shade->DrawPositionLine(tmpF);
    //panShade->DrawPositionLine(tmpF);
   // qDebug() << "GetValue";
}

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

inline static bool x_less_than(const QPointF &p1, const QPointF &p2)
{
    return p1.x() < p2.x();
}

void Widget::pointsUpdated()
{
    QPolygonF points;

   // points += m_red_shade->points();

    std::sort(points.begin(), points.end(), x_less_than);

    /*
    QString dots;
    for (int i = 0; i < points.size(); ++i) {
        float x = (points.at(i).x());
        float y = (points.at(i).y());
        dots += " [X:" + QString::number(x) +"/Y: " + QString::number(y) + "]";
    }
    qDebug() << dots;
    */
}

void Widget::Slot_MouseRelease()
{
    QPolygonF points;
    //points += m_red_shade->points();
    vector<funcSection_t> sections;
    sections.resize(points.size()-1);
    int i=0;
    for(funcSection_t &s : sections)
    {
        s.minX = points[i].x();
        s.minY = points[i].y();

        s.maxX = points[i+1].x();
        s.maxY = points[i+1].y();
        i++;
    }
/*
    for(int m=0; m<AMT_DEVICES; m++)
    {
        bsmTilt.GetBundleSeries(m)->funcContainer.SetFunctionSections(sections);
    }
    */
}


void Widget::Slot_PanRelease()
{
    QPolygonF points;
    //points += panShade->points();
    vector<funcSection_t> sections;
    sections.resize(points.size()-1);
    int i=0;
    for(funcSection_t &s : sections)
    {
        s.minX = points[i].x();
        s.minY = points[i].y();

        s.maxX = points[i+1].x();
        s.maxY = points[i+1].y();
        i++;
    }

    for(int m=0; m<AMT_DEVICES; m++)
    {
        bsmPan. GetBundleSeries(m)->GetFuncCont()->SetFunctionSections(sections);
    }
}

void Widget::Slot_PrvStackedIndex()
{
    int curIndx = ui->stackedWidget->currentIndex();
    if(curIndx > 0) curIndx--;
    else curIndx = ui->stackedWidget->count()-1;
    ui->stackedWidget->setCurrentIndex(curIndx);
}

void Widget::Slot_NxtStackedIndex()
{
    int curIndx = ui->stackedWidget->currentIndex();
    curIndx++;
    if(curIndx == ui->stackedWidget->count()) curIndx = 0;
    ui->stackedWidget->setCurrentIndex(curIndx);
}
