/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
/*
#ifdef QT_OPENGL_SUPPORT
#include <QGLWidget>
#endif
*/

//#include "arthurwidgets.h"
#include "hoverpoints.h"

#include <algorithm>
#include <QDebug>

#define printf

#define NO_POINT_SELECTED   -1

HoverPoints::HoverPoints(QWidget *widget, PointShape shape, qreal _titleHeight)
    : QObject(widget), titleHeight(_titleHeight)
{
    m_widget = widget;
    widget->installEventFilter(this);
    widget->setAttribute(Qt::WA_AcceptTouchEvents);

    m_connectionType = LineConnection;
    m_sortType = NoSort;
    m_shape = shape;
    m_pointPen = QPen(QColor(255, 255, 255, 191), 1);
    m_connectionPen = QPen(QColor(255, 255, 255, 127), 2);
    m_pointBrush = QBrush(QColor(191, 191, 191, 127));
    m_pointSize = QSize(11, 11);
    m_currentIndex = NO_POINT_SELECTED;
    m_editable = true;
    m_enabled = true;

    positionLine << QPointF(0, 0)
                 << QPointF(0, 1.0);

    opMode = OpMode::DEFAULT;

    connect(this, SIGNAL(pointsChanged(QPolygonF)),
            m_widget, SLOT(update()));
}


void HoverPoints::EnableSelectMode()
{
    opMode = OpMode::COPY;
}

void HoverPoints::DisableSelectMode()
{
    opMode = OpMode::DEFAULT;
    selectedIndexes.clear();
    UpdateSelectedPoints();
}

void HoverPoints::UpdateSelectedPoints()
{
    paintPoints();
}


void HoverPoints::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        m_widget->update();
    }
}
/* */
 qreal HoverPoints::TranslateRelToAbsX(qreal x)
{
     return x*boundingRect().width() + boundingRect().x();
}


qreal HoverPoints::TranslateRelToAbsY(qreal y)
{
    return y*(boundingRect().height()-titleHeight) + (boundingRect().y()+titleHeight);
}

QPointF HoverPoints::TranslateAbsToRel(qreal x, qreal y)
{
    QPointF rP;
    rP.rx() = (x-boundingRect().x())/boundingRect().width();
    rP.ry() = (y-(boundingRect().y()+titleHeight))/(boundingRect().height()-titleHeight);
    return rP;
}

QPointF HoverPoints::TranslateRelToAbs(qreal x, qreal y)
{
    QPointF p;
    p.rx() = TranslateRelToAbsX(x);
    p.ry() = TranslateRelToAbsY(y);
    return p;
}

bool eventFilterIdleMode(QObject *object, QEvent *event)
{

}

bool eventFilterCopyPaste(QObject *object, QEvent *event)
{

}

bool HoverPoints::eventFilter(QObject *object, QEvent *event)
{
    if (object == m_widget && m_enabled) {
        switch (event->type())
        {
            case QEvent::MouseButtonPress:
            {
                if (!m_fingerPointMapping.isEmpty())return true;
                QMouseEvent *me = (QMouseEvent *) event;

                QPointF clickPos = me->pos();
                int newSelectedPoint = NO_POINT_SELECTED;
                IndexOfClickedPoint(newSelectedPoint, clickPos);

                if (me->button() == Qt::LeftButton)
                {
                    if(opMode == OpMode::PASTE)
                    {
                        Signal_PasteHere();
                        opMode = OpMode::DEFAULT;
                    }
                    else if((newSelectedPoint != NO_POINT_SELECTED) && (opMode == OpMode::COPY))
                    {
                        Copy(newSelectedPoint);
                    }
                    else if ((newSelectedPoint == NO_POINT_SELECTED) && (opMode == OpMode::DEFAULT))
                    {
                        if (!m_editable) return false;
                        InsertPoint(clickPos);
                    }
                    else//NO_POINT_SELECTED
                    {
                        m_currentIndex = newSelectedPoint;
                    }
                    return true;
                }
                else if (me->button() == Qt::RightButton)
                {
                    RemovePoint(newSelectedPoint);
                    return true;
                }
            }
            break;

        case QEvent::MouseButtonRelease:
            emit SignalMouseRelease(m_points);
            if (!m_fingerPointMapping.isEmpty())
                return true;
            m_currentIndex = NO_POINT_SELECTED;
            break;

        case QEvent::MouseMove:
            if (!m_fingerPointMapping.isEmpty())
                return true;
            if (m_currentIndex >= 0)
                movePoint(m_currentIndex, TranslateAbsToRel(((QMouseEvent *)event)->pos().x(), ((QMouseEvent *)event)->pos().y()));
            break;
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
            {
                const QTouchEvent *const touchEvent = static_cast<const QTouchEvent*>(event);
                const QList<QTouchEvent::TouchPoint> points = touchEvent->touchPoints();
                const qreal pointSize = qMax(m_pointSize.width(), m_pointSize.height());
                foreach (const QTouchEvent::TouchPoint &touchPoint, points) {
                    const int id = touchPoint.id();
                    switch (touchPoint.state()) {
                    case Qt::TouchPointPressed:
                        {
                            // find the point, move it
                            QSet<int> activePoints = QSet<int>::fromList(m_fingerPointMapping.values());
                            int activePoint = NO_POINT_SELECTED;
                            qreal distance = -1;
                            const int pointsCount = m_points.size();
                            const int activePointCount = activePoints.size();
                            if (pointsCount == 2 && activePointCount == 1) { // only two points
                                activePoint = activePoints.contains(0) ? 1 : 0;
                            } else {
                                for (int i=0; i<pointsCount; ++i) {
                                    if (activePoints.contains(i))
                                        continue;

                                    qreal d = QLineF(touchPoint.pos(), TranslateRelToAbs(m_points.at(i).x(), m_points.at(i).y())).length();
                                    if ((distance < 0 && d < 12 * pointSize) || d < distance) {
                                        distance = d;
                                        activePoint = i;
                                    }

                                }
                            }
                            if (activePoint != NO_POINT_SELECTED) {
                                m_fingerPointMapping.insert(touchPoint.id(), activePoint);
                                movePoint(activePoint, TranslateAbsToRel(touchPoint.pos().x(), touchPoint.pos().y()));
                            }
                        }
                        break;
                    case Qt::TouchPointReleased:
                        {
                            // move the point and release
                            QHash<int,int>::iterator it = m_fingerPointMapping.find(id);
                            movePoint(it.value(), TranslateAbsToRel(touchPoint.pos().x(), touchPoint.pos().y()));
                            m_fingerPointMapping.erase(it);
                        }
                        break;
                    case Qt::TouchPointMoved:
                        {
                            // move the point
                            const int pointIdx = m_fingerPointMapping.value(id, NO_POINT_SELECTED);
                            if (pointIdx >= 0) // do we track this point?
                                movePoint(pointIdx, TranslateAbsToRel(touchPoint.pos().x(), touchPoint.pos().y()));
                        }
                        break;
                    default:
                        break;
                    }
                }
                if (m_fingerPointMapping.isEmpty()) {
                    event->ignore();
                    return false;
                } else {
                    return true;
                }
            }
            break;
        case QEvent::TouchEnd:
            if (m_fingerPointMapping.isEmpty()) {
                event->ignore();
                return false;
            }
            return true;
            break;

        case QEvent::Resize:
        {
            QResizeEvent *e = (QResizeEvent *) event;
            if (e->oldSize().width() == 0 || e->oldSize().height() == 0)
                break;

            firePointChange();
            break;
        }

        case QEvent::Paint:
        {
            QWidget *that_widget = m_widget;
            m_widget = 0;
            QApplication::sendEvent(object, event);
            m_widget = that_widget;
            paintPoints();
            return true;
        }
        default:
            break;
        }
    }

    return false;
}


void HoverPoints::paintPoints()
{
    QPainter p;
    p.begin(m_widget);
    p.setRenderHint(QPainter::Antialiasing);

    if (m_connectionPen.style() != Qt::NoPen && m_connectionType != NoConnection) {
        p.setPen(m_connectionPen);

        if (m_connectionType == CurveConnection) {
            QPainterPath path;
            path.moveTo(TranslateRelToAbs(m_points.at(0).x(),m_points.at(0).y()));//;
            for (int i=1; i<m_points.size(); ++i) {
                QPointF p1 = TranslateRelToAbs(m_points.at(i-1).x(), m_points.at(i-1).y());//
                QPointF p2 = TranslateRelToAbs(m_points.at(i).x(), m_points.at(i).y());//
                qreal distance = p2.x() - p1.x();

                path.cubicTo(p1.x() + distance / 2, p1.y(),
                             p1.x() + distance / 2, p2.y(),
                             p2.x(), p2.y());
            }
            p.drawPath(path);
        } else {
            QPolygonF polygon;
            for(QPointF p : m_points)
            {
                polygon.append(TranslateRelToAbs(p.rx(), p.ry()));
            }
            p.drawPolyline(polygon);
        }
    }

    p.setPen(m_pointPen);
    p.setBrush(m_pointBrush);

    for (int i=0; i<m_points.size(); ++i) {
        if(selectedIndexes.contains(i))
        {
            m_pointBrush.setColor(QColor(191, 191, 0, 255));
            p.setBrush(m_pointBrush);
        }
        else
        {
            m_pointBrush.setColor(QColor(191, 191, 191, 127));
            p.setBrush(m_pointBrush);
        }
        QRectF bounds = pointBoundingRect(i, TranslateRelToAbsX(m_points[i].x()), TranslateRelToAbsY(m_points[i].y()));
        if (m_shape == CircleShape)
            p.drawEllipse(bounds);
        else
            p.drawRect(bounds);
    }

    p.drawPolyline(positionLine);
}

void HoverPoints::DrawPositionLine(float _pos)
{
    positionLine[0].setX(TranslateRelToAbsX(_pos));
    positionLine[0].setY(TranslateRelToAbsY(0));
    positionLine[1].setX(positionLine[0].x());
    positionLine[1].setY(TranslateRelToAbsY(1));
}

static QPointF bound_point(const QPointF &point, const QRectF &bounds, int lock)
{
    QPointF p = point;

    if (p.x() < 0.0 || lock & HoverPoints::LockToLeft)
    {
        p.setX(0.0);
    }
    else if (p.x() > 1.0 ||lock & HoverPoints::LockToRight)
        {
            p.setX(1.0);
        }

    if (p.y() < 0.0 ||lock & HoverPoints::LockToTop)
    {
        p.setY(0.0);
    }
    else if (p.y() > 1.0 ||lock & HoverPoints::LockToBottom)
        {
            p.setY(1.0);
        }

    return p;
}

void HoverPoints::setPoints(const QPolygonF &points)
{
    if (points.size() != m_points.size())
        m_fingerPointMapping.clear();
    if(opMode != OpMode::PASTE)
    {
        m_points.clear();
    }

    for (int i=0; i<points.size(); ++i)
    m_points << bound_point(points.at(i), boundingRect(), 0);

    m_locks.clear();
    if (m_points.size() > 0) {
        m_locks.resize(m_points.size());

        m_locks.fill(0);
    }

    if(opMode == OpMode::PASTE)
    {
        firePointChange();
    }
}


void HoverPoints::movePoint(int index, const QPointF &point, bool emitUpdate)
{
    //m_points[index] = point;
    m_points[index] = bound_point(point, boundingRect(), m_locks.at(index));
    if (emitUpdate)
        firePointChange();
}


inline static bool x_less_than(const QPointF &p1, const QPointF &p2)
{
    return p1.x() < p2.x();
}


inline static bool y_less_than(const QPointF &p1, const QPointF &p2)
{
    return p1.y() < p2.y();
}

void HoverPoints::firePointChange()
{
//    printf("HoverPoints::firePointChange(), current=%d\n", m_currentIndex);

    if (m_sortType != NoSort) {

        QPointF oldCurrent;
        if (m_currentIndex != NO_POINT_SELECTED) {
            oldCurrent = m_points[m_currentIndex];
        }

        if (m_sortType == XSort)
            std::sort(m_points.begin(), m_points.end(), x_less_than);
        else if (m_sortType == YSort)
            std::sort(m_points.begin(), m_points.end(), y_less_than);

        // Compensate for changed order...
        if (m_currentIndex != NO_POINT_SELECTED) {
            for (int i=0; i<m_points.size(); ++i) {
                if (m_points[i] == oldCurrent) {
                    m_currentIndex = i;
                    break;
                }
            }
        }

//         printf(" - firePointChange(), current=%d\n", m_currentIndex);

    }

//     for (int i=0; i<m_points.size(); ++i) {
//         printf(" - point(%2d)=[%.2f, %.2f], lock=%d\n",
//                i, m_points.at(i).x(), m_points.at(i).y(), m_locks.at(i));
//     }

    emit pointsChanged(m_points);
}

void HoverPoints::CopySelectedPoints(QPolygonF &pointsCopied)
{
    pointsCopied.clear();
    int i = 0;
    for(QPointF &p : m_points)
    {
        QPointF copyPoint(p);
        if(selectedIndexes.contains(i))
        {
            pointsCopied.append(copyPoint);
        }
        i++;
    }
}

void HoverPoints::WaitForPaste()
{
    opMode = OpMode::PASTE;
}

void HoverPoints::SetXonSelected(float _x)
{
    for(int i : selectedIndexes)
    {
        m_points[i].setX(_x);
    }
    firePointChange();
}
void HoverPoints::SetYonSelected(float _y)
{
    for(int i : selectedIndexes)
    {
        m_points[i].setY(_y);
    }
    firePointChange();
}
void HoverPoints::SetPonSelected(float _x, float _y)
{
    for(int i : selectedIndexes)
    {
        m_points[i].setX(_x);
        m_points[i].setY(_y);
    }
    firePointChange();
}
