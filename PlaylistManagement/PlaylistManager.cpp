#include "PlaylistManager.h"
#include "ui_PlaylistManager.h"

#include <QJsonDocument>
#include <QDirIterator>


const QString playlistPath = "C:/LightInSync/Qt/LightInSync/Playlists/";
const QString musicPath = "C:/Musik";

PlaylistManager::PlaylistManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaylistManager)
{
    ui->setupUi(this);
    ui->verticalLayout_SongSearch->addWidget(&fileBrowser);
    connect(&fileBrowser, SIGNAL(Signal_ClickedSong(QString)), this, SLOT(Slot_ClickedSong(QString)));
    ListPlaylists();
    currentSong = 0;
    playing = false;
    changedSongByUser = false;
}

PlaylistManager::~PlaylistManager()
{
    delete ui;
}

void PlaylistManager::ListPlaylists()
{
    QDir dir;
    dir.setPath(playlistPath);
    QStringList filter("*.json");
    QStringList availableScenes = dir.entryList(filter);

    ui->listWidget_ListOfPlaylists->clear();

    for(QString &s : availableScenes)
    {
        ui->listWidget_ListOfPlaylists->addItem(s);
    }

}

void PlaylistManager::Slot_ClickedSong(const QString _song)
{
    ui->listWidget_CurrentPlaylist->addItem(_song);
}

void PlaylistManager::on_pushButton_addPlaylist_clicked()
{
    QString name = ui->lineEditNewListName->text();
    if(name.isEmpty()) return;

    QString f = playlistPath + name + ".json";
    QFile saveFile(f);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
    }

    QJsonArray songArr;
    for(int i=0; i<ui->listWidget_CurrentPlaylist->count(); i++)
    {
        songArr.append(QJsonValue(ui->listWidget_CurrentPlaylist->item(i)->text()));
    }
    saveFile.write(QJsonDocument(songArr).toJson());
    ListPlaylists();
}

void PlaylistManager::on_listWidget_ListOfPlaylists_itemDoubleClicked(QListWidgetItem *item)
{
    QString fileName = playlistPath+item->text();
    ui->lineEditNewListName->setText(item->text().remove(".json"));
    QFile loadFile(fileName);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonArray jUiArr = loadDoc.array();
    for (const QJsonValue &v : jUiArr)
    {
        Slot_ClickedSong(v.toString());
    }
}

void PlaylistManager::on_listWidget_ListOfPlaylists_itemClicked(QListWidgetItem *item)
{

}

void PlaylistManager::on_listWidget_CurrentPlaylist_itemDoubleClicked(QListWidgetItem *item)
{
    //changedSongByUser = true;
    currentSong = ui->listWidget_CurrentPlaylist->row(item);
    HighlightSong(currentSong);
    AnnounceSongToPlay(item->text(), true);
}


void PlaylistManager::AnnounceSongToPlay(QString _song, bool changedByUser)
{
    QString songLoc = FindLocationOfSong(_song);
    Signal_PlaySequenceOfSong(songLoc, changedByUser);
}

QString PlaylistManager::FindLocationOfSong(QString _song)
{
    QString foundSong;
    QDirIterator it(musicPath, QStringList() << _song, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        foundSong = it.next();
        qDebug() << foundSong;
        break;
    }
    /*
    QDir dir;
    dir.setPath(musicPath);
    QStringList filter(_song);
    QStringList availableScenes = dir.entryList(filter);
    QString foundSong;

    for(QString &s : availableScenes)
    {
        foundSong = s;
        qDebug() << "Found Song:" << foundSong;
        break;
    }

    */
    return foundSong;
}

void PlaylistManager::on_pushButton_PlayPause_clicked()
{
    if(playing)
    {
        playing = false;
        Signal_Pause();
    }
    else
    {
        playing = true;
        AnnounceSongToPlay(GetSong(currentSong), true);
    }
}

QString PlaylistManager::GetSong(int _songNbr)
{
    QString retString;
    retString.clear();
    if(_songNbr >= ui->listWidget_CurrentPlaylist->count())return retString;
    ui->listWidget_CurrentPlaylist->setCurrentRow(_songNbr);
    retString = ui->listWidget_CurrentPlaylist->currentItem()->text();
    qDebug() << retString;
    return retString;
}

void PlaylistManager::HighlightSong(int _songNbr)
{
    ui->listWidget_CurrentPlaylist->setCurrentRow(_songNbr);
}

void PlaylistManager::on_pushButton_PrevSong_clicked()
{
    if(currentSong > 0)
    {
        //changedSongByUser = true;
        currentSong--;
        HighlightSong(currentSong);
        AnnounceSongToPlay(ui->listWidget_CurrentPlaylist->currentItem()->text(), true);
    }
}

void PlaylistManager::on_pushButton_NextSong_clicked()
{
    if(!(currentSong >= ui->listWidget_CurrentPlaylist->count()))
    {
        //changedSongByUser = true;
        currentSong++;
        HighlightSong(currentSong);
        AnnounceSongToPlay(ui->listWidget_CurrentPlaylist->currentItem()->text(), true);
    }
}

void PlaylistManager::Slot_SongFinished()
{
    if(!(currentSong >= ui->listWidget_CurrentPlaylist->count()))
    {
        //changedSongByUser = true;
        currentSong++;
        HighlightSong(currentSong);
        AnnounceSongToPlay(ui->listWidget_CurrentPlaylist->currentItem()->text(), false);
    }
    /*
    if(!changedSongByUser)
    {
        on_pushButton_NextSong_clicked();
    }
    changedSongByUser = false;
    */
}

void PlaylistManager::on_pushButton_clicked()
{
    if(currentSong >= ui->listWidget_CurrentPlaylist->count())return;
    delete ui->listWidget_CurrentPlaylist->takeItem(currentSong);
}


void PlaylistManager::on_pushButton_SaveList_clicked()
{

}

