#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <QWidget>
#include <QListWidgetItem>

#include "FileBrowser/window.h"

namespace Ui {
class PlaylistManager;
}

class PlaylistManager : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistManager(QWidget *parent = nullptr);
    ~PlaylistManager();

private:
    Ui::PlaylistManager *ui;

    Window fileBrowser;
    void ListPlaylists();
    QString FindLocationOfSong(QString _song);
    int currentSong;
    bool playing;
    QString GetSong(int _songNbr);
    void HighlightSong(int _songNbr);
    void AnnounceSongToPlay(QString _song, bool changedByUser);
    bool changedSongByUser;

signals:
    void Signal_PlaySequenceOfSong(const QString &_song, const bool changedByUser);
    void Signal_Pause();

public slots:
    void Slot_SongFinished();


private slots:
    void Slot_ClickedSong(const QString);
    void on_pushButton_addPlaylist_clicked();
    void on_listWidget_ListOfPlaylists_itemClicked(QListWidgetItem *item);
    void on_listWidget_ListOfPlaylists_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_CurrentPlaylist_itemDoubleClicked(QListWidgetItem *item);
    void on_pushButton_PlayPause_clicked();
    void on_pushButton_PrevSong_clicked();
    void on_pushButton_NextSong_clicked();
    void on_pushButton_clicked();
    void on_pushButton_SaveList_clicked();
};

#endif // PLAYLISTMANAGER_H
