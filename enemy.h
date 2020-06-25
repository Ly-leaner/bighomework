#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>  //信号与槽
#include <QPoint>
#include <QSize>
#include <QPixmap>

class WayPoint;
class QPainter;
class MainWindow;
class Tower;

class Enemy : public QObject
{
    Q_OBJECT
public:
    Enemy(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite = QPixmap(":/new/image/enemy.png"));
    ~Enemy();

    void draw(QPainter *painter) const;
    void move();
    void getDamage(int damage);
    void getRemoved();
    void getAttacked(Tower *attacker);
    void gotLostSight(Tower *attacker);
    QPoint pos() const;
    //*****************************************
    //bool exist();//状态，避免重复析构，1表示未析构，0表示被析构

public slots:
    void doActivate();

private:
    //******************************************
    //int state_distructed;//状态，用于析构
    bool m_active;   //判断敌人是否可以移动
    int m_maxHp;    //敌人的血量
    int	m_currentHp;    //当前的血量
    double m_walkingSpeed; //移动速度
    qreal m_rotationSprite;      //改变行进方向时转过的角度

    QPoint m_pos;  //当前坐标，表示中心，绘制的时候需要偏移
    WayPoint * m_destinationWayPoint;  //目标点的位置
    MainWindow * m_game;    //用于敌人进入基地后调用移除函数
    QList<Tower *> m_attackedTowersList;    //攻击此敌人的塔*************重新开始游戏时需要清除**********************

    const QPixmap m_sprite;   //图片
    static const QSize ms_fixedSize;       //尺寸
};

#endif // ENEMY_H
