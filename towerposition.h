#ifndef TOWERPOSITION_H
#define TOWERPOSITION_H

#include<QPoint>
#include<QPainter>
#include<QPixmap>
#include<QSize>

class TowerPosition
{
public:
    TowerPosition(QPoint pos, const QPixmap &sprite = QPixmap(":new/image/spot.png"));

    void setHasTower(bool hasTower = true);
    //void setNoTower(bool noTower = true);
    bool hasTower() const;
    const QPoint centerPos() const;
    bool containPoint(const QPoint &pos) const;

    void draw(QPainter *painter) const;

private:
    bool		m_hasTower;
    QPoint		m_pos;
    QPixmap		m_sprite;

    static const QSize ms_fixedSize;
};
#endif // TOWERPOSITION_H
