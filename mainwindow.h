#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QList>
#include "towerposition.h"
#include"tower.h"
#include"waypoint.h"
#include"enemy.h"
#include<QMediaplayer>
#include<QMediaPlaylist>
#include<QLabel>
#include"mypushbutton.h"

class WayPoint;
class Enemy;
class Bullet;

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
    void removedBullet(Bullet *bullet);
    void addBullet(Bullet *bullet);
    void awardGold(int gold);
    void gameRestart();
    int Wave_num();

    static int main_state;//状态，用于暂停，0表示正常，1表示暂停
    //static int main_state_restart;//状态，用于游戏重新开始，0表示正常，1表示重新开始

    QLabel * WinLabel;//胜利图标
    QLabel * FailLabel;//失败图标
    MyPushButton * restart;//重新开始按钮

    QList<Enemy *> enemyList() const;
    //QMediaPlaylist *musiclist/* = new QMediaPlaylist*/;//创建播放列表

signals:
    //自定义信号，告知开始界面点击了返回
    //自定义信号只需要声明，不需要实现
    void chooseScenceBack();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private slots:
    void updateMap();
    void gameStart();

private:
    Ui::MainWindow *ui;
//    QMediaPlayer *startSound/* = new QMediaPlayer*/;//游戏背景音乐
    QTimer * timer;

    void loadTowerPositions();  //用于从XML文件中读取塔安放的位置信息m_pos
    bool canBuyTower() const;
    void addWayPoints();//绘制路线航点
    void preLoadWavesInfo();
    bool loadWave();    //加载下一波敌人的数目和出现时间
    void drawWave(QPainter *painter);
    void drawHP(QPainter *painter);
    void drawPlayerGold(QPainter *painter);
    void doGameOver();
    //void awardGold(int gold);


    QList<TowerPosition> m_towerPositionsList;//管理塔的位置信息
    QList<Tower *> m_towersList;   //管理塔的信息
    QList<WayPoint *> m_wayPointsList;    //管理路线点
    QList<Enemy *> m_enemyList; //管理路线点
    QList<QVariant>	m_wavesInfo; //波数信息
    QList<Bullet *>	m_bulletList; //子弹

    int	m_waves;//敌人的波数，可以用于控制敌人移动速度
    bool m_gameWin;//判断游戏是否胜利
    bool m_gameEnded;
    int	m_playerHp;     //基地的血量
    int	m_playrGold;

};

#endif // MAINWINDOW_H
