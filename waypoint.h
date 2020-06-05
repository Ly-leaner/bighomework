#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QPoint>

class QPainter;

// �����ƶ��ĺ���
class WayPoint
{
public:
    WayPoint(QPoint pos);

    void setNextWayPoint(WayPoint *nextPoint);
    WayPoint* nextWayPoint() const;
    const QPoint pos() const;

    void draw(QPainter *painter) const;

private:
    const QPoint		m_pos;
    WayPoint *			m_nextWayPoint;
};

#endif // WAYPOINT_H
