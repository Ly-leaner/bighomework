#ifndef TOWER_H
#define TOWER_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>
#include<QSoundEffect>

class QPainter;
class Enemy;
class MainWindow;
class QTimer;

class Tower : QObject
{
    Q_OBJECT//处理信号与槽
public:
    Tower(QPoint pos, MainWindow *game, const QPixmap &sprite = QPixmap(":new/image/tower.png"));
    ~Tower();

    void draw(QPainter *painter) const;
    void checkEnemyInRange();
    void targetKilled();
    void attackEnemy();
    void chooseEnemyForAttack(Enemy *enemy);
    void removeBullet();
    void damageEnemy();
    void lostSightOfEnemy();

private slots:
    void shootWeapon();

private:
    //QSoundEffect * m_place_tower;
    bool m_attacking;
    int m_attackRange;	// 代表塔可以攻击到敌人的距离
    int m_damage;		// 代表攻击敌人时造成的伤害
    int	m_fireRate;		// 代表再次攻击敌人的时间间隔
    qreal/*double*/	m_rotationSprite;//角度

    Enemy * m_chooseEnemy;
    MainWindow * m_game;
    QTimer * m_fireRateTimer;   //设置打炮频率

    const QPoint m_pos;
    const QPixmap m_sprite;

    static const QSize ms_fixedSize;
};

#endif // TOWER_H
