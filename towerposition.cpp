#include "towerposition.h"
#include <QPainter>

const QSize TowerPosition::ms_fixedSize(44, 44);

TowerPosition::TowerPosition(QPoint pos, const QPixmap &sprite/*精灵 = QPixmap(":new/image/open_spot.png")*/)
    : m_hasTower(false)
    , m_pos(pos)
    , m_sprite(sprite)
{}

//返回安装塔区域的中心坐标
const QPoint TowerPosition::centerPos() const
{
    QPoint offsetPoint(ms_fixedSize.width() / 2, ms_fixedSize.height() / 2);
    return m_pos/*安装塔的位置左上角坐标*/ + offsetPoint;
}

//判断点包含在安装塔的区域内
bool TowerPosition::containPoint(const QPoint &pos) const
{
    bool isXInHere = m_pos.x() < pos.x() && pos.x() < (m_pos.x() + ms_fixedSize.width());
    bool isYInHere = m_pos.y() < pos.y() && pos.y() < (m_pos.y() + ms_fixedSize.height());
    return isXInHere && isYInHere;
}

bool TowerPosition::hasTower() const
{
    return m_hasTower;
}

//若安装塔，则改变m_hasTower的值为true
void TowerPosition::setHasTower(bool hasTower/* = true*/)
{
    m_hasTower = hasTower;
}

//绘制塔台
void TowerPosition::draw(QPainter *painter) const
{
    painter->drawPixmap(m_pos.x(), m_pos.y(), m_sprite);
}
