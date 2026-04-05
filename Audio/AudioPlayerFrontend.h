#ifndef AUDIOPLAYERFRONTEND_H
#define AUDIOPLAYERFRONTEND_H

#include <QWidget>
#include "AudioPlayer.h"
#include "../UI/HorSliderSelfCentering.h"

namespace Ui {
class AudioPlayerFrontend;
}

class AudioPlayerFrontend : public QWidget
{
    Q_OBJECT

public:
    explicit AudioPlayerFrontend( AudioPlayer &_audioPlayer, QWidget *parent = nullptr);
    ~AudioPlayerFrontend();
    AudioPlayer &audioPlayer;

private:
    Ui::AudioPlayerFrontend *ui;

    HorSliderSelfCentering seekSlider;
    bool songWasPlaying, durChanged;

signals:
    void Signal_Play();
    void Signal_Stop();
    void Signal_SongFinsihed();

private slots:
    void Slot_Seek(int _seek);
    void Slot_DurationChanged();
    void Slot_UpdatePlayerPos(qint64);

    void on_pushButton_BrowseSong_clicked();

    void on_pushButton_Pause_clicked();

public slots:

    void UpdateCurrentTime();
    void Slot_Play();
    void Slot_Pause();
};

#endif // AUDIOPLAYERFRONTEND_H
