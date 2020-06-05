#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QList>
#include "towerposition.h"
#include"tower.h"
#include"waypoint.h"
#include"enemy.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void getHpDamage(int damage = 1);   //敌人进入基地后，基地耗血
    void removedEnemy(Enemy *enemy);
    QList<Enemy *> enemyList() const;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private slots:
    void updateMap();

private:
    Ui::MainWindow *ui;
    int	m_waves;//敌人的波数

    void loadTowerPositions();  //用于从XML文件中读取塔安放的位置信息m_pos
    bool canBuyTower() const;
    void addWayPoints();//绘制路线航点
    void preLoadWavesInfo();
    bool loadWave();    //加载下一波敌人的数目和出现时间


    QList<TowerPosition> m_towerPositionsList;//管理塔的位置信息
    QList<Tower *> m_towersList;   //管理塔的信息
    QList<WayPoint *> m_wayPointsList;    //管理路线点
    QList<Enemy *> m_enemyList; //管理路线点
    QList<QVariant>	m_wavesInfo; //波数信息

    bool m_gameWin;//判断游戏是否胜利

};

#endif // MAINWINDOW_H
