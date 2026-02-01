#include "SequencePlayer.h"

#include <QDebug>

SequencePlayer::SequencePlayer()
{
    saveLoadSceneList = nullptr;
    treeWidget = nullptr;
    QObject::connect(&timer, &QTimer::timeout, this, &SequencePlayer::Slot_TimerExpired);
    timer.setInterval(10);
}

void SequencePlayer::PlayCollection(QString _collection, qint64 _fadeIn)
{
    qDebug() << "Collection to play: " << _collection;
    if(treeWidget == nullptr) return;
    QList<QTreeWidgetItem*> clist = treeWidget->findItems(_collection, Qt::MatchContains, 0);
    if(clist.count() == 0)return;

    QString scene;
    for(int i=0; i<clist.at(0)->childCount(); i++)
    {
        scene = clist.at(0)->child(i)->text(0);
        qDebug() << "Scene to play: " << scene;
        for(SaveLoadScene* s : *saveLoadSceneList)
        {
            qDebug() << "Scene Loaded: " << s->Load(scene,_fadeIn/10);
        }
    }

    qDebug() << " ";
}


void SequencePlayer::Slot_TimerExpired()
{
    while(true)
    {
        if(listToPlay->at(curSeqItem)->GetTimeStamp() <= time)
        {
            PlayCollection(listToPlay->at(curSeqItem)->GetCollection(), listToPlay->at(curSeqItem)->GetFadeIn());
            curSeqItem++;
            if(curSeqItem == listToPlay->count())
            {
                StopSecquence();
                break;
            }
        }
        else
        {
            break;
        }
    }

    time += 10;
}

void SequencePlayer::StopSecquence()
{
    QObject::disconnect(&timer, &QTimer::timeout, this, &SequencePlayer::Slot_TimerExpired);
    timer.stop();
}

void SequencePlayer::PlaySequence(QList<SequenceItem*> *_listToPlay)
{
    time = 0;
    curSeqItem = 0;
    listToPlay = _listToPlay;
    QObject::connect(&timer, &QTimer::timeout, this, &SequencePlayer::Slot_TimerExpired);
    timer.start();
}
