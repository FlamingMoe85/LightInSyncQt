#include "SceneLoadButtons.h"
#include <QDir>
#include <QPushButton>

#include <QDebug>

SceneLoadButtons::SceneLoadButtons(QString _name)
{
    name = _name;
    UpdateAvailableSceneButtons();
}

void SceneLoadButtons::UpdateAvailableSceneButtons()
{

    QDir dir;
    dir.setPath("C:/LightInSync/Qt/build-LightInSync-Desktop_Qt_5_15_2_MinGW_64_bit-Debug");
    QStringList filter("*" + name + ".json");
    availableScenes = dir.entryList(filter);

    for(QString &s : availableScenes)
    {
        NameButton* newNameNutton = new NameButton(this, s);
        AddToScrollArea(newNameNutton);
        QObject::connect(newNameNutton, &NameButton::Sig_SendName, this, &SceneLoadButtons::Slot_NameClicked);
    }
}

void SceneLoadButtons::Slot_NameClicked(const QString &_name)
{
    emit Sig_NameClicked(_name);
}
