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

    void getHpDamage(int damage = 1);   //���˽�����غ󣬻��غ�Ѫ
    void removedEnemy(Enemy *enemy);
    void removedBullet(Bullet *bullet);
    void addBullet(Bullet *bullet);
    void awardGold(int gold);
    void gameRestart();
    int Wave_num();

    static int main_state;//״̬��������ͣ��0��ʾ������1��ʾ��ͣ
    //static int main_state_restart;//״̬��������Ϸ���¿�ʼ��0��ʾ������1��ʾ���¿�ʼ

    QLabel * WinLabel;//ʤ��ͼ��
    QLabel * FailLabel;//ʧ��ͼ��
    MyPushButton * restart;//���¿�ʼ��ť

    QList<Enemy *> enemyList() const;
    //QMediaPlaylist *musiclist/* = new QMediaPlaylist*/;//���������б�

signals:
    //�Զ����źţ���֪��ʼ�������˷���
    //�Զ����ź�ֻ��Ҫ����������Ҫʵ��
    void chooseScenceBack();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private slots:
    void updateMap();
    void gameStart();

private:
    Ui::MainWindow *ui;
//    QMediaPlayer *startSound/* = new QMediaPlayer*/;//��Ϸ��������
    QTimer * timer;

    void loadTowerPositions();  //���ڴ�XML�ļ��ж�ȡ�����ŵ�λ����Ϣm_pos
    bool canBuyTower() const;
    void addWayPoints();//����·�ߺ���
    void preLoadWavesInfo();
    bool loadWave();    //������һ�����˵���Ŀ�ͳ���ʱ��
    void drawWave(QPainter *painter);
    void drawHP(QPainter *painter);
    void drawPlayerGold(QPainter *painter);
    void doGameOver();
    //void awardGold(int gold);


    QList<TowerPosition> m_towerPositionsList;//��������λ����Ϣ
    QList<Tower *> m_towersList;   //����������Ϣ
    QList<WayPoint *> m_wayPointsList;    //����·�ߵ�
    QList<Enemy *> m_enemyList; //����·�ߵ�
    QList<QVariant>	m_wavesInfo; //������Ϣ
    QList<Bullet *>	m_bulletList; //�ӵ�

    int	m_waves;//���˵Ĳ������������ڿ��Ƶ����ƶ��ٶ�
    bool m_gameWin;//�ж���Ϸ�Ƿ�ʤ��
    bool m_gameEnded;
    int	m_playerHp;     //���ص�Ѫ��
    int	m_playrGold;

};

#endif // MAINWINDOW_H
