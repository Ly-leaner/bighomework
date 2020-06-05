#include "tower.h"

#include "mainwindow.h"
#include <QPainter>

const QSize Tower::ms_fixedSize(42, 42);

Tower::Tower(QPoint pos, MainWindow *game, const QPixmap &sprite/* = QPixmap(":/image/tower.png"*/)
    : m_attacking(false)
    , m_attackRange(70)
    , m_damage(10)
    , m_fireRate(1000)
    , m_rotationSprite(0.0)
    , m_pos(pos)
    , m_sprite(sprite)
{
}

void Tower::draw(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::white);
    // ���ƹ�����Χ
    painter->drawEllipse(m_pos, m_attackRange, m_attackRange);

    // ����ƫת����,������+ƫ��=����
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    // ����������ѡ������
    painter->translate(m_pos);//Translates the coordinate system by the given offset
    painter->rotate(m_rotationSprite);//Rotates the coordinate system clockwise
    painter->drawPixmap(offsetPoint, m_sprite);//�����Ͻ�Ϊ����ԭ���ͼ
    painter->restore();
}

void Tower::lostSightOfEnemy()
{}
