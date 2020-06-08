#include "mainwindow.h"
#include "ui_mainwindow.h"

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

static const int TowerCost = 300;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_waves(0),
    m_gameWin(false),
    m_gameEnded(false),
    m_playerHp(5),
    m_playrGold(1000),
    startSound(NULL),
    musiclist(NULL)
{
    ui->setupUi(this);

    loadTowerPositions();   //��������λ����Ϣ�����ڳ�ʼ����Ϸ����
    addWayPoints();
    preLoadWavesInfo();
    //loadWave();


    //ÿ30ms����һ�δ��ڣ������ƶ�����
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);



    //����500ms����Ϸ��ʼ
    QTimer::singleShot(500, this, SLOT(gameStart()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    if (m_gameEnded || m_gameWin)
    {
        QString text = m_gameEnded ? "YOU fail!!!" : "YOU WIN!!!";
        QPainter painter(this);
        painter.setPen(QPen(Qt::red));
        painter.drawText(rect(), Qt::AlignCenter, text);
        return;
    }

    //�൱�ڻ���cache
    QPixmap cachePix(":new/image/my.png");
    QPainter cachePainter(&cachePix);//�趨��ͼ�豸Ϊ����ͼcachePix

    //ʹ��foreach������list
    //���ư�װ����λ��
    foreach (const TowerPosition &towerPos, m_towerPositionsList)
        towerPos.draw(&cachePainter);

    //������
    foreach (const Tower *tower, m_towersList)
        tower->draw(&cachePainter);

    //����·�ߺ���
    foreach (const WayPoint *wayPoint, m_wayPointsList)
        wayPoint->draw(&cachePainter);

    //���Ƶ���
    foreach (const Enemy *enemy, m_enemyList)
        enemy->draw(&cachePainter);

    //�ӵ�
    foreach (const Bullet *bullet, m_bulletList)
        bullet->draw(&cachePainter);

    this->drawWave(&cachePainter);//����
    drawHP(&cachePainter);
    drawPlayerGold(&cachePainter);

    //���Ʊ���ͼ
    QPainter painter(this);
    painter.drawPixmap(0, 0, cachePix);
}

void MainWindow::loadTowerPositions()//���ڴ�XML�ļ��ж�ȡ�����ŵ�λ����Ϣm_pos
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
//Ҳ����ͨ�����飬������λ��������ӵ�m_towerPositionsList��
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
    //auto����������ʱ���ݱ�����ʼֵ�����Զ�Ϊ�˱���ѡ��ƥ�������
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
        ++m_waves;//���˵Ĳ�����һ
        //������һ������
        if (!loadWave())
        {
            m_gameWin = true;
            // ��Ϸʤ��ת����Ϸʤ������
        }
    }
}



//�ۺ���
void MainWindow::updateMap()
{
    foreach (Enemy *enemy, m_enemyList)
        enemy->move();
    foreach (Tower *tower, m_towersList)
        tower->checkEnemyInRange();
    update();//���´���
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

    // ��ȡ������Ϣ
    m_wavesInfo = reader.data();

    file.close();
}

bool MainWindow::loadWave()
{
    if (m_waves >= m_wavesInfo.size())
        return false;

    WayPoint *startWayPoint = m_wayPointsList.back();   //list�еĵ���������õ�
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

    //��������ʵ�ֲ�������
//    bool MainWindow::loadWave()
//    {
//        if (m_waves >= 6)
//            return false;
//        WayPoint *startWayPoint = m_wayPointsList.back(); // �����Ǹ�����ģ�β��������ʵ�ڵ�
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
        m_gameEnded = true;

        if(startSound)
        startSound->stop();
        // �˴�Ӧ���л���������������
        // ��ʱ�Դ�ӡ���,��paintEvent����
    }
}

void MainWindow::gameStart()
{
    QMediaPlaylist *musiclist = new QMediaPlaylist;//���������б�
    QMediaPlayer *startSound = new QMediaPlayer;//��Ϸ��������
    startSound->setMedia(QUrl("qrc:new/music/background.mp3"));
    startSound->setVolume(100);
    musiclist->addMedia(QUrl("qrc:new/music/background.mp3"));//�������
    musiclist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);//ѭ������

    startSound->play();
    loadWave();
}

void MainWindow::awardGold(int gold)
{
    m_playrGold += gold;
    update();
}




