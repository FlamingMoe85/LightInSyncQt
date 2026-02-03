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

    void SetSong(QString &_song, bool _changedByUSer);
    QString GetSong();
    void Play(){changedByUser = true; player.play();};
    void Pause(){changedByUser = true; player.pause();};
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
    QString currentSong;
    QTimer timer;
    QMediaPlayer::State oldPlayerState;
    bool changedByUser;

signals:
    void Signal_DurationChanged();
    void Signal_TimerExpired(qint64);
    void Signal_SongFinsihed();

private slots:
    void Slot_DurationChanged();
    void Slot_TimerExpired();
    void Slot_PlayerSTateChanged(QMediaPlayer::State);
};

#endif // AUDIOPLAYER_H
