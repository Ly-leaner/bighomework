#ifndef TOWER2_H
#define TOWER2_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>
#include<QSoundEffect>
#include"tower.h"

class QPainter;
class Enemy;
class MainWindow;
class QTimer;


class Tower2 : public Tower
{
public:
    Tower2(QPoint pos, MainWindow *game, const QSize &size = QSize(44,44), const QPixmap &sprite = QPixmap(":new/image/tower2.png"));
    ~Tower2();

};

#endif // TOWER2_H
