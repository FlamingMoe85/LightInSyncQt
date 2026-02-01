#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QTime>
#include <QTimer>

#include "../Utilities.hpp"

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    AudioPlayer();

    void Play(){player.play();};
    void Pause(){player.pause();};
    void SeekTo(int _ms){player.setPosition(_ms);};
    void SeekBy(int _ms){player.setPosition(GetCurrentPosAbs() + _ms);}
    int GetCurrentPosAbs(){return player.position();};
    float GetCurrentPosRel(){return (float)GetCurrentPosAbs() / (float)player.duration();};
    //void GetCurTime(QString &time);
    //void GetLength(QString &time);
    qint64 GetLength();
    qint64 GetPosition();
    //void MillisToTimeString(QString &_timeString, qint64 _millis);

private:
    QMediaPlayer player;

    //void millisToTime(int &h, int &m, int &s, int &ms, qint64 _millis);

    QTimer timer;

signals:
    void Signal_DurationChanged();
    void Signal_TimerExpired(qint64);

private slots:
    void Slot_DurationChanged();
    void Slot_TimerExpired();
};

#endif // AUDIOPLAYER_H
