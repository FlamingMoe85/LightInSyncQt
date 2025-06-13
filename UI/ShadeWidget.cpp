#include "ShadeWidget.h"

#define TITLE_HEIGHT    30



ShadeWidget::ShadeWidget(ShadeType type, QWidget *parent, vector<Mapper_Base *> &_funcContVect, int _index, QRect &geometry)
    : QWidget(parent), m_shade_type(type), m_alpha_gradient(QLinearGradient(0, 0, 0, 0))
{

    setAttribute(Qt::WA_NoBackground);

    for(FunctionOwners *fo : _funcContVect)
    {
        funcContVect.push_back(fo->GetFuncCont());
    }

    index = _index;

    QPolygonF points;
    points << QPointF(0, 0)
           << QPointF(1.0, 0);

    title.setText(QString::fromStdString(*(_funcContVect[0]->GetType())));
    title.setParent(this);
    QPalette palette = title.palette();
    palette.setColor(title.foregroundRole(), Qt::yellow);
    title.setPalette(palette);
    title.setStyleSheet("font: 18pt;");

    m_hoverPoints = new HoverPoints(this, HoverPoints::CircleShape, TITLE_HEIGHT);
//  m_hoverPoints->setConnectionType(HoverPoints::LineConnection);
    m_hoverPoints->setPoints(points);
    m_hoverPoints->setPointLock(0, HoverPoints::LockToLeft);
    m_hoverPoints->setPointLock(1, HoverPoints::LockToRight);
    m_hoverPoints->setSortType(HoverPoints::XSort);

    /*
    points.clear();
    points << QPointF(0.5, 0.5)
           << QPointF(0.75, 0.5)
           << QPointF(0.75, 0.75)
           << QPointF(0.5, 0.75);
    selectPoints = new SelectPoints(this, SelectPoints::CircleShape, TITLE_HEIGHT);
    selectPoints->setPoints(points);


    connect(selectPoints, SIGNAL(SignalMouseRelease(QRectF)), this, SLOT(Slot_SelectionChanged(QRectF)));
*/
    setMinimumHeight(100);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setGeometry(geometry);


    connect(m_hoverPoints, SIGNAL(pointsChanged(QPolygonF)), this, SLOT(Slot_colorsChanged()));
    connect(m_hoverPoints, SIGNAL(SignalMouseRelease(QPolygonF)), this, SLOT(Slot_SignalMouseRelease()));
    connect(m_hoverPoints, SIGNAL(Signal_NoteMeAsActive()), this, SLOT(Slot_GetActivityNote()));
    connect(m_hoverPoints, SIGNAL(Signal_PasteHere()), this, SLOT(Slot_PasteHere()));
}

void ShadeWidget::EnableSelect()
{
    hoverPoints()->EnableSelectMode();

}

QPolygonF ShadeWidget::points() const
{
    return m_hoverPoints->points();
}

bool ShadeWidget::eventFilter(QObject *object, QEvent *event)
{
    qDebug() << "ShadeWidget::eventFilter";
    return false;
}

void ShadeWidget::paintEvent(QPaintEvent *)
{
    generateShade();

    QPainter p(this);
    p.drawImage(0, 0, m_shade);

    p.setPen(QColor(146, 146, 146));
    p.drawRect(0, TITLE_HEIGHT, width() - 1, height() - (1+TITLE_HEIGHT));
}


/**/
void ShadeWidget::generateShade()
{
    if (m_shade.isNull() || m_shade.size() != size()) {

        m_shade = QImage(size(), QImage::Format_RGB32);
        QLinearGradient shade(0, TITLE_HEIGHT, 0, (height() -TITLE_HEIGHT));
        shade.setColorAt(1, Qt::black);
        shade.setColorAt(0, Qt::red);
        QPainter p(&m_shade);
        p.fillRect(rect().x(), rect().y()+TITLE_HEIGHT, rect().width(), rect().height()-TITLE_HEIGHT, shade);
    }
}

void ShadeWidget::DrawPositionLine(float _pos)
{
    m_hoverPoints->DrawPositionLine(_pos);
    this->repaint();
}

inline static bool x_less_than(const QPointF &p1, const QPointF &p2)
{
    return p1.x() < p2.x();
}

void ShadeWidget::Slot_colorsChanged()
{
    QPolygonF ps;
    ps += points();

    std::sort(ps.begin(), ps.end(), x_less_than);
    //qDebug() << "Slot_colorsChanged";
}

void ShadeWidget::Slot_SignalMouseRelease()
{
    QPolygonF ps;
    ps += points();

    std::sort(ps.begin(), ps.end(), x_less_than);

    vector<funcSection_t> sections;
    sections.resize(ps.size()-1);
    int i=0;
    for(funcSection_t &s : sections)
    {
        s.minX = ps[i].x();
        s.minY = ps[i].y();

        s.maxX = ps[i+1].x();
        s.maxY = ps[i+1].y();
        i++;
    }
    for(int m=0; m<funcContVect.size(); m++)
    {
        funcContVect[m]->SetFunctionSections(sections);
        //bsmTilt.GetBundleSeries(m)->funcContainer.SetFunctionSections(sections);
    }
    //qDebug() << "Slot_SignalMouseRelease";
}

void ShadeWidget::Slot_GetActivityNote()
{
    Signal_NoteMeAsActive(index);
}

void ShadeWidget::Slot_PasteHere()
{
    Signal_PasteHere(index);
}

void ShadeWidget::Slot_SelectionChanged(QRectF searchArea)
{
    qDebug() << searchArea;
}
