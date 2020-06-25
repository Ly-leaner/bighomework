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
    Q_OBJECT//�����ź����
public:
    Tower(QPoint pos, MainWindow *game,  const QSize &size = QSize(44,44), const QPixmap &sprite = QPixmap(":new/image/tower.png"));
    virtual ~Tower();

    void draw(QPainter *painter) const;
    void checkEnemyInRange();
    void targetKilled();
    void attackEnemy();
    void chooseEnemyForAttack(Enemy *enemy);
    void removeBullet();
    void damageEnemy();
    void lostSightOfEnemy();
    bool positionInRange(QPoint & pos);//���������Ƴ�-----------------2020-06-26(00:15)-----------------------------
    void timeStop();

private slots:
    void shootWeapon();

public://------------------------new------------------------------------------------
    //QSoundEffect * m_place_tower;
    bool m_attacking;
    int m_attackRange;	// ���������Թ��������˵ľ���
    int m_damage;		// ����������ʱ��ɵ��˺�
    int	m_fireRate;		// �����ٴι������˵�ʱ����
    qreal/*double*/	m_rotationSprite;//�Ƕ�

    Enemy * m_chooseEnemy;
    Enemy * getEnemy();

    MainWindow * m_game;
    QTimer * m_fireRateTimer;   //���ô���Ƶ��

    const QPoint m_pos;
    const QPixmap m_sprite;

    const QSize ms_fixedSize;
};

#endif // TOWER_H
