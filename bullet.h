#ifndef BULLET_H
#define BULLET_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>

class QPainter;
class Enemy;
class MainWindow;

class Bullet : QObject
{
    Q_OBJECT
    /*
     * Qt属性，生成子弹后，调用move方法
     * READ读取属性的值
     * WRITE设置属性的值
    */
    Q_PROPERTY(QPoint m_currentPos READ currentPos WRITE setCurrentPos)

public:
    Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,
           MainWindow *game, const QPixmap &sprite = QPixmap(":/new/image/bullet.png"));
    //~Bullet();

    void draw(QPainter *painter) const;
    void move();
    void setCurrentPos(QPoint pos);
    QPoint currentPos() const;

private slots:
    void hitTarget();

private:
    const QPoint m_startPos;//塔的位置
    const QPoint m_targetPos;//敌人的位置
    const QPixmap m_sprite;
    QPoint m_currentPos;//当前位置
    Enemy *	m_target;
    MainWindow * m_game;
    int	m_damage;

    static const QSize ms_fixedSize;
};

#endif // BULLET_H
