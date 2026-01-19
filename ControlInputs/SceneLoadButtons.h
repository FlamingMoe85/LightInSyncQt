#ifndef SCENELOADBUTTONS_H
#define SCENELOADBUTTONS_H

#include <QStringList>
#include <QObject>

#include "ScrollAreaWithVertLayout.h"
#include "NameButton.h"

class SceneLoadButtons : public ScrollAreaWithVertLayout
{
    Q_OBJECT
public:
    SceneLoadButtons(QString _name = " ");


private:
    QString name;
    void UpdateAvailableSceneButtons();

    QStringList availableScenes;

signals:
    void Sig_NameClicked(const QString &_name);

private slots:
    void Slot_NameClicked(const QString &_name);

};

#endif // SCENELOADBUTTONS_H
