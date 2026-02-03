#include "SequenceEditor.h"
#include "ui_SequenceEditor.h"

#include <QDebug>
#include <QFileDialog>
#include <QDirIterator>

const QString SequenceBasePath = "C:/LightInSync/Qt/LightInSync/SequenceDatas/";
const QString SequenceCollectionsPath = SequenceBasePath + "Collections/";
const QString SequencesPathConst = SequenceBasePath + "Sequences/";

SequenceEditor::SequenceEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SequenceEditor)
{
    ui->setupUi(this);
    markedItem = nullptr;

    AudioPlayer *player = new AudioPlayer();
    playerFrontend = new AudioPlayerFrontend((*player), this);
    ui->verticalLayout_Player->layout()->addWidget(playerFrontend);
    ui->verticalLayout_Sequence->layout()->addWidget(&sequenceArea);
    currentTimeChangeTarget = nullptr;
    sequencePlayerRuns = false;
    sequencePlayer.SetTreeWdiget(ui->treeWidget_Collections);
    SequencePath = SequencesPathConst;
    sequenceSong.clear();
    LoadCollections();
    connect(playerFrontend, SIGNAL(Signal_Play()), this, SLOT(Slot_PlayerStarted()));
    connect(&playerFrontend->audioPlayer, SIGNAL(Signal_SongFinsihed()), this, SLOT(Slot_AudioPlayerFinished()));
    FindAllSequences();
    currentSong = 0;
    songHasSequence = false;
}

SequenceEditor::~SequenceEditor()
{
    delete ui;
}

void SequenceEditor::DisplayCurrentTargetFromSeqItem()
{
    if(currentTimeChangeTarget != nullptr)
    {
        int h, m, s, ms;
        Utilities::millisToH_M_S_MS(h, m, s, ms, currentTimeChangeTarget->GetTimeFromCurrentTarget());
        ui->spinBox_Minute->setValue(m);
        ui->spinBox_Second->setValue(s);
        ui->spinBox_Milliseconds->setValue(ms);
    }
}

void SequenceEditor::SetTimeOnTargetOfSeqItem()
{
    if(currentTimeChangeTarget != nullptr)
    {
        currentTimeChangeTarget->SetTimeForCurrentTarget(Utilities::m_s_ms_ToMilliseconds(  ui->spinBox_Minute->value(),
                                                                                            ui->spinBox_Second->value(),
                                                                                            ui->spinBox_Milliseconds->value()));
    }
}

/**/
void SequenceEditor::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{

}




void SequenceEditor::on_pushButton_AddSceneToCollection_clicked()
{
    AddCollection(ui->plainTextEdit_Collection->toPlainText());
}

void SequenceEditor::AddCollection(QString _name)
{
    QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(
                ui->treeWidget_Collections,
                QStringList() << _name);
    topLevelItem->setFirstColumnSpanned(true);
    markedItem = topLevelItem;
    markedColoumn = 0;
}

void SequenceEditor::on_pushButton_RemoveSceneFromCollection_clicked()
{
    if(markedItem != nullptr)
    {
        ui->treeWidget_Collections->removeItemWidget(markedItem, markedColoumn);
        delete markedItem;
        markedItem = nullptr;
    }
}


void SequenceEditor::on_treeWidget_Collections_itemClicked(QTreeWidgetItem *item, int column)
{
    markedItem = item;
    markedColoumn = column;
}

void SequenceEditor::Slot_AddByName(const QString &_name)
{
    if((markedItem != nullptr) && (markedColoumn == 0))
    {
        QTreeWidgetItem *scene = new QTreeWidgetItem();
        scene->setText(0, _name);
        ui->treeWidget_Collections->topLevelItem(ui->treeWidget_Collections->indexOfTopLevelItem(markedItem))->addChild(scene);
        //ui->treeWidget_Collections->topLevelItem(0)->addChild(scene);
    }
}

void SequenceEditor::on_treeWidget_Collections_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if((markedItem != nullptr) && (markedColoumn == 0))
    {
        SequenceItem* seqItm = new SequenceItem(&sequenceArea, item->text(0), playerFrontend->audioPlayer.GetPosition() , 1000);
        sequenceArea.AddToScrollArea(seqItm);
        connect(seqItm, SIGNAL(Signal_ConnectForTimeChange(SequenceItem*)), this, SLOT(Slot_ReceiveTimeChangeTarget(SequenceItem*)));
        sequenceItemsList.append(seqItm);
    }
}

void SequenceEditor::Slot_ReceiveTimeChangeTarget(SequenceItem* _target)
{
    currentTimeChangeTarget = _target;
    if(currentTimeChangeTarget->GetTimeFromCurrentTarget() != 2)
    {
        DisplayCurrentTargetFromSeqItem();
    }
}

void SequenceEditor::on_spinBox_Minute_valueChanged(int arg1)
{
    SetTimeOnTargetOfSeqItem();
}


void SequenceEditor::on_spinBox_Second_valueChanged(int arg1)
{
    SetTimeOnTargetOfSeqItem();
}

void SequenceEditor::on_spinBox_Milliseconds_valueChanged(int arg1)
{
    SetTimeOnTargetOfSeqItem();
}

void SequenceEditor::on_pushButton_SortList_clicked()
{
    SequenceItem::SortSequenceItems(sequenceItemsList);
    RemoveAllSequenceItemsFromUi();
    AddSequenceItemsFromList();
}


void SequenceEditor::on_pushButton_playStopSequence_clicked()
{
    if(sequencePlayerRuns)
    {
        sequencePlayer.StopSecquence();
        sequencePlayerRuns = false;
    }
    else
    {
        sequencePlayer.StartSequence(&sequenceItemsList);
        sequencePlayerRuns = true;
    }

}


void SequenceEditor::on_pushButton_Save_clicked()
{
    SaveCollections();
    SaveSequence();
}

void SequenceEditor::SaveCollections()
{
    QString f = SequenceCollectionsPath + "Collections" + ".json";
    QFile saveFile(f);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
    }

    QJsonArray collectionArr;
    for(int i=0; i<ui->treeWidget_Collections->topLevelItemCount(); i++)
    {
        QJsonObject collectionObj;
        QJsonArray sceneArr;

        collectionObj["Collection"] = ui->treeWidget_Collections->topLevelItem(i)->text(0);

        for(int j=0; j<ui->treeWidget_Collections->topLevelItem(i)->childCount(); j++)
        {
            QJsonValue scene(ui->treeWidget_Collections->topLevelItem(i)->child(j)->text(0));
            sceneArr.append(scene);
        }
        collectionObj["Scenes"] = sceneArr;
        //json.append()
        collectionArr.append(collectionObj);
    }
    saveFile.write(QJsonDocument(collectionArr).toJson());
}

void SequenceEditor::LoadCollections()
{
    QString f = SequenceCollectionsPath + "Collections" + ".json";
    QFile loadFile(f);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        return;
    }
    ClearCollection();

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonArray collectionArray = loadDoc.array();

    for (const QJsonValue &v : collectionArray)
    {
        QJsonObject collectionObj = v.toObject();
        AddCollection(collectionObj["Collection"].toString());
        //markedItem = ui->treeWidget_Collections->takeTopLevelItem(ui->treeWidget_Collections->topLevelItemCount()-1);
        //markedColoumn = 0;
        QJsonArray sceneArr = collectionObj["Scenes"].toArray();
        for (const QJsonValue &v : sceneArr)
        {
            Slot_AddByName(v.toString());
        }
    }
}

void SequenceEditor::ClearCollection()
{
    ui->treeWidget_Collections->clear();
}

void SequenceEditor::SaveSequence()
{
    //QString f = SequencesPathConst + "Sequence" + ".json";
    QFile saveFile(SequencePath);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
    }

    QJsonObject sequenceObject;
    sequenceObject["Song"] = sequenceSong;
    QJsonArray sequenceItemArr;
    for(int i=0; i<sequenceItemsList.count(); i++)
    {
        sequenceItemsList.at(i)->Save(sequenceItemArr);
    }
    sequenceObject["SequenceItems"] = sequenceItemArr;
    saveFile.write(QJsonDocument(sequenceObject).toJson());
}


void SequenceEditor::LoadSequence(QString &seqPath, bool _changedByUser)
{
    QString fileName = seqPath;
    SequencePath = fileName;
    QFile loadFile(seqPath);

    ClearAllSequenceItems();

    if (!loadFile.open(QIODevice::ReadOnly)) {
        return;
    }

    fileName.remove(".json");
    QStringList pathParts = fileName.split("/");
    ui->lineEdit_SequenceName->setText(pathParts.last());

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject sequenceObj = loadDoc.object();

    QString seqSong = sequenceObj["Song"].toString();
    if(!seqSong.isEmpty())
    {
        playerFrontend->audioPlayer.SetSong(seqSong, _changedByUser);
    }
            //QJsonArray sequenceArray = loadDoc.array();
    QJsonArray sequenceArray = sequenceObj["SequenceItems"].toArray();
    for (const QJsonValue &v : sequenceArray)
    {
        QJsonObject collectionObj = v.toObject();

        SequenceItem* seqItm = new SequenceItem(&sequenceArea,
                                                collectionObj["Collection"].toString(),
                                                collectionObj["TimeStamp"].toString().toInt(),
                                                collectionObj["FadeIn"].toString().toInt());
        sequenceArea.AddToScrollArea(seqItm);
        connect(seqItm, SIGNAL(Signal_ConnectForTimeChange(SequenceItem*)), this, SLOT(Slot_ReceiveTimeChangeTarget(SequenceItem*)));
        sequenceItemsList.append(seqItm);
    }

    sequencePlayer.SetSequence(&sequenceItemsList);
    songHasSequence = true;
}

void SequenceEditor::on_pushButton_Load_clicked()
{
    LoadCollections();
    LoadSequence(SequencePath, true);
}

void SequenceEditor::RemoveSequenceItem(SequenceItem* _remItem)
{
    SequenceItem* toDelete;
    sequenceArea.RemoveFromScrollArea(_remItem);
    disconnect(_remItem, SIGNAL(Signal_ConnectForTimeChange(SequenceItem*)), this, SLOT(Slot_ReceiveTimeChangeTarget(SequenceItem*)));
    toDelete = sequenceItemsList.takeAt(sequenceItemsList.indexOf(_remItem));
    delete toDelete;
}

void SequenceEditor::RemoveAllSequenceItemsFromUi()
{
    sequenceArea.RemoveAllWidgetsFromLayout();
}

void SequenceEditor::ClearAllSequenceItems()
{
    while(sequenceItemsList.count() > 0)
    {
        RemoveSequenceItem(sequenceItemsList.last());
    }
}

void SequenceEditor::AddSequenceItemsFromList()
{
    for(SequenceItem* si : sequenceItemsList)
    {
        sequenceArea.AddToScrollArea(si);
    }
}

void SequenceEditor::on_pushButton_clicked()
{
    if((currentTimeChangeTarget != nullptr) && (currentTimeChangeTarget->CurrentTarget() == 2))
    {
        RemoveSequenceItem(currentTimeChangeTarget);
    }
}


void SequenceEditor::on_pushButton_NewSequence_clicked()
{
    QDir dir;
    QString p = SequencesPathConst+ui->lineEdit_SequenceName->text();
    if(ui->lineEdit_SequenceName->text().isEmpty()) return;
    if(QFile::exists(p+".json")) return;
    dir.mkdir(p);
    SequencePath = SequencesPathConst+ui->lineEdit_SequenceName->text()+".json";
}

void SequenceEditor::on_pushButton_useSongName_clicked()
{
   sequenceSong = playerFrontend->audioPlayer.GetSong();
   ui->lineEdit_SequenceName->setText(sequenceSong);
}

void SequenceEditor::on_pushButton_OpenSequence_clicked()
{
    QString selectedSequence =
        QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, tr("Select File"), QDir::currentPath()));
    selectedSequence.replace("\\", "/");
    qDebug() << selectedSequence;
    LoadCollections();
    LoadSequence(selectedSequence, true);
}


void SequenceEditor::on_pushButton_Forward_clicked()
{
    sequencePlayer.ForwardToPosition(playerFrontend->audioPlayer.GetPosition());
}

void SequenceEditor::Slot_PlayerStarted()
{
    if(songHasSequence)
    {
        sequencePlayer.StopSecquence();
        sequencePlayer.Start();
    }
}

void SequenceEditor::Slot_SongFromPlayList(const QString &_song, const bool _changedByUser)
{
    QMap<QString, QString>::iterator it;
    QString file = songToSequenceFileMap[_song];
    QString song = _song;
    if(file.isEmpty())
    {
        songHasSequence = false;
        //playerFrontend->Slot_Pause();
        playerFrontend->audioPlayer.SetSong(song, _changedByUser);
        playerFrontend->Slot_Play();
    }
    else
    {
        songHasSequence = true;
        for (it = songToSequenceFileMap.begin(); it != songToSequenceFileMap.end(); ++it) {
            // Format output here.
            if(it.key().contains(_song))
            {
                LoadSequence(file, _changedByUser);
                //playerFrontend->Slot_Pause();
                //playerFrontend->audioPlayer.SetSong(song, _changedByUser);
                sequencePlayer.StartSequence(&sequenceItemsList);
                playerFrontend->Slot_Play();
                break;
            }
        }
    }

}

void SequenceEditor::FindAllSequences()
{
    QDirIterator it(SequencesPathConst, QStringList() << "*.json", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QString file = it.next();
        QFile loadFile(file);

        if (!loadFile.open(QIODevice::ReadOnly)) {
            return;
        }

        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        QJsonObject sequenceObj = loadDoc.object();

        QString seqSong = sequenceObj["Song"].toString();

        songToSequenceFileMap[seqSong] = file;
    }

}

void SequenceEditor::Slot_AudioPlayerFinished()
{
    Signal_AudioPlayerFinished();
}
