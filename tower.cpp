#include "tower.h"

#include "enemy.h"
#include "bullet.h"
#include "mainwindow.h"
#include "utilityfunction.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>
#include<QSoundEffect>

//塔的攻击原则：选择一个敌人持续攻击直到敌人死亡或移除攻击范围

//const QSize Tower::ms_fixedSize(42, 42);

Tower::Tower(QPoint pos, MainWindow *game, const QSize &size/* = QSize(44,44)*/, const QPixmap &sprite/* = QPixmap(":/image/tower.png"*/)
    : m_attacking(false),
    m_attackRange(75),
    m_damage(10),
    m_fireRate(1000),
    m_rotationSprite(0.0),
    m_pos(pos),
    m_sprite(sprite),
    m_chooseEnemy(NULL),
    m_game(game),
    ms_fixedSize(size)
{
    m_fireRateTimer = new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
}

Tower::~Tower()
{
    delete m_fireRateTimer;
    m_fireRateTimer = NULL;
}

void Tower::timeStop()
{
    m_fireRateTimer->stop();
}

void Tower::draw(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::white);
    // 绘制攻击范围
    painter->drawEllipse(m_pos, m_attackRange, m_attackRange);

    // 绘制偏转坐标,由中心+偏移=左上
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    // 绘制炮塔并选择炮塔
    painter->translate(m_pos);//Translates the coordinate system by the given offset
    painter->rotate(m_rotationSprite);//Rotates the coordinate system clockwise
    painter->drawPixmap(offsetPoint, m_sprite);//以左上角为坐标原点绘图
    painter->restore();

    //if(m_place_tower)delete m_place_tower;
//    QSoundEffect *m_place_tower = new QSoundEffect;
//    m_place_tower->setSource(QUrl("qrc:new/music/place_tower.wav"));
//    m_place_tower->setLoopCount(1);
//    m_place_tower->setVolume(0.5);
//    m_place_tower->play();
}

void Tower::attackEnemy()
{
    m_fireRateTimer->start(m_fireRate);//启动塔的计时器，时间间隔为m_fireRate
}

void Tower::checkEnemyInRange()
{
    if (m_chooseEnemy)
    {
        // 这种情况下,需要旋转炮台对准敌人
        // 向量标准化
        QVector2D normalized(m_chooseEnemy->pos() - m_pos);
        normalized.normalize();
        m_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) - 90;
    /*
     * 说明：
     * 角度减90度后攻击塔的指向为正右侧
     * qAtan2返回正弦值为y/x的弧度，转换成角度后为
     * 攻击塔由正向右转向敌人所转过的角度
    */

        // 如果敌人脱离攻击范围
        if (!collisionWithCircle(m_pos, m_attackRange, m_chooseEnemy->pos(), 1))
            lostSightOfEnemy();
    }
    else
    {
        // 遍历敌人,看是否有敌人在攻击范围内
        QList<Enemy *> enemyList = m_game->enemyList();
        foreach (Enemy *enemy, enemyList)
        {
            if (collisionWithCircle(m_pos, m_attackRange, enemy->pos(), 1))
            {
                chooseEnemyForAttack(enemy);
                break;
            }
        }
    }
}

bool Tower::positionInRange(QPoint &pos)
{
    if(collisionWithCircle(m_pos, 10, pos, 1))
        return true;
    else
        return false;
}

void Tower::chooseEnemyForAttack(Enemy *enemy)
{
    m_chooseEnemy = enemy;
    attackEnemy();
    m_chooseEnemy->getAttacked(this);
}

void Tower::shootWeapon()//******************************************************************************
{
    if(MainWindow::main_state == 0){
        Bullet *bullet = new Bullet(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game);
        bullet->move();

        QSoundEffect *m_place_tower = new QSoundEffect;
        m_place_tower->setSource(QUrl("qrc:new/music/shoot.wav"));
        m_place_tower->setLoopCount(1);
        m_place_tower->setVolume(0.5);
        m_place_tower->play();

        m_game->addBullet(bullet);
    }
    else return;
}

void Tower::targetKilled()
{
    if (m_chooseEnemy)
        m_chooseEnemy = NULL;

    m_fireRateTimer->stop();
    m_rotationSprite = 0.0;
}

void Tower::lostSightOfEnemy()
{
    m_chooseEnemy->gotLostSight(this);
    if (m_chooseEnemy)
        m_chooseEnemy = NULL;

    m_fireRateTimer->stop();
    m_rotationSprite = 0.0;
}

Enemy * Tower::getEnemy()
{
    return m_chooseEnemy;
}
