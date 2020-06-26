#include "enemy2.h"

#include"tower.h"
#include"tower2.h"
#include "enemy.h"
#include "waypoint.h"
#include "utilityfunction.h"//ֻ����һ����ײ��⺯��
#include "mainwindow.h"
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMatrix>
#include <QVector2D>
#include <QtMath>
#include<QSoundEffect>

Enemy2::Enemy2(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite /*= QPixmap(":/new/image/enemy2.png")*/)
    :Enemy(startWayPoint, game, sprite)
{
    m_maxHp = 100;
    m_currentHp = 100;
    m_walkingSpeed = 1.5;
}

Enemy2::~Enemy2()
{
    foreach (/*const*/ Tower * tower, this->m_attackedTowersList) {
        tower->timeStop();
    }
    this->m_attackedTowersList.clear();
    m_destinationWayPoint = NULL;
    m_game = NULL;
}

void Enemy2::draw(QPainter *painter) const
{
    if (!m_active)
        return;//�����ƶ��򷵻�

    painter->save();

    QPoint healthBarPoint = m_pos + QPoint(-20 / 2 - 5/*����Ѫ��λ�ã�ʹ֮λ�ڵ����Ϸ�*/, -ms_fixedSize.height() / 3);
    // ����Ѫ��
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(20, 3));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::blue);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * 20, 3));
    painter->drawRect(healthBarRect);

    // ����ƫת����,������+ƫ��->����
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(m_pos);  //�������������ɻ�ͼ�豸�����Ͻ�ת������������
    painter->rotate(m_rotationSprite);  //��ͼ����ϵ˳ʱ��ת��һ���Ƕȣ�ֱ�ǣ�
    // ���Ƶ���
    painter->drawPixmap(offsetPoint, m_sprite);//��ͼ����λ�ڵ������Ͻ�

    painter->restore();
}
