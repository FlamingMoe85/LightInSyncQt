#include "SequencePlayer.h"

#include <QDebug>

SequencePlayer::SequencePlayer()
{
    saveLoadSceneList = nullptr;
    treeWidget = nullptr;
    QObject::connect(&timer, &QTimer::timeout, this, &SequencePlayer::Slot_TimerExpired);
    timer.setInterval(10);
    lastState = SeqPlayerStates::STOP;
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
    PlaySequence(false);
}

bool SequencePlayer::PlaySequence(bool forward)
{
    while(true)
    {
        if(listToPlay->count() > 0 &&  listToPlay->at(curSeqItem)->GetTimeStamp() <= time)
        {
            if(forward)
            {
                PlayCollection(listToPlay->at(curSeqItem)->GetCollection(), 20000);
            }
            else
            {
                PlayCollection(listToPlay->at(curSeqItem)->GetCollection(), listToPlay->at(curSeqItem)->GetFadeIn());
            }

            curSeqItem++;
            if(curSeqItem == listToPlay->count())
            {
                StopSecquence();
                return true;
                break;
            }
        }
        else
        {
            break;
        }
    }

    time += 10;
    Signal_CurrentTime(time);
    return false;
}


void SequencePlayer::StopSecquence()
{
    //QObject::disconnect(&timer, &QTimer::timeout, this, &SequencePlayer::Slot_TimerExpired);
    timer.stop();
    Signal_Stop();
    lastState = SeqPlayerStates::STOP;
}

void SequencePlayer::Pause()
{
    timer.stop();
    Signal_Stop();
    lastState = SeqPlayerStates::PAUSE;
}

void SequencePlayer::StartSequence(QList<SequenceItem*> *_listToPlay)
{
    time = 0;
    curSeqItem = 0;
    listToPlay = _listToPlay;
    //QObject::connect(&timer, &QTimer::timeout, this, &SequencePlayer::Slot_TimerExpired);
    timer.start();
    Signal_Start();
    lastState = SeqPlayerStates::PLAY;
}

void SequencePlayer::Start()
{
    if(lastState == 0)
    {
        time = 0;
        curSeqItem = 0;
    }
    //QObject::connect(&timer, &QTimer::timeout, this, &SequencePlayer::Slot_TimerExpired);
    timer.start();
    Signal_Start();
    lastState = SeqPlayerStates::PLAY;
}

void SequencePlayer::SetSequence(QList<SequenceItem*> *_listToPlay)
{
    listToPlay = _listToPlay;
}

void SequencePlayer::ForwardToPosition(qint64 _position)
{
    timer.stop();
    _position = _position;
    time = 0;
    curSeqItem = 0;

    while(time < _position)
    {
        if(PlaySequence(true))break;
    }

    for(SaveLoadScene* s : *saveLoadSceneList)
    {
        s->SetFadeIn(10);
    }
}
