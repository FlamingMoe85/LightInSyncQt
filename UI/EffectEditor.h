#ifndef EFFECTEDITOR_H
#define EFFECTEDITOR_H

#include <QWidget>
using std::vector;
#include <vector>

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

protected:

    CustomScrollArea scrollArea;

private:
    Ui::EffectEditor *ui;

    QVector<ShadeWidget*> shadeWidgets;
    QVector<EditorItem*> editorItems;
    void RemoveAllShadeWidgets();

private slots:

    void Slot_colorsChanged(QPolygonF &points, int index);
    void Slot_SignalMouseRelease(QPolygonF &points, int index);
};

#endif // EFFECTEDITOR_H
