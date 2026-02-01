#include "SequenceItem.h"
#include "ui_SequenceItem.h"
#include <algorithm>

#include "../Utilities.hpp"

SequenceItem::SequenceItem(QWidget *parent, QString _collection, qint64 _time, qint64 _fadeIn) :
    QWidget(parent),
    ui(new Ui::SequenceItem)
{

    ui->setupUi(this);
    ui->pushButton_Collection->setText(_collection);
    timeStamp = _time;
    fadeIn = _fadeIn;
    DisplayTimeStamp();
    DisplayFadeIn();
}

SequenceItem::SequenceItem(QWidget *parent, QJsonObject &loadObj) :
    QWidget(parent),
    ui(new Ui::SequenceItem)
{
    ui->setupUi(this);
    ui->pushButton_Collection->setText(loadObj["Collection"].toString());
    timeStamp = loadObj["TimeStamp"].toInt();
    fadeIn = loadObj["FadeIn"].toInt();
    DisplayTimeStamp();
    DisplayFadeIn();

}

SequenceItem::~SequenceItem()
{
    delete ui;
}

QString SequenceItem::GetCollection()
{
    return ui->pushButton_Collection->text();
}

void SequenceItem::SetTimeForCurrentTarget(qint64 _time)
{
    if(currentTimeChangeTarget == 0)
    {
        timeStamp = _time;
        DisplayTimeStamp();
    }
    else if(currentTimeChangeTarget == 1)
    {
        fadeIn = _time;
        DisplayFadeIn();
    }
}

qint64 SequenceItem::GetTimeFromCurrentTarget()
{
    if(currentTimeChangeTarget == 0)
    {
        return timeStamp;
    }
    else if(currentTimeChangeTarget == 1)
    {
        return fadeIn;
    }

    return 0;
}

void SequenceItem::DisplayTimeStamp()
{
    QString time;
    Utilities::MillisToTimeString(time, timeStamp);
    ui->pushButton_Time->setText(time);
}

void SequenceItem::DisplayFadeIn()
{
    QString time;
    Utilities::MillisToTimeString(time, fadeIn);
    ui->pushButton_FadeIn->setText(time);
}

void SequenceItem::on_pushButton_Time_clicked()
{
    currentTimeChangeTarget = 0;
    emit Signal_ConnectForTimeChange(this);
}


void SequenceItem::on_pushButton_FadeIn_clicked()
{
    currentTimeChangeTarget = 1;
    emit Signal_ConnectForTimeChange(this);
}

void SequenceItem::Save(QJsonArray &jsonSequenceItem) const
{
   QJsonObject jObj;
   jObj["Collection"] = ui->pushButton_Collection->text();
   jObj["TimeStamp"] = QString::number(timeStamp);
   jObj["FadeIn"] = QString::number(fadeIn);
   jsonSequenceItem.append(jObj);
}


void SequenceItem::on_pushButton_Collection_clicked()
{
    currentTimeChangeTarget = 2;
    emit Signal_ConnectForTimeChange(this);
}

