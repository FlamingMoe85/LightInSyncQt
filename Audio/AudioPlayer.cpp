#include "AudioPlayer.h"


AudioPlayer::AudioPlayer()
{
    player.setParent(this);
    player.setMedia(QUrl::fromLocalFile("C:/Musik/Zouk/Fly On.mp3"));
    player.setVolume(20);
    //Play();
    QObject::connect(&player, SIGNAL(durationChanged(qint64)), this, SLOT(Slot_DurationChanged()));
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(Slot_TimerExpired()));
    timer.setInterval(10);
    timer.start();
}

void AudioPlayer::Slot_TimerExpired()
{
    emit Signal_TimerExpired(player.position());
}

/*
void AudioPlayer::millisToTime(int &h, int &m, int &s, int &ms, qint64 _millis)
{

    ms = _millis % 1000;
    _millis -= ms;

    s = _millis % (60*1000);
    _millis -= s;
    s = s/1000;

    m = _millis % (60*60*1000);
    _millis -= m;
    m = m/(60*1000);

    h = _millis / (60*60*1000);

}

void AudioPlayer::MillisToTimeString(QString &_timeString, qint64 _millis)
{
    Utilities::
    int h,m,s,ms;
    _timeString.clear();
    millisToTime(h,m,s,ms, _millis);

    if(h<10) _timeString = "0";
    _timeString += QString::number(h);
    _timeString += ":";

    if(m<10) _timeString += "0";
    _timeString += QString::number(m);
    _timeString += ":";

    if(s<10) _timeString += "0";
    _timeString += QString::number(s);
    _timeString += ":";

    if(ms<100) _timeString += "0";
    if(ms<10) _timeString += "0";
    _timeString += QString::number(ms);
}


void AudioPlayer::GetLength(QString &time)
{
    MillisToTimeString(time, player.duration());
}
*/
qint64 AudioPlayer::GetLength()
{
    return player.duration();
}

qint64 AudioPlayer::GetPosition()
{
    return player.position();
}
/*
void AudioPlayer::GetCurTime(QString &time)
{
    MillisToTimeString(time, GetCurrentPosAbs());
}
*/

void AudioPlayer::Slot_DurationChanged()
{
    Signal_DurationChanged();
}
