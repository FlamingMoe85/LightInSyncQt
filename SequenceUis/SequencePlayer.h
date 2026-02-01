#ifndef SEQUENCEPLAYER_H
#define SEQUENCEPLAYER_H

#include <QObject>
#include <QTimer>
#include <QTreeWidget>

#include "SequenceItem.h"

#include "../ControlInputs/SaveLoadScene.h"

class SequencePlayer : public QObject
{
    Q_OBJECT
public:
    SequencePlayer();

    void PlaySequence(QList<SequenceItem*> *_listToPlay);
    void StopSecquence();
    void SetSaveLoadSceneList(QList<SaveLoadScene*> *_saveLoadSceneList) {saveLoadSceneList = _saveLoadSceneList;}
    void SetTreeWdiget(QTreeWidget *_treeWidget){treeWidget = _treeWidget;}

    QTimer timer;

private slots:
    void Slot_TimerExpired();

private:
    QList<SequenceItem*> *listToPlay;
    qint64 time;
    int curSeqItem;
    QList<SaveLoadScene*> *saveLoadSceneList;
    QTreeWidget *treeWidget;

    void PlayCollection(QString _collection, qint64 _fadeIn);
};

#endif // SEQUENCEPLAYER_H
