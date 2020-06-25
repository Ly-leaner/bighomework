#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"mypushbutton.h"

#include<QPainter>
#include<QMessageBox>
#include<QXmlStreamReader>
#include<QList>
#include<QMouseEvent>
#include<QtGlobal> /*qreal*/
#include<QTimer>
#include"plistreader.h"
#include"waypoint.h"
#include"enemy.h"
#include"bullet.h"
#include<QMediaplayer>
#include<QMediaPlaylist>
#include<QSoundEffect>
#include<QPropertyAnimation>
#include<QDebug>
#include<QMessageBox>


static const int TowerCost = 300;

int MainWindow::main_state = 0;
//int MainWindow::main_state_restart = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_waves(0),
    m_gameWin(false),
    m_gameEnded(false),
    m_playerHp(3),
    m_playrGold(1000)
    //startSound(NULL),
    //musiclist(NULL)
{
    ui->setupUi(this);

    //添加返回按钮
    MyPushButton * back = new MyPushButton(":new/image/backbutton.png");
    back->setParent(this);
    back->move(this->width() - back->width()*1.2, 70);
    //点击返回
    connect(back, &MyPushButton::clicked, [=](){
         //告知开始界面要返回 开始界面监听mainWindow的返回按钮

        back->zoom1();
        back->zoom2();

        //延时 0.6s 返回到初始界面
        QTimer::singleShot(600,this,[=](){
            emit this->chooseScenceBack();
        });

    });

    //*****************************************************************
    //胜利和失败图片显示，先画到窗口外，胜利或失败时移到中央
    WinLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":new/image/win.png");
    WinLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    WinLabel->setPixmap(tmpPix);
    WinLabel->setParent(this);
    WinLabel->move((this->width() - tmpPix.width())*0.5, -tmpPix.height());

    FailLabel = new QLabel;
    QPixmap temp;
    temp.load(":new/image/fail.png");
    FailLabel->setGeometry(0,0,temp.width(),temp.height());
    FailLabel->setPixmap(temp);
    FailLabel->setParent(this);
    FailLabel->move((this->width() - temp.width())*0.5, -temp.height());
//*******************************************************************************


    loadTowerPositions();   //加载塔的位置信息，用于初始化游戏界面
    addWayPoints();
    preLoadWavesInfo();
    //loadWave();


    //每30ms更新一次窗口，用于移动敌人
    /*QTimer **/timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);

    //添加暂停按钮******************************************************************************************
    MyPushButton * pause = new MyPushButton(":new/image/pause.png", ":new/image/continue.png");
    pause->setParent(this);
    pause->move(this->width() - pause->width()*2, 30);

    //点击暂停**************************功能待实现-2020-06-25*************************************************
    connect(pause, &MyPushButton::clicked, [=](){
       pause->zoom1();
       pause->zoom2();
       //延时 0.5s 暂停/继续游戏界面
       QTimer::singleShot(500,this,[=](){
           if(timer->isActive()){
               timer->stop();
               main_state = 1;
           }
           else{
               timer->start();
               main_state = 0;
           }
       });

    });//*************************实现-2020-6-25**************************************************************

    //添加重新开始按钮***************实现-2020-6-25**************************************************************
    MyPushButton * restart = new MyPushButton(":new/image/restartbutton.png");
    restart->setParent(this);
    restart->move(this->width() - restart->width()*1.2, 150);
    connect(restart, &MyPushButton::clicked, [=](){
        //结束游戏
        restart->zoom1();
        restart->zoom2();
        timer->stop();
        main_state = 1;
        //m_gameEnded = true;
        //延时 0.6s 重新开始
        QTimer::singleShot(600,this,[=](){
            if (m_gameEnded || m_gameWin){
                gameRestart();
                timer->start();
                main_state = 0;
                gameStart();
            }
            else{
                QMessageBox * dig = new QMessageBox(this);
                dig->setWindowTitle("Prompt Information");
                dig->setInformativeText("You cannot restart the game until the game is over.");
                dig->exec();
                timer->start();
                main_state = 0;
            }
        });
    });


    //设置500ms后游戏开始
    QTimer::singleShot(100, this, SLOT(gameStart()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    if (m_gameEnded || m_gameWin)//*********此处应将return改为游戏暂停或结束（未实现），仍保留结束时的场景*************************
    {

//        QString text = m_gameEnded ? "YOU fail!!!" : "YOU WIN!!!";
//        QPainter painter(this);
//        painter.setPen(QPen(Qt::red));
//        painter.drawText(rect(), Qt::AlignCenter, text);
        return;
    }

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

    //子弹
    foreach (const Bullet *bullet, m_bulletList)
        bullet->draw(&cachePainter);

    this->drawWave(&cachePainter);//波数
    drawHP(&cachePainter);
    drawPlayerGold(&cachePainter);

    //绘制背景图
    QPainter painter(this);
    painter.drawPixmap(0, 0, cachePix);
}

void MainWindow::loadTowerPositions()//用于从XML文件中读取塔安放的位置信息m_pos
{
    QFile file(":/new/config/TowersPosition.plist");
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
            m_playrGold -= TowerCost;
            update();


            QSoundEffect *m_place_tower = new QSoundEffect;
            m_place_tower->setSource(QUrl("qrc:new/music/place_tower.wav"));
            m_place_tower->setLoopCount(1);
            m_place_tower->setVolume(0.5);
            m_place_tower->play();


            break;
        }

        ++it;
    }
}

bool MainWindow::canBuyTower() const
{
    if (m_playrGold >= TowerCost)
        return true;
    return false;
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
{
    m_playerHp -= damage;

    QSoundEffect *m_place_tower = new QSoundEffect;
    m_place_tower->setSource(QUrl("qrc:new/music/lose_life.wav"));
    m_place_tower->setLoopCount(1);
    m_place_tower->setVolume(0.5);
    m_place_tower->play();

    if (m_playerHp <= 0)

        doGameOver();
}

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
            //restart->move(this->width() - restart->width()*1.2, 150);//************2020-06-25-14:00*****************
            // 游戏胜利转到游戏胜利场景****************************************************************************
            //将胜利的图片移动下来
            QPropertyAnimation * animation = new QPropertyAnimation(WinLabel, "geometry");
            //设置时间间隔
            animation->setDuration(1000);
            //设置开始位置
            animation->setStartValue(QRect(WinLabel->x(), WinLabel->y(), WinLabel->width(), WinLabel->height()));

            //设置结束位置
            animation->setEndValue(QRect(WinLabel->x(), WinLabel->y()+300, WinLabel->width(), WinLabel->height()));

            //设置缓和曲线
            animation->setEasingCurve(QEasingCurve::OutBounce);
            //执行动画
            animation->start();
        }//****************************************************************************************************
    }
}



//槽函数
void MainWindow::updateMap()
{
    foreach (Enemy *enemy, m_enemyList)
        enemy->move();
    foreach (Tower *tower, m_towersList)
        tower->checkEnemyInRange();
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

void MainWindow::removedBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);

    m_bulletList.removeOne(bullet);
    delete bullet;
}

void MainWindow::addBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);

    m_bulletList.push_back(bullet);
}

void MainWindow::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::black));
    painter->drawText(QPoint(600, 18), QString("WAVE : %1").arg(m_waves + 1));
}

void MainWindow::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::black));
    painter->drawText(QPoint(30, 18), QString("HP : %1").arg(m_playerHp));
}

void MainWindow::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::black));
    painter->drawText(QPoint(330, 18), QString("GOLD : %1").arg(m_playrGold));
}

void MainWindow::doGameOver()
{
    if (!m_gameEnded)
    {
        m_gameEnded = true;//**************************************************************************

        //计时器结束
        timer->stop();
        //restart->move(this->width() - restart->width()*1.2, 150);//************2020-06-25-14:00*****************

        QPropertyAnimation * animation = new QPropertyAnimation(FailLabel, "geometry");
        //设置时间间隔
        animation->setDuration(1000);
        //设置开始位置
        animation->setStartValue(QRect(FailLabel->x(), FailLabel->y(), FailLabel->width(), FailLabel->height()));

        //设置结束位置
        animation->setEndValue(QRect(FailLabel->x(), FailLabel->y()+350, FailLabel->width(), FailLabel->height()));

        //设置缓和曲线
        animation->setEasingCurve(QEasingCurve::OutBounce);
        //执行动画
        animation->start();

//        if(startSound)
//        startSound->stop();
        // 此处应该切换场景到结束场景
        // 暂时以打印替代,见paintEvent处理***************已实现于上方-2020-06-25**********************************************************
    }
}

void MainWindow::gameStart()
{
//    //************************************修正-2020-06-25-9:50a.m.，将之移动到startwindown**********************************************
//    QMediaPlaylist *musiclist = new QMediaPlaylist;//创建播放列表
//    /*QMediaPlayer **/startSound = new QMediaPlayer;//游戏背景音乐
//    startSound->setMedia(QUrl("qrc:new/music/background.mp3"));
//    startSound->setVolume(20);
//    musiclist->addMedia(QUrl("qrc:new/music/background.mp3"));//添加音乐
//    musiclist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);//循环播放

//    startSound->play();
    loadWave();
}

void MainWindow::awardGold(int gold)
{
    if(m_playrGold<=1200)
        m_playrGold += gold;
    update();
}

int MainWindow::Wave_num()
{
    return m_waves;
}

void MainWindow::gameRestart()
{
    //main_state_restart = 1;

    if(m_gameWin){
        WinLabel->move((this->width() - 300)*0.5, -300);
    }
    else{
        FailLabel->move((this->width() - 300)*0.5, -300);
    }

    m_waves = 0;
    m_gameWin = false;
    m_gameEnded = false;
    m_playerHp = 3;
    m_playrGold = 1000;

    foreach (const Enemy *enemy, m_enemyList)
        enemy->~Enemy();

    m_towerPositionsList.clear();
    m_towersList.clear();
//    if(m_towersList.empty()){
//        qDebug()<<"empty";
//    }
    m_wayPointsList.clear();
    m_enemyList.clear();
    m_wavesInfo.clear();
    m_bulletList.clear();
//        if(m_bulletList.empty()){
//            qDebug()<<"empty";
//        }


    loadTowerPositions();   //加载塔的位置信息，用于初始化游戏界面
    addWayPoints();
    preLoadWavesInfo();

}




