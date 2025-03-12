#ifndef EFFECTEDITOR_H
#define EFFECTEDITOR_H

#include <QWidget>
using std::vector;
#include <vector>
#include <QKeyEvent>

#include "EditorItem.h"
#include "CustomScrollArea.h"
#include "ShadeWidget.h"
#include "../../../share/FunctionOwners.hpp"

namespace Ui {
class EffectEditor;
}

class EffectEditor : public QWidget
{
    Q_OBJECT

public:




    explicit EffectEditor(QWidget *parent = nullptr);
    ~EffectEditor();
    void SetBundSerMangr(vector<vector<FunctionOwners *> > &funcContainerContainers);
    void DrawPositionLine(float _pos);


    void keyPressEvent(QKeyEvent *event) override;

protected:

    CustomScrollArea scrollArea;


private:
    Ui::EffectEditor *ui;

    QVector<ShadeWidget*> shadeWidgets;
    QVector<EditorItem*> editorItems;
    void RemoveAllShadeWidgets();
    QPolygonF copiedPoints;
    int activeWidget, pasteWidget;

private slots:

    void Slot_colorsChanged(QPolygonF &points, int index);
    void Slot_SignalMouseRelease(QPolygonF &points, int index);
    void Slot_GetActivityNote(int);
    void Slot_PasteHere(int);
    void Slot_CopyX();
    void Slot_CopyY();
    void Slot_CopyPoint();
};

#endif // EFFECTEDITOR_H
