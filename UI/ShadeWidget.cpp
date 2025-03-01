#include "ShadeWidget.h"
#include "hoverpoints.h"


ShadeWidget::ShadeWidget(ShadeType type, QWidget *parent)
    : QWidget(parent), m_shade_type(type), m_alpha_gradient(QLinearGradient(0, 0, 0, 0))
{

    setAttribute(Qt::WA_NoBackground);


    QPolygonF points;
    points << QPointF(0, 0)
           << QPointF(1.0, 0);

    m_hoverPoints = new HoverPoints(this, HoverPoints::CircleShape);
//     m_hoverPoints->setConnectionType(HoverPoints::LineConnection);
    m_hoverPoints->setPoints(points);
    m_hoverPoints->setPointLock(0, HoverPoints::LockToLeft);
    m_hoverPoints->setPointLock(1, HoverPoints::LockToRight);
    m_hoverPoints->setSortType(HoverPoints::XSort);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(m_hoverPoints, SIGNAL(pointsChanged(QPolygonF)), this, SIGNAL(colorsChanged()));
    connect(m_hoverPoints, SIGNAL(SignalMouseRelease(QPolygonF)), this, SIGNAL(SignalMouseRelease()));
}

QPolygonF ShadeWidget::points() const
{
    return m_hoverPoints->points();
}


void ShadeWidget::paintEvent(QPaintEvent *)
{
    generateShade();

    QPainter p(this);
    p.drawImage(0, 0, m_shade);

    p.setPen(QColor(146, 146, 146));
    p.drawRect(0, 0, width() - 1, height() - 1);
}

/**/
void ShadeWidget::generateShade()
{
    if (m_shade.isNull() || m_shade.size() != size()) {

        m_shade = QImage(size(), QImage::Format_RGB32);
        QLinearGradient shade(0, 0, 0, height());
        shade.setColorAt(1, Qt::black);
        shade.setColorAt(0, Qt::red);
        QPainter p(&m_shade);
        p.fillRect(rect(), shade);
    }
}

void ShadeWidget::DrawPositionLine(float _pos)
{
    m_hoverPoints->DrawPositionLine(_pos);
    this->repaint();
}
