#include "AudioPlayerFrontend.h"
#include "ui_AudioPlayerFrontend.h"
#include <QTime>
#include <QFileDialog>
#include <QDir>

AudioPlayerFrontend::AudioPlayerFrontend(AudioPlayer &_audioPlayer, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioPlayerFrontend),
    audioPlayer(_audioPlayer),
    seekSlider(this, -5000, 5000)
{
    ui->setupUi(this);
    ui->verticalLayout_seekSlider->addWidget(&seekSlider);

    QObject::connect(ui->pushButton_Play, SIGNAL(clicked()), this, SLOT(Slot_Play()));
    QObject::connect(ui->pushButton_Pause, SIGNAL(clicked()), this, SLOT(Slot_Pause()));
    QObject::connect(&seekSlider, SIGNAL(ValueChangedAbs(int)), this, SLOT(Slot_Seek(int)));
    QObject::connect(&audioPlayer, SIGNAL(Signal_DurationChanged()), this, SLOT(Slot_DurationChanged()));
    QObject::connect(&_audioPlayer, SIGNAL(Signal_TimerExpired(qint64)), this, SLOT(Slot_UpdatePlayerPos(qint64)));
}

void AudioPlayerFrontend::Slot_UpdatePlayerPos(qint64 playerPos)
{
    UpdateCurrentTime();
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
    QString time;
    //audioPlayer.GetLength(time);
    //ui->label_duration->setText(time);
    Utilities::MillisToTimeString(time, audioPlayer.GetLength());
    ui->label_duration->setText(time);
}

void AudioPlayerFrontend::UpdateCurrentTime()
{
    QString time;
    Utilities::MillisToTimeString(time, audioPlayer.GetPosition());
    ui->label_curPos->setText(time);
}

void AudioPlayerFrontend::on_pushButton_BrowseSong_clicked()
{
    QString selectedSong =
        QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, tr("Select File"), QDir::currentPath()));
    selectedSong.replace("\\", "/");
    qDebug() << selectedSong;
    if(selectedSong.isEmpty())return;
    audioPlayer.SetSong(selectedSong);
}

