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
    for(int i = ui->stackedWidget->count()-1; i >= 0; i--)
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

    WWA_Strip_t RGBW_Dimm_MovingHead_Init;

    RGBW_Dimm_MovingHead_Init.cold = 0;
    RGBW_Dimm_MovingHead_Init.warm = 1;
    RGBW_Dimm_MovingHead_Init.amber = 2;

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

    vector<Mapper_Base *> funcCont_1, funcCont_2, funcCont_3;
    vector<vector<Mapper_Base *> > funcContainerContainers;
    funcContainerContainers.push_back(funcCont_1);
    funcContainerContainers.push_back(funcCont_2);
    funcContainerContainers.push_back(funcCont_3);

    for(int m=0; m<AMT_DEVICES; m++)
    {
        movingHead[m] = new WWA_Strip(universum);
        RGBW_Dimm_MovingHead_Init.adr = (m*3)+0;
        movingHead[m]->Init(RGBW_Dimm_MovingHead_Init);

        bsmMaster.RegisterClient(bundleSeriesDevice.GetBundleSeries(m));
        bundleSeriesDevice.GetBundleSeries(m)->GetFuncCont()->AddFunctionSectionByParams(1.0, 0.0, 1.0, 0.0);

        bundleSeriesDevice.GetBundleSeries(m)->SetSerParamShift(&shiftTop);

        bundleSeriesDevice.GetBundleSeries(m)->RegisterClient(movingHead[m]->GetColdMapper());
        movingHead[m]->GetColdMapper()->GetType()->append("Cold");
        funcContainerContainers[0].push_back((movingHead[m]->GetColdMapper()));

        bundleSeriesDevice.GetBundleSeries(m)->RegisterClient(movingHead[m]->GetWarmMapper());
        movingHead[m]->GetWarmMapper()->GetType()->append("Warm");
        funcContainerContainers[1].push_back((movingHead[m]->GetWarmMapper()));

        bundleSeriesDevice.GetBundleSeries(m)->RegisterClient(movingHead[m]->GetAmberMapper());
        movingHead[m]->GetAmberMapper()->GetType()->append("Amber");
        funcContainerContainers[2].push_back((movingHead[m]->GetAmberMapper()));
    }

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
            bundleSeriesDevice.GetBundleSeries(0)->GetRequested(itteration);
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
    //qDebug() <<  debugMsg.toLatin1();
    //qDebug() <<  "_";
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
}

void Widget::Slot_GetShift(ClientServer_Top *b, int itterration)
{
    float tmpF = (float)ui->horizontalSlider_2->value() / (float)ui->horizontalSlider_2->maximum();
    tmpF *= 20;
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
