#include "EffectEditor.h"
#include "ui_EffectEditor.h"
#include "hoverpoints.h"

#include "QDebug"


EffectEditor::EffectEditor(int x, int y, int w, int h, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EffectEditor)
{
    ui->setupUi(this);

    ui->verticalLayout_ScrollArea->addWidget(&scrollArea);
    connect(ui->pushButton_CopyX, SIGNAL(clicked()), this, SLOT(Slot_CopyX()));
    connect(ui->pushButton_CopyY, SIGNAL(clicked()), this, SLOT(Slot_CopyY()));
    connect(ui->pushButton_CopyPoint, SIGNAL(clicked()), this, SLOT(Slot_CopyPoint()));
    connect(ui->pushButton_setX, SIGNAL(clicked()), this, SLOT(Slot_SetX()));
    connect(ui->pushButton_setY, SIGNAL(clicked()), this, SLOT(Slot_SetY()));
    connect(ui->pushButton_setP, SIGNAL(clicked()), this, SLOT(Slot_SetBoth()));

    pasteWidget = -1;
    geometry.setRect(x, y, w, h);
}

EffectEditor::~EffectEditor()
{
    delete ui;
}


void EffectEditor::keyPressEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat())
    {
        qDebug() << "keyPressEvent: " << event->key();
        if(event->key() == 67)
        {
            for(ShadeWidget* sW : shadeWidgets)
            {
                sW->hoverPoints()->EnableSelectMode();
            }
        }

        if(event->key() == Qt::Key_Escape)
        {
            for(ShadeWidget* sW : shadeWidgets)
            {
                sW->hoverPoints()->DisableSelectMode();
            }
        }

        if(event->key() == 86)
        {
            for(ShadeWidget* sW : shadeWidgets)
            {
                sW->hoverPoints()->DisableSelectMode();
                sW->hoverPoints()->WaitForPaste();
            }
        }
    }
}

void EffectEditor::SetItemGeometry(int x, int y, int w, int h)
{
    geometry.setRect(x, y, w, h);
}

void EffectEditor::SetWidth(int _w)
{
    scrollArea.SetWidthOfItems(_w);
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

        ShadeWidget* shadeWidget = new ShadeWidget(ShadeWidget::RedShade, NULL, funcOwnVec, i, geometry);
        shadeWidgets[i] = shadeWidget;
        scrollArea.AddWidget(shadeWidget);
        connect(shadeWidgets[i], SIGNAL(Signal_NoteMeAsActive(int)), this, SLOT(Slot_GetActivityNote(int)));
        connect(shadeWidgets[i], SIGNAL(Signal_PasteHere(int)), this, SLOT(Slot_PasteHere(int)));
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

void EffectEditor::Slot_GetActivityNote(int pinger)
{
    int i=0;
    activeWidget = pinger;
    for(ShadeWidget* sW : shadeWidgets)
    {
        if(i != pinger)
        {
            sW->hoverPoints()->DisableSelectMode();
        }
        i++;
    }
}

void EffectEditor::Slot_PasteHere(int pasteTarget)
{
    shadeWidgets[pasteTarget]->hoverPoints()->setPoints(copiedPoints);
}


void EffectEditor::Slot_CopyX()
{
    if(activeWidget != -1)
    {
        shadeWidgets[activeWidget]->hoverPoints()->CopySelectedPoints(copiedPoints);
    }

    for(ShadeWidget* sW : shadeWidgets)
    {
        sW->hoverPoints()->axisSelect = PointAxisSelect::X;
    }
}

void EffectEditor::Slot_CopyY()
{
    if(activeWidget != -1)
    {
        shadeWidgets[activeWidget]->hoverPoints()->CopySelectedPoints(copiedPoints);
    }

    for(ShadeWidget* sW : shadeWidgets)
    {
        sW->hoverPoints()->axisSelect = PointAxisSelect::Y;
    }
}

void EffectEditor::Slot_CopyPoint()
{
    if(activeWidget != -1)
    {
        shadeWidgets[activeWidget]->hoverPoints()->CopySelectedPoints(copiedPoints);
    }

    for(ShadeWidget* sW : shadeWidgets)
    {
        sW->hoverPoints()->axisSelect = PointAxisSelect::P;
    }
}

void EffectEditor::Slot_SetX()
{
    if(activeWidget != -1)
    {
        shadeWidgets[activeWidget]->hoverPoints()->SetXonSelected(ui->doubleSpinBox_X->value());
        shadeWidgets[activeWidget]->Slot_SignalMouseRelease();
    }
}
void EffectEditor::Slot_SetY()
{
    if(activeWidget != -1)
    {
        shadeWidgets[activeWidget]->hoverPoints()->SetYonSelected(ui->doubleSpinBox_Y->value());
        shadeWidgets[activeWidget]->Slot_SignalMouseRelease();
    }
}
void EffectEditor::Slot_SetBoth()
{
    if(activeWidget != -1)
    {
        shadeWidgets[activeWidget]->hoverPoints()->SetPonSelected(ui->doubleSpinBox_X->value(), ui->doubleSpinBox_Y->value());
        shadeWidgets[activeWidget]->Slot_SignalMouseRelease();
    }
}
