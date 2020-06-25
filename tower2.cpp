#include "tower2.h"

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
#include<QSize>

//const QSize Tower2::ms_fixedSize2(70, 70);

Tower2::Tower2(QPoint pos, MainWindow *game, const QSize &size/* = QSize(44,44)*/, const QPixmap &sprite/* = QPixmap(":/image/tower2.png"*/)
    :Tower(pos, game, size, sprite)
{
    m_fireRate = 500;
}

Tower2::~Tower2()
{
    delete m_fireRateTimer;
    m_fireRateTimer = NULL;
}
