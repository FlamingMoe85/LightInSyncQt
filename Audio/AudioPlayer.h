#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QTime>

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
    void GetCurTime(QTime &time);
    void GetLength(QTime &time);

private:
    QMediaPlayer player;

    void millisToTime(QTime &time, qint64 _millis);

signals:
    void Signal_DurationChanged();

private slots:
    void Slot_DurationChanged();
};

#endif // AUDIOPLAYER_H
