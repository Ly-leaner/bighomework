#ifndef ENEMY2_H
#define ENEMY2_H

#include"enemy.h"
#include <QObject>  //–≈∫≈”Î≤€
#include <QPoint>
#include <QSize>
#include <QPixmap>
#include"utilityfunction.h"

class WayPoint;
class QPainter;
class MainWindow;
class Tower;


class Enemy2 : public Enemy
{
public:
    Enemy2(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite = QPixmap(":/new/image/enemy2.png"));
    ~Enemy2();
    void draw(QPainter *painter) const;
};

#endif // ENEMY2_H
