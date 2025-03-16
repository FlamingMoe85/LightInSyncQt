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

private:
    Ui::AudioPlayerFrontend *ui;
    AudioPlayer &audioPlayer;
    HorSliderSelfCentering seekSlider;

private slots:
    void Slot_Play();
    void Slot_Pause();
    void Slot_Seek(int _seek);
    void Slot_DurationChanged();

public slots:

    void UpdateCurrentTime();
};

#endif // AUDIOPLAYERFRONTEND_H
