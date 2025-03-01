#ifndef SHADEWIDGET_H
#define SHADEWIDGET_H

#include <QWidget>

class HoverPoints;

class ShadeWidget : public QWidget
{
    Q_OBJECT

public:
    enum ShadeType {
        RedShade
    };

    ShadeWidget(ShadeType type, QWidget *parent);

    void paintEvent(QPaintEvent *e) override;

    QSize sizeHint() const override { return QSize(150, 80); }
    QPolygonF points() const;

    HoverPoints *hoverPoints() const { return m_hoverPoints; }

    void DrawPositionLine(float _pos);

signals:
    void colorsChanged();
    void SignalMouseRelease(const QPolygonF &points);

private:
    void generateShade();

    ShadeType m_shade_type;
    QImage m_shade;
    HoverPoints *m_hoverPoints;
    QLinearGradient m_alpha_gradient;
};

#endif // SHADEWIDGET_H
