#include "EffectEditor.h"
#include "ui_EffectEditor.h"


EffectEditor::EffectEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EffectEditor)
{
    ui->setupUi(this);

    ui->verticalLayout_ScrollArea->addWidget(&scrollArea);
}

EffectEditor::~EffectEditor()
{
    delete ui;
}

void EffectEditor::SetBundSerMangr(vector<vector<FunctionOwners*>> &funcContainerContainers)
{
    RemoveAllShadeWidgets();
    shadeWidgets.resize(funcContainerContainers.size());
    //editorItems.resize(funcContainerContainers.size());
    int i=0;
    for(vector<FunctionOwners*> &funcOwnVec :  funcContainerContainers)
    {
        /*
        editorItems[i] = new EditorItem((QWidget*)this);
        ShadeWidget* shadeWidget = new ShadeWidget(ShadeWidget::RedShade, (QWidget*)editorItems[i], funcOwnVec);
        QString title(QString::fromStdString(*(funcOwnVec[0]->GetType())));
        editorItems[i]->SetTitle(title);
        editorItems[i]->SetWidget(shadeWidget);
        scrollArea.AddWidget(editorItems[i]);
        */

        ShadeWidget* shadeWidget = new ShadeWidget(ShadeWidget::RedShade, (QWidget*)shadeWidget, funcOwnVec);
        shadeWidgets[i] = shadeWidget;
        scrollArea.AddWidget(shadeWidget);
        i++;
        //connect(shadeWidgets[i], SIGNAL(Signal_colorsChanged(const QPolygonF &points,int index)), this, SLOT(Slot_colorsChanged(const QPolygonF &points, int index)));
        //connect(shadeWidgets[i], SIGNAL(Signal_SignalMouseRelease(const QPolygonF &points, int index)), this, SLOT(Slot_SignalMouseRelease(const QPolygonF &points, int index)));
    }

}

void EffectEditor::RemoveAllShadeWidgets()
{
    if(shadeWidgets.size() <= 0) return;
    for(int i=shadeWidgets.size()-1; i!=0; i--)
    {
        scrollArea.RemoveWidget(shadeWidgets[i]);
    }
    shadeWidgets.clear();
}

inline static bool x_less_than(const QPointF &p1, const QPointF &p2)
{
    return p1.x() < p2.x();
}

void EffectEditor::Slot_colorsChanged(QPolygonF &points, int index)
{
    std::sort(points.begin(), points.end(), x_less_than);
}
void EffectEditor::Slot_SignalMouseRelease(QPolygonF &points, int index)
{
    vector<funcSection_t> sections;
    sections.resize(points.size()-1);
    int i=0;
    for(funcSection_t &s : sections)
    {
        s.minX = points[i].x();
        s.minY = points[i].y();

        s.maxX = points[i+1].x();
        s.maxY = points[i+1].y();
        i++;
    }

}

void EffectEditor::DrawPositionLine(float _pos)
{
    for(ShadeWidget *sw : shadeWidgets)
    {
        sw->DrawPositionLine(_pos);
    }
    this->repaint();
}


