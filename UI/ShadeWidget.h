#ifndef SHADEWIDGET_H
#define SHADEWIDGET_H

#include <QWidget>
#include <QLabel>
#include "../../../share/FunctionOwners.hpp"

class HoverPoints;

class ShadeWidget : public QWidget
{
    Q_OBJECT

public:
    enum ShadeType {
        RedShade
    };

    ShadeWidget(ShadeType type, QWidget *parent, vector<FunctionOwners *> &_funcContVect);

    void paintEvent(QPaintEvent *e) override;

    QSize sizeHint() const override { return QSize(150, 80); }
    QPolygonF points() const;

    HoverPoints *hoverPoints() const { return m_hoverPoints; }

    void DrawPositionLine(float _pos);

signals:
    void Signal_colorsChanged(QPolygonF &points);
    void Signal_SignalMouseRelease(QPolygonF &points);

protected:
    int index;

private:
    void generateShade();

    ShadeType m_shade_type;
    QImage m_shade;
    HoverPoints *m_hoverPoints;
    QLinearGradient m_alpha_gradient;
    vector<FunctionContainer *> funcContVect;
    QLabel title;

private slots:
    void Slot_colorsChanged();
    void Slot_SignalMouseRelease();
};

#endif // SHADEWIDGET_H
