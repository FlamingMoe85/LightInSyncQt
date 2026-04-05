#ifndef SEQUENCEEDITOR
#define SEQUENCEEDITOR

#include <QWidget>
#include <QTreeWidgetItem>
#include <QDir>
#include <QMap>

#include "../Audio/AudioPlayerFrontend.h"
#include "SequencePlayer.h"
#include "SequenceItem.h"
#include "../ControlInputs/ScrollAreaWithVertLayout.h"

namespace Ui {
class SequenceEditor;
}

class SequenceEditor : public QWidget
{
    Q_OBJECT

public:
    explicit SequenceEditor(QWidget *parent = nullptr);
    ~SequenceEditor();

    void DisplayCurrentTargetFromSeqItem();
    void SetSaveLoadSceneList(QList<SaveLoadScene*> *_saveLoadSceneList) {sequencePlayer.SetSaveLoadSceneList(_saveLoadSceneList);}

signals:
    void Signal_AudioPlayerFinished();

public slots:
    void Slot_AddByName(const QString &_name);
    void Slot_SongFromPlayList(const QString &_song, const bool _changedByUser);

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_AddSceneToCollection_clicked();

    void on_pushButton_RemoveSceneFromCollection_clicked();

    void on_treeWidget_Collections_itemClicked(QTreeWidgetItem *item, int column);


    void on_treeWidget_Collections_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void Slot_ReceiveTimeChangeTarget(SequenceItem*);

    void on_spinBox_Minute_valueChanged(int arg1);

    void on_spinBox_Second_valueChanged(int arg1);

    void on_spinBox_Milliseconds_valueChanged(int arg1);

    void on_pushButton_SortList_clicked();

    void on_pushButton_playStopSequence_clicked();

    void on_pushButton_Save_clicked();

    void on_pushButton_Load_clicked();

    void on_pushButton_clicked();

    void on_pushButton_NewSequence_clicked();

    void on_pushButton_useSongName_clicked();

    void on_pushButton_OpenSequence_clicked();

    void on_pushButton_Forward_clicked();

    void Slot_PlayerStarted();
    void Slot_PlayerStoped();
    void Slot_SeqPlayerStart();
    void Slot_SeqPlayerStop();
    void Slot_SeqPlayerTime(const int);

    void Slot_AudioPlayerFinished();

private:
    Ui::SequenceEditor *ui;

    void AddCollection(QString _name);

    QTreeWidgetItem* markedItem;
    int markedColoumn;

    ScrollAreaWithVertLayout sequenceArea;

    AudioPlayerFrontend *playerFrontend;
    SequenceItem* currentTimeChangeTarget;
    QList<SequenceItem*> sequenceItemsList;
    void SetTimeOnTargetOfSeqItem();

    SequencePlayer sequencePlayer;
    bool sequencePlayerRuns;

    void SaveCollections();
    void SaveSequence();

    void LoadCollections();
    void ClearCollection();
    void LoadSequence(QString &seqPath, bool _changedByUser);

    void RemoveSequenceItem(SequenceItem* _remItem);
    void RemoveAllSequenceItemsFromUi();
    void ClearAllSequenceItems();
    void AddSequenceItemsFromList();

    QString SequencePath, sequenceSong;

    void FindAllSequences();
    QMap<QString, QString> songToSequenceFileMap;

    int currentSong;
    bool songHasSequence;

};

#endif // SEQUENCEEDITOR
