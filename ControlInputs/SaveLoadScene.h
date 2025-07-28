#ifndef SAVELOADSCENE_H
#define SAVELOADSCENE_H

#include <QWidget>
#include <QStringListModel>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>

#include "Position.h"

namespace Ui {
class SaveLoadScene;
}

class SaveLoadScene : public QWidget
{
    Q_OBJECT

public:
    explicit SaveLoadScene(QWidget *parent = nullptr);
    ~SaveLoadScene();

    void AddPositionUi(Position* posUi){posUis.append(posUi);}

    void UpdateAvailableCurves();
    QStringListModel listModel;
    QStringList availableScenes;

    void Save(QString fileName) const;
    void Load(QString fileName) const;

    QString name;

private:
    Ui::SaveLoadScene *ui;
    QVector<Position*> posUis;

private slots:
    void Slot_Save();
    void Slot_SaveAll();
    void Slot_SceneSelected(int);
    void Slot_Load();
};

#endif // SAVELOADSCENE_H
