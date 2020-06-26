#include "enemy2.h"

#include"tower.h"
#include"tower2.h"
#include "enemy.h"
#include "waypoint.h"
#include "utilityfunction.h"//只含有一个碰撞检测函数
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
        return;//不能移动则返回

    painter->save();

    QPoint healthBarPoint = m_pos + QPoint(-20 / 2 - 5/*调整血条位置，使之位于敌人上方*/, -ms_fixedSize.height() / 3);
    // 绘制血条
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(20, 3));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::blue);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * 20, 3));
    painter->drawRect(healthBarRect);

    // 绘制偏转坐标,由中心+偏移->左上
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(m_pos);  //绘制中心坐标由画图设备的左上角转换到敌人中心
    painter->rotate(m_rotationSprite);  //绘图坐标系顺时针转过一个角度（直角）
    // 绘制敌人
    painter->drawPixmap(offsetPoint, m_sprite);//绘图中心位于敌人左上角

    painter->restore();
}
