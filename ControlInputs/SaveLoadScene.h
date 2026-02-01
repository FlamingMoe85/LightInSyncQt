#ifndef SAVELOADSCENE_H
#define SAVELOADSCENE_H

#include <QWidget>
#include <QStringListModel>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QTimer>

#include "Position.h"

namespace Ui {
class SaveLoadScene;
}

class SaveLoadScene : public QWidget
{
    Q_OBJECT

public:
    explicit SaveLoadScene(QWidget *parent = nullptr, QString _name = " ");
    ~SaveLoadScene();

    void AddPositionUi(Position* posUi){posUis.append(posUi);}
    Position* GetPosUi(int _sel){return posUis[_sel];}
    bool OverrideEnabled();

    void UpdateAvailableCurves();
    QStringListModel listModel;
    QStringList availableScenes;

    bool LoadByNameAndFadeIn(QString &fileName, qint64 _fadeIn);
    void Save(QString fileName) const;
    bool Load(QString fileName, qint64 _fadeIn) const;
    void SetFadeIn(qint64 _fadeIn);

    QString name;
    SaveLoadScene* slaveSaveLoadScene;

private:
    Ui::SaveLoadScene *ui;
    QVector<Position*> posUis;
    QTimer sliderPingTimer;

public slots:
    void Slot_LoadByName(const QString &_name);

private slots:
    void Slot_Save();
    void Slot_SaveAll();
    void Slot_SceneSelected(int);
    void Slot_Load();
    void Slot_SliderPing();

};

#endif // SAVELOADSCENE_H
