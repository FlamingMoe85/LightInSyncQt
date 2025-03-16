#include "AudioPlayer.h"

AudioPlayer::AudioPlayer()
{
    player.setParent(this);
    player.setMedia(QUrl::fromLocalFile("C:/Users/fischeju/Music/Music/Branko - Lost Arps.mp3"));
    player.setVolume(50);
    Play();
    QObject::connect(&player, SIGNAL(durationChanged(qint64)), this, SLOT(Slot_DurationChanged()));
}

void AudioPlayer::millisToTime(QTime &_time, qint64 _millis)
{
    qint64 h, m, s, ms;

    ms = _millis % 1000;
    _millis -= ms;

    s = _millis % (60*1000);
    _millis -= s;
    s = s/1000;

    m = _millis % (60*60*1000);
    _millis -= m;
    m = m/(60*1000);

    h = _millis / (60*60*1000);

    _time.setHMS(h, m, s, ms);
}

void AudioPlayer::GetLength(QTime &time)
{
    millisToTime(time, player.duration());
}

void AudioPlayer::GetCurTime(QTime &time)
{
    millisToTime(time, GetCurrentPosAbs());
}

void AudioPlayer::Slot_DurationChanged()
{
    Signal_DurationChanged();
}
