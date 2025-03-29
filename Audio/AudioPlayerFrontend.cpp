#include "AudioPlayerFrontend.h"
#include "ui_AudioPlayerFrontend.h"
#include <QTime>

AudioPlayerFrontend::AudioPlayerFrontend(AudioPlayer &_audioPlayer, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioPlayerFrontend),
    audioPlayer(_audioPlayer),
    seekSlider(this, -5000, 5000),
    lines(0,0,400,200, this)
{
    ui->setupUi(this);
    ui->verticalLayout_seekSlider->addWidget(&seekSlider);
    ui->verticalLayout_lines->addWidget(&lines);

    QObject::connect(ui->pushButton_Play, SIGNAL(clicked()), this, SLOT(Slot_Play()));
    QObject::connect(ui->pushButton_Pause, SIGNAL(clicked()), this, SLOT(Slot_Pause()));
    QObject::connect(&seekSlider, SIGNAL(ValueChangedAbs(int)), this, SLOT(Slot_Seek(int)));
    QObject::connect(&audioPlayer, SIGNAL(Signal_DurationChanged()), this, SLOT(Slot_DurationChanged()));
}

AudioPlayerFrontend::~AudioPlayerFrontend()
{
    delete ui;
}

void AudioPlayerFrontend::Slot_Play()
{
    audioPlayer.Play();
}
void AudioPlayerFrontend::Slot_Pause()
{
    audioPlayer.Pause();
}
void AudioPlayerFrontend::Slot_Seek(int _seek)
{
    audioPlayer.SeekBy(_seek);
}
void AudioPlayerFrontend::Slot_DurationChanged()
{
    QTime time;
    audioPlayer.GetLength(time);
    ui->label_duration->setText(time.toString("hh:mm:ss:ms"));

    qint64 length;
    audioPlayer.GetLength(length);
    length /= 10;
    lines.SetWidth(length);
}

void AudioPlayerFrontend::UpdateCurrentTime()
{
    QTime time;
    audioPlayer.GetCurTime(time);
    ui->label_curPos->setText(time.toString("hh:mm:ss:ms"));
}
