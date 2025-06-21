#ifndef EFFECTEDITOR_H
#define EFFECTEDITOR_H

#include <QWidget>
using std::vector;
#include <vector>
#include <QKeyEvent>
#include <QStringListModel>

#include "EditorItem.h"
#include "CustomScrollArea.h"
#include "ShadeWidget.h"
#include "../../../share/Mapper/Mapper_Base.hpp"

namespace Ui {
class EffectEditor;
}

class EffectEditor : public QWidget
{
    Q_OBJECT

public:




    explicit EffectEditor(int x, int y, int w, int h, QWidget *parent = nullptr);
    ~EffectEditor();
    void SetBundSerMangr(vector<vector<Mapper_Base *> > &funcContainerContainers);
    void DrawPositionLine(float _pos);


    void keyPressEvent(QKeyEvent *event) override;
    void SetItemGeometry(int x, int y, int w, int h);
    void SetWidth(int _w);

    void UpdateAvailableCurves();

    void Save(QString fileName) const
    {
        QString f = fileName += ".json";
        QFile saveFile(f);

        if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open save file.");
        }

        QJsonArray saveObject;
        for(ShadeWidget* sW : shadeWidgets)
        {
            saveObject.append(sW->hoverPoints()->toJson());
        }
        saveFile.write(QJsonDocument(saveObject).toJson());
    }

    void Load(QString fileName) const
    {
        QFile loadFile(fileName);

        if (!loadFile.open(QIODevice::ReadOnly)) {
            return;
        }

        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

        QJsonArray loadArr = loadDoc.array();
        int i=0;

        for(ShadeWidget *sW : shadeWidgets)
        {
            sW->hoverPoints()->ClearPoints();
        }

        for (const QJsonValue &v : loadArr)
        {
            shadeWidgets[i++]->hoverPoints()->FromJson(v.toObject());
        }
    }

protected:

    CustomScrollArea scrollArea;


private:
    Ui::EffectEditor *ui;

    QVector<ShadeWidget*> shadeWidgets;
    QVector<EditorItem*> editorItems;
    void RemoveAllShadeWidgets();
    QPolygonF copiedPoints;
    int activeWidget, pasteWidget;
    QRect geometry;
    int opMode;
    QStringListModel listModel;
    QStringList availableCurves;

private slots:

    void Slot_colorsChanged(QPolygonF &points, int index);
    void Slot_SignalMouseRelease(QPolygonF &points, int index);
    void Slot_GetActivityNote(int);
    void Slot_PasteHere(int);
    void Slot_CopyX();
    void Slot_CopyY();
    void Slot_CopyPoint();
    void Slot_DeletePoint();
    void Slot_SetX();
    void Slot_SetY();
    void Slot_SetBoth();
    void Slot_Save();
    void Slot_CurveSelected(int);
};

#endif // EFFECTEDITOR_H
