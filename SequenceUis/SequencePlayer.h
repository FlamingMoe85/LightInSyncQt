#ifndef SEQUENCEPLAYER_H
#define SEQUENCEPLAYER_H

#include <QObject>
#include <QTimer>
#include <QTreeWidget>

#include "SequenceItem.h"

#include "../ControlInputs/SaveLoadScene.h"

enum SeqPlayerStates
{
    STOP,
    PLAY,
    PAUSE
};

class SequencePlayer : public QObject
{
    Q_OBJECT
public:
    SequencePlayer();

    void StartSequence(QList<SequenceItem*> *_listToPlay);
    void SetSequence(QList<SequenceItem*> *_listToPlay);
    void Start();
    void StopSecquence();
    void Pause();
    void SetSaveLoadSceneList(QList<SaveLoadScene*> *_saveLoadSceneList) {saveLoadSceneList = _saveLoadSceneList;}
    void SetTreeWdiget(QTreeWidget *_treeWidget){treeWidget = _treeWidget;}
    void ForwardToPosition(qint64 _position);
    bool PlaySequence(bool forward);

signals:
    void Signal_CurrentTime(const int);
    void Signal_Start();
    void Signal_Stop();

private slots:
    void Slot_TimerExpired();

private:
    QList<SequenceItem*> *listToPlay;
    qint64 time;
    int curSeqItem;
    QList<SaveLoadScene*> *saveLoadSceneList;
    QTreeWidget *treeWidget;

    void PlayCollection(QString _collection, qint64 _fadeIn);

    QTimer timer;

    SeqPlayerStates lastState;
};

#endif // SEQUENCEPLAYER_H
