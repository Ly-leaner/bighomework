#include "enemy.h"
#include "waypoint.h"
#include "tower.h"
#include "utilityfunction.h"//只含有一个碰撞检测函数
#include "mainwindow.h"
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMatrix>
#include <QVector2D>
#include <QtMath>
#include<QSoundEffect>

static const int Health_Bar_Width = 20;

const QSize Enemy::ms_fixedSize(52, 52);

Enemy::Enemy(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite/* = QPixmap(":new/image/enemy.png")*/)
    : QObject(0)
    , m_active(false)
    , m_maxHp(50)
    , m_currentHp(50)
    , m_walkingSpeed(1.0)
    , m_rotationSprite(0.0)
    , m_pos(startWayPoint->pos())
    , m_destinationWayPoint(startWayPoint->nextWayPoint())
    , m_game(game)
    , m_sprite(sprite)
{}

Enemy::~Enemy()
{
    m_attackedTowersList.clear();
    m_destinationWayPoint = NULL;
    m_game = NULL;
}

void Enemy::doActivate()
{
    m_active = true;
}

void Enemy::move()
{
    if (!m_active)
        return;//不能移动则返回

    if (collisionWithCircle(m_pos, 1, m_destinationWayPoint->pos(), 1))
    {// 敌人抵达了一个航点
        if (m_destinationWayPoint->nextWayPoint())
        {// 还有下一个航点
            m_pos = m_destinationWayPoint->pos();//移动到下一个航点
            m_destinationWayPoint = m_destinationWayPoint->nextWayPoint();
        }
        else
        {// 表示进入基地
            m_game->getHpDamage();//基地耗血
            m_game->removedEnemy(this);//移除敌人
            return;
        }
    }

    // 还在前往航点的路上
    // 目标航点的坐标
    QPoint targetPoint = m_destinationWayPoint->pos();

    // 修改这个可以添加移动状态,加快,减慢,m_walkingSpeed是基准值
    // 向量标准化
    double movementSpeed = m_walkingSpeed + (double)m_game->Wave_num()/2.0;
    QVector2D/*二维向量*/ normalized(targetPoint - m_pos);
    normalized.normalize();//二维向量标准化
    //当前位置位置+=标准化二维向量*移动速度
    m_pos = m_pos + normalized.toPoint() * movementSpeed;

    // 确定敌人选择方向
    // 默认图片向左,修正180度转右
    m_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) + 180;

    /*
     * qRadiansToDegrees：This function converts the radians in double to degrees，弧度->角度
     * qAtan2：This function will return the angle (argument) of that point
    */
}

void Enemy::draw(QPainter *painter) const
{
    if (!m_active)
        return;//不能移动则返回

    painter->save();

    QPoint healthBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5/*调整血条位置，使之位于敌人上方*/, -ms_fixedSize.height() / 3);
    // 绘制血条
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, 2));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * Health_Bar_Width, 2));
    painter->drawRect(healthBarRect);

    // 绘制偏转坐标,由中心+偏移->左上
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(m_pos);  //绘制中心坐标由画图设备的左上角转换到敌人中心
    painter->rotate(m_rotationSprite);  //绘图坐标系顺时针转过一个角度（直角）
    // 绘制敌人
    painter->drawPixmap(offsetPoint, m_sprite);//绘图中心位于敌人左上角

    painter->restore();
}



void Enemy::getAttacked(Tower *attacker)
{
    m_attackedTowersList.push_back(attacker);
}

// 表明敌人已经逃离了攻击范围
void Enemy::gotLostSight(Tower *attacker)//
{
    m_attackedTowersList.removeOne(attacker);
}

QPoint Enemy::pos() const
{
    return m_pos;
}

void Enemy::getRemoved()//
{
    if (m_attackedTowersList.empty())
        return;

    foreach (Tower *attacker, m_attackedTowersList)
        attacker->targetKilled();
    // 通知game,此敌人已经阵亡
    m_game->removedEnemy(this);
}

void Enemy::getDamage(int damage)//
{
    m_currentHp -= damage;

    // 阵亡,需要移除
    if (m_currentHp <= 0)
    {
        QSoundEffect *m_place_tower = new QSoundEffect;
        m_place_tower->setSource(QUrl("qrc:new/music/kill_enemy.wav"));
        m_place_tower->setLoopCount(1);
        m_place_tower->setVolume(0.5);
        m_place_tower->play();


        m_game->awardGold(200);
        getRemoved();
    }
}


