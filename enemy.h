#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>  //�ź����
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

public slots:
    void doActivate();

private:
    bool m_active;   //�жϵ����Ƿ�����ƶ�
    int m_maxHp;    //���˵�Ѫ��
    int	m_currentHp;    //��ǰ��Ѫ��
    double m_walkingSpeed; //�ƶ��ٶ�
    qreal m_rotationSprite;      //�ı��н�����ʱת���ĽǶ�

    QPoint m_pos;  //��ǰ���꣬��ʾ���ģ����Ƶ�ʱ����Ҫƫ��
    WayPoint * m_destinationWayPoint;  //Ŀ����λ��
    MainWindow * m_game;    //���ڵ��˽�����غ�����Ƴ�����
    QList<Tower *> m_attackedTowersList;    //�����˵��˵���

    const QPixmap m_sprite;   //ͼƬ
    static const QSize ms_fixedSize;       //�ߴ�
};

#endif // ENEMY_H
