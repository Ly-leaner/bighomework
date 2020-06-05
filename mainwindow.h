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

    void getHpDamage(int damage = 1);   //���˽�����غ󣬻��غ�Ѫ
    void removedEnemy(Enemy *enemy);
    QList<Enemy *> enemyList() const;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private slots:
    void updateMap();

private:
    Ui::MainWindow *ui;
    int	m_waves;//���˵Ĳ���

    void loadTowerPositions();  //���ڴ�XML�ļ��ж�ȡ�����ŵ�λ����Ϣm_pos
    bool canBuyTower() const;
    void addWayPoints();//����·�ߺ���
    void preLoadWavesInfo();
    bool loadWave();    //������һ�����˵���Ŀ�ͳ���ʱ��


    QList<TowerPosition> m_towerPositionsList;//��������λ����Ϣ
    QList<Tower *> m_towersList;   //����������Ϣ
    QList<WayPoint *> m_wayPointsList;    //����·�ߵ�
    QList<Enemy *> m_enemyList; //����·�ߵ�
    QList<QVariant>	m_wavesInfo; //������Ϣ

    bool m_gameWin;//�ж���Ϸ�Ƿ�ʤ��

};

#endif // MAINWINDOW_H
