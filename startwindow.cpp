#include "startwindow.h"
#include<QPainter>
#include"mypushbutton.h"
#include"mainwindow.h"
#include<QTimer>
#include<QDialog>
#include<QMessageBox>
#include<QMediaplayer>
#include<QMediaPlaylist>

StartWindow::StartWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setFixedSize(750, 480);
    isBacking = false;
    ispausing = false;
    setWindowTitle("StartWindow");

    //************************************修正-2020-06-25-9:50a.m.***********************************
    /*QMediaPlaylist **/musiclist = new QMediaPlaylist;//创建播放列表
    /*QMediaPlayer **/startSound = new QMediaPlayer;//游戏背景音乐
    musiclist->addMedia(QUrl("qrc:new/music/background.mp3"));//添加音乐
    musiclist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);//循环播放
    startSound->setPlaylist(musiclist);
    //startSound->setMedia(QUrl("qrc:new/music/background.mp3"));
    startSound->setVolume(20);

    startSound->play();

    //开始按钮
    MyPushButton * startBtn = new MyPushButton(":/new/image/startbutton.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5 - startBtn->width()*0.5, this->height()*0.7);

    //帮助按钮
    MyPushButton * helpBtn = new MyPushButton(":/new/image/helpbutton.png");
    helpBtn->setParent(this);
    helpBtn->move(this->width()*0.5 - helpBtn->width()*0.5 - 90, this->height()*0.7);

    //退出按钮
    MyPushButton * closeBtn = new MyPushButton(":/new/image/closebutton.png");
    closeBtn->setParent(this);
    closeBtn->move(this->width()*0.5 - closeBtn->width()*0.5 + 90, this->height()*0.7);

    //实例化游戏主界面
    w = new MainWindow;

    //监听游戏界面的返回按钮的信号
    connect(w, &MainWindow::chooseScenceBack, this, [=](){
        isBacking = true;
        if(w->checktimerActive()){
            w->timerStop();
        }
        else{
            if(!w->WinOrFail())
                ispausing = true;
        }
        w->hide();//隐藏游戏主界面
        this->show();//重新显示开始界面
    });

    connect(startBtn, &MyPushButton::clicked, [=](){
        //做弹起特效
        startBtn->zoom1();
        startBtn->zoom2();

        //延时 0.6s 进入到游戏界面
        QTimer::singleShot(600,this,[=](){
            //自身隐藏
            this->hide();
            //显示游戏主界面
            w->show();
            if(isBacking==false){
                w->gameStart();
            }
            else{
                if(!ispausing){
                w->timerStart();
                isBacking = false;
                }
                else{
                    ispausing = false;
                }
            }
        });
    });

    connect(closeBtn, &MyPushButton::clicked, [=](){
        //做弹起特效
        closeBtn->zoom1();
        closeBtn->zoom2();
        //延时 0.6s 退出游戏界面
        QTimer::singleShot(600,this,[=](){
            startSound->stop();
            this->close();
        });
    });

    connect(helpBtn, &MyPushButton::clicked, [=](){
        //做弹起特效
        helpBtn->zoom1();
        helpBtn->zoom2();
        //延时 0.6s 显示游戏说明
        QTimer::singleShot(600,this,[=](){
            //*************************待实现-2020-06-25************************************************************
            //对话框（非模态）
//            QDialog * dig = new QDialog(this);//在堆区创建对话框
//            dig->resize(200, 100);
//            dig->show();//非模态
//            dig->setAttribute(Qt::WA_DeleteOnClose);//55号属性

            QMessageBox * dig = new QMessageBox(this);
            dig->setWindowTitle("Rule Description");
            dig->setInformativeText("Dear player,\n"
                                    "    Here is a simple description of the game rules:\n"
                                    "    Click on the square tower base marked on the map to install"
                                    "the defense tower and prevent the enemy from entering the house."
                                    "You will have three chances to make a mistake, which means that"
                                    "the game will fail when the fourth enemy enters the house.\n"
                                    "\n"
                                    "Mouse operation:\n"
                                    "Left-click to install the first defense tower.\n"
                                    "Right-click to install the second defense tower.\n"
                                    "Double left-click to remove the defense tower.\n"
                                    "\n"
                                    "Good luck!");
            dig->exec();

        });
    });
}


void StartWindow::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/new/image/start.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

}
