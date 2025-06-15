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

#ifndef HOVERPOINTS_H
#define HOVERPOINTS_H

#include <QtWidgets>

enum PointAxisSelect {
    X,
    Y,
    P
};



QT_FORWARD_DECLARE_CLASS(QBypassWidget)

class HoverPoints : public QObject
{
    Q_OBJECT
public:
    enum PointShape {
        CircleShape,
        RectangleShape
    };

    enum LockType {
        LockToLeft   = 0x01,
        LockToRight  = 0x02,
        LockToTop    = 0x04,
        LockToBottom = 0x08
    };

    enum SortType {
        NoSort,
        XSort,
        YSort
    };

    enum ConnectionType {
        NoConnection,
        LineConnection,
        CurveConnection
    };



    HoverPoints(QWidget *widget, PointShape shape, qreal _titleHeight);

    bool eventFilter(QObject *object, QEvent *event) override;
    bool eventFilterSelect(QObject *object, QEvent *event);

    void paintPoints();
    void PaintSelectPoints();

    inline QRectF boundingRect() const;
    void setBoundingRect(const QRectF &boundingRect) { m_bounds = boundingRect; }

    QPolygonF points() const { return m_points; }
    void setPoints(const QPolygonF &points);
    void setSelectPoints(const QPolygonF &points);

    QSizeF pointSize() const { return m_pointSize; }
    void setPointSize(const QSizeF &size) { m_pointSize = size; }

    SortType sortType() const { return m_sortType; }
    void setSortType(SortType sortType) { m_sortType = sortType; }

    ConnectionType connectionType() const { return m_connectionType; }
    void setConnectionType(ConnectionType connectionType) { m_connectionType = connectionType; }

    void setConnectionPen(const QPen &pen) { m_connectionPen = pen; }
    void setShapePen(const QPen &pen) { m_pointPen = pen; }
    void setShapeBrush(const QBrush &brush) { m_pointBrush = brush; }

    void setPointLock(int pos, LockType lock) { m_locks[pos] = lock; }

    void setEditable(bool editable) { m_editable = editable; }
    bool editable() const { return m_editable; }

    void DrawPositionLine(float _pos);

    qreal TranslateRelToAbsX(qreal x);
    qreal TranslateRelToAbsY(qreal y);
    QPointF TranslateAbsToRel(qreal x, qreal y);
    QPointF TranslateRelToAbs(qreal x, qreal y);

    void EnableSelectMode();
    void DisableSelectMode();
    void WaitForPaste();
    void SetAnnounceMeMode();
    void DisableAnnounceMeMode();
    void GetPointInSelectArea(QPolygonF &points);
    void DeletePointsInSelectArea();

    void CopySelectedPoints(QPolygonF &pointsCopied);
    PointAxisSelect axisSelect;

    void SetXonSelected(float _x);
    void SetYonSelected(float _y);
    void SetPonSelected(float _x, float _y);


public slots:
    void setEnabled(bool enabled);
    void setDisabled(bool disabled) { setEnabled(!disabled); }

signals:
    void pointsChanged(const QPolygonF &points);
    void SignalMouseRelease(const QPolygonF &points);
    void Signal_NoteMeAsActive();
    void Signal_PasteHere();

public:
    void firePointChange();

private:
    inline QRectF pointBoundingRect(int i, qreal pX, qreal pY) const;
    void movePoint(int i, const QPointF &newPos, bool emitChange = true);
    void moveSelectPoint(int i, const QPointF &newPos, bool emitChange = true);

    QWidget *m_widget;

    QPolygonF m_points;
    QPolygonF select_points;
    QRectF m_bounds;
    QRectF searchArea;
    PointShape m_shape;
    SortType m_sortType;
    ConnectionType m_connectionType;

    QVector<uint> m_locks;
    QVector<uint> m_locksSelect;

    QSizeF m_pointSize;
    int m_currentIndex;
    int m_currentIndexSelect;
    QVector<int> selectedIndexes;
    bool m_editable;
    bool m_enabled;

    QHash<int, int> m_fingerPointMapping;
    QHash<int, int> m_fingerPointMappingSelect;

    QPen m_pointPen;
    QBrush m_pointBrush;
    QPen m_connectionPen;

    QPolygonF positionLine;

    qreal titleHeight;

    int opMode;
    void UpdateSelectedPoints();

    void GetIndexesInSelectArea();
    bool IsPointInSelectArea(QPointF &p);

    void IndexOfClickedPoint(int& _i, QPointF &clickPos, QPolygonF &points)
    {
        for (int i=0; i<points.size(); ++i) {
            QPainterPath path;
            if (m_shape == CircleShape)
                path.addEllipse(pointBoundingRect(i, TranslateRelToAbsX(points[i].x()), TranslateRelToAbsY(points[i].y())));
            else
                path.addRect(pointBoundingRect(i, TranslateRelToAbsX(points[i].x()), TranslateRelToAbsY(points[i].y())));

            if (path.contains(clickPos)) {
                _i = i;
                break;
            }
        }
    }

    void Copy(int _index)
    {
        m_currentIndex = -1;
        Signal_NoteMeAsActive();
        selectedIndexes.append(_index);
        /*
        if(selectedIndexes.contains(_index))
        {
            selectedIndexes.remove(_index);
        }
        else
        {
            selectedIndexes.append(_index);
        }
        */
        UpdateSelectedPoints();
    }

    void InsertPoint(QPointF &clickPos)
    {
        int pos = 0;
        // Insert sort for x or y
        if (m_sortType == XSort) {
            for (int i=0; i<m_points.size(); ++i)
                if (TranslateRelToAbsX(m_points.at(i).x()) > clickPos.x()) {
                    pos = i;
                    break;
                }
        } else if (m_sortType == YSort) {
            for (int i=0; i<m_points.size(); ++i)
                if (TranslateRelToAbsY(m_points.at(i).y()) > clickPos.y()) {
                    pos = i;
                    break;
                }
        }
        m_points.insert(pos, TranslateAbsToRel(clickPos.rx(), clickPos.ry()));
        m_locks.insert(pos, 0);
        m_currentIndex = pos;
        firePointChange();
    }

    void RemovePoint(int index, bool emitPointChange)
    {
        if (index >= 0 && m_editable)
        {
            if (m_locks[index] == 0)
            {
                m_locks.remove(index);
                m_points.remove(index);
            }
            if(emitPointChange)firePointChange();
        }
    }

    void RemoveSelectPoint(int index)
    {
        if (index >= 0 && m_editable)
        {
            select_points.remove(index);
        }
    }
};


inline QRectF HoverPoints::pointBoundingRect(int i, qreal pX, qreal pY) const
{
    qreal w = m_pointSize.width();
    qreal h = m_pointSize.height();
    qreal x = pX - w / 2;
    qreal y = pY - h / 2;
    return QRectF(x, y, w, h);
}

inline QRectF HoverPoints::boundingRect() const
{
    if (m_bounds.isEmpty())
        return m_widget->rect();
    else
        return m_bounds;
}

#endif // HOVERPOINTS_H
