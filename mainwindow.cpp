#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QPainter>
#include <QMessageBox>
#include<QXmlStreamReader>
#include<QList>
#include <QMouseEvent>
#include <QtGlobal> /*qreal*/
#include <QTimer>
#include"plistreader.h"
#include "waypoint.h"
#include "enemy.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_waves(0),
    m_gameWin(false)
{
    ui->setupUi(this);

    loadTowerPositions();   //加载塔的位置信息，用于初始化游戏界面
    addWayPoints();
    preLoadWavesInfo();
    loadWave();


    //每30ms更新一次窗口，用于移动敌人
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    //相当于缓存cache
    QPixmap cachePix(":new/image/my.png");
    QPainter cachePainter(&cachePix);//设定绘图设备为背景图cachePix

    //使用foreach来遍历list
    //绘制安装塔的位置
    foreach (const TowerPosition &towerPos, m_towerPositionsList)
        towerPos.draw(&cachePainter);

    //绘制塔
    foreach (const Tower *tower, m_towersList)
        tower->draw(&cachePainter);

    //绘制路线航点
    foreach (const WayPoint *wayPoint, m_wayPointsList)
        wayPoint->draw(&cachePainter);

    //绘制敌人
    foreach (const Enemy *enemy, m_enemyList)
        enemy->draw(&cachePainter);

    //绘制背景图
    QPainter painter(this);
    painter.drawPixmap(0, 0, cachePix);
}

void MainWindow::loadTowerPositions()//用于从XML文件中读取塔安放的位置信息m_pos
{
    QFile file(":new/config/TowersPosition.plist");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
        return;
    }

    PListReader reader;
    reader.read(&file);

    QList<QVariant> array = reader.data();
    foreach (QVariant dict, array)
    {
        QMap<QString, QVariant> point = dict.toMap();//dict->QMap
        int x = point.value("x").toInt();//Returns the value associated with the key key->int
        int y = point.value("y").toInt();
        m_towerPositionsList.push_back(QPoint(x, y));
    }

    file.close();
//也可以通过数组，将塔的位置坐标添加到m_towerPositionsList中
//        QPoint pos[] =
//        {
//            QPoint(65, 220),
//            QPoint(155, 220),
//            QPoint(245, 220),
//            QPoint(335, 220),

//            QPoint(100, 125),
//            QPoint(195, 125),
//            QPoint(280, 125),
//            QPoint(370, 125),

//            QPoint(80, 35),
//            QPoint(170, 35),
//            QPoint(260, 35),
//            QPoint(350, 35)
//        };
//        int len = sizeof(pos) / sizeof(pos[0]);

//        for (int i = 0; i < len; ++i)
//            m_towerPositionsList.push_back(pos[i]);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint pressPos = event->pos();
    //auto在声明变量时根据变量初始值类型自动为此变量选择匹配的类型
    auto it = m_towerPositionsList.begin();
    while (it != m_towerPositionsList.end())
    {
        if (canBuyTower() && it->containPoint(pressPos) && !it->hasTower())
        {
            it->setHasTower();

            Tower *tower = new Tower(it->centerPos(), this);
            m_towersList.push_back(tower);
            update();
            break;
        }

        ++it;
    }
}

bool MainWindow::canBuyTower() const
{
    return true;
}

void MainWindow::addWayPoints()
{
    WayPoint *wayPoint1 = new WayPoint(QPoint(510, 380));
    m_wayPointsList.push_back(wayPoint1);

    WayPoint *wayPoint2 = new WayPoint(QPoint(84, 380));
    m_wayPointsList.push_back(wayPoint2);
    wayPoint2->setNextWayPoint(wayPoint1);

    WayPoint *wayPoint3 = new WayPoint(QPoint(84, 240));
    m_wayPointsList.push_back(wayPoint3);
    wayPoint3->setNextWayPoint(wayPoint2);

    WayPoint *wayPoint4 = new WayPoint(QPoint(410, 240));
    m_wayPointsList.push_back(wayPoint4);
    wayPoint4->setNextWayPoint(wayPoint3);

    WayPoint *wayPoint5 = new WayPoint(QPoint(410, 95));
    m_wayPointsList.push_back(wayPoint5);
    wayPoint5->setNextWayPoint(wayPoint4);

    WayPoint *wayPoint6 = new WayPoint(QPoint(35, 95));
    m_wayPointsList.push_back(wayPoint6);
    wayPoint6->setNextWayPoint(wayPoint5);
}

void MainWindow::getHpDamage(int damage/* = 1*/)
{}

void MainWindow::removedEnemy(Enemy *enemy)
{
    /*
     * Prints a warning message containing
     * the source code file name and line number
     * if test is false.
    */
    Q_ASSERT(enemy);

    m_enemyList.removeOne(enemy);
    delete enemy;

    if (m_enemyList.empty())
    {
        ++m_waves;//敌人的波数加一
        //加载下一波敌人
        if (!loadWave())
        {
            m_gameWin = true;
            // 游戏胜利转到游戏胜利场景
        }
    }
}



//槽函数
void MainWindow::updateMap()
{
    foreach (Enemy *enemy, m_enemyList)
        enemy->move();
    update();//更新窗口
}

void MainWindow::preLoadWavesInfo()
{
    QFile file(":new/config/Waves.plist");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
        return;
    }

    PListReader reader;
    reader.read(&file);

    // 获取波数信息
    m_wavesInfo = reader.data();

    file.close();
}

bool MainWindow::loadWave()
{
    if (m_waves >= m_wavesInfo.size())
        return false;

    WayPoint *startWayPoint = m_wayPointsList.back();   //list中的点是逆序放置的
    QList<QVariant> curWavesInfo = m_wavesInfo[m_waves].toList();

    for (int i = 0; i < curWavesInfo.size(); ++i)
    {
        QMap<QString, QVariant> dict = curWavesInfo[i].toMap();
        int spawnTime = dict.value("spawnTime").toInt();

        Enemy *enemy = new Enemy(startWayPoint, this);
        m_enemyList.push_back(enemy);
        QTimer::singleShot(spawnTime, enemy, SLOT(doActivate()));
        /*
         * [static] void QTimer::singleShot(int msec, const QObject *receiver, const char *member)
         * This static function calls a slot after a given time interval.
        */
    }

    return true;

    //利用数组实现波数控制
//    bool MainWindow::loadWave()
//    {
//        if (m_waves >= 6)
//            return false;
//        WayPoint *startWayPoint = m_wayPointsList.back(); // 这里是个逆序的，尾部才是其实节点
//        int enemyStartInterval[] = { 100, 500, 600, 1000, 3000, 6000 };
//        for (int i = 0; i < 6; ++i)
//        {
//            Enemy *enemy = new Enemy(startWayPoint, this);
//            m_enemyList.push_back(enemy);
//            QTimer::singleShot(enemyStartInterval[i], enemy, SLOT(doActivate()));
//        }
//        return true;
//    }
}


QList<Enemy *> MainWindow::enemyList() const
{
    return m_enemyList;
}


