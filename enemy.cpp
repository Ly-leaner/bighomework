#include "enemy.h"
#include "waypoint.h"
#include "tower.h"
#include "utilityfunction.h"//ֻ����һ����ײ��⺯��
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
        return;//�����ƶ��򷵻�

    if (collisionWithCircle(m_pos, 1, m_destinationWayPoint->pos(), 1))
    {// ���˵ִ���һ������
        if (m_destinationWayPoint->nextWayPoint())
        {// ������һ������
            m_pos = m_destinationWayPoint->pos();//�ƶ�����һ������
            m_destinationWayPoint = m_destinationWayPoint->nextWayPoint();
        }
        else
        {// ��ʾ�������
            m_game->getHpDamage();//���غ�Ѫ
            m_game->removedEnemy(this);//�Ƴ�����
            return;
        }
    }

    // ����ǰ�������·��
    // Ŀ�꺽�������
    QPoint targetPoint = m_destinationWayPoint->pos();

    // �޸������������ƶ�״̬,�ӿ�,����,m_walkingSpeed�ǻ�׼ֵ
    // ������׼��
    double movementSpeed = m_walkingSpeed + (double)m_game->Wave_num()/2.0;
    QVector2D/*��ά����*/ normalized(targetPoint - m_pos);
    normalized.normalize();//��ά������׼��
    //��ǰλ��λ��+=��׼����ά����*�ƶ��ٶ�
    m_pos = m_pos + normalized.toPoint() * movementSpeed;

    // ȷ������ѡ����
    // Ĭ��ͼƬ����,����180��ת��
    m_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) + 180;

    /*
     * qRadiansToDegrees��This function converts the radians in double to degrees������->�Ƕ�
     * qAtan2��This function will return the angle (argument) of that point
    */
}

void Enemy::draw(QPainter *painter) const
{
    if (!m_active)
        return;//�����ƶ��򷵻�

    painter->save();

    QPoint healthBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5/*����Ѫ��λ�ã�ʹ֮λ�ڵ����Ϸ�*/, -ms_fixedSize.height() / 3);
    // ����Ѫ��
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, 2));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * Health_Bar_Width, 2));
    painter->drawRect(healthBarRect);

    // ����ƫת����,������+ƫ��->����
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(m_pos);  //�������������ɻ�ͼ�豸�����Ͻ�ת������������
    painter->rotate(m_rotationSprite);  //��ͼ����ϵ˳ʱ��ת��һ���Ƕȣ�ֱ�ǣ�
    // ���Ƶ���
    painter->drawPixmap(offsetPoint, m_sprite);//��ͼ����λ�ڵ������Ͻ�

    painter->restore();
}



void Enemy::getAttacked(Tower *attacker)
{
    m_attackedTowersList.push_back(attacker);
}

// ���������Ѿ������˹�����Χ
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
    // ֪ͨgame,�˵����Ѿ�����
    m_game->removedEnemy(this);
}

void Enemy::getDamage(int damage)//
{
    m_currentHp -= damage;

    // ����,��Ҫ�Ƴ�
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


