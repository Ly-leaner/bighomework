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

    //************************************����-2020-06-25-9:50a.m.***********************************
    /*QMediaPlaylist **/musiclist = new QMediaPlaylist;//���������б�
    /*QMediaPlayer **/startSound = new QMediaPlayer;//��Ϸ��������
    musiclist->addMedia(QUrl("qrc:new/music/background.mp3"));//�������
    musiclist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);//ѭ������
    startSound->setPlaylist(musiclist);
    //startSound->setMedia(QUrl("qrc:new/music/background.mp3"));
    startSound->setVolume(20);

    startSound->play();

    //��ʼ��ť
    MyPushButton * startBtn = new MyPushButton(":/new/image/startbutton.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5 - startBtn->width()*0.5, this->height()*0.7);

    //������ť
    MyPushButton * helpBtn = new MyPushButton(":/new/image/helpbutton.png");
    helpBtn->setParent(this);
    helpBtn->move(this->width()*0.5 - helpBtn->width()*0.5 - 90, this->height()*0.7);

    //�˳���ť
    MyPushButton * closeBtn = new MyPushButton(":/new/image/closebutton.png");
    closeBtn->setParent(this);
    closeBtn->move(this->width()*0.5 - closeBtn->width()*0.5 + 90, this->height()*0.7);

    //ʵ������Ϸ������
    w = new MainWindow;

    //������Ϸ����ķ��ذ�ť���ź�
    connect(w, &MainWindow::chooseScenceBack, this, [=](){
        isBacking = true;
        if(w->checktimerActive()){
            w->timerStop();
        }
        else{
            if(!w->WinOrFail())
                ispausing = true;
        }
        w->hide();//������Ϸ������
        this->show();//������ʾ��ʼ����
    });

    connect(startBtn, &MyPushButton::clicked, [=](){
        //��������Ч
        startBtn->zoom1();
        startBtn->zoom2();

        //��ʱ 0.6s ���뵽��Ϸ����
        QTimer::singleShot(600,this,[=](){
            //��������
            this->hide();
            //��ʾ��Ϸ������
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
        //��������Ч
        closeBtn->zoom1();
        closeBtn->zoom2();
        //��ʱ 0.6s �˳���Ϸ����
        QTimer::singleShot(600,this,[=](){
            startSound->stop();
            this->close();
        });
    });

    connect(helpBtn, &MyPushButton::clicked, [=](){
        //��������Ч
        helpBtn->zoom1();
        helpBtn->zoom2();
        //��ʱ 0.6s ��ʾ��Ϸ˵��
        QTimer::singleShot(600,this,[=](){
            //*************************��ʵ��-2020-06-25************************************************************
            //�Ի��򣨷�ģ̬��
//            QDialog * dig = new QDialog(this);//�ڶ��������Ի���
//            dig->resize(200, 100);
//            dig->show();//��ģ̬
//            dig->setAttribute(Qt::WA_DeleteOnClose);//55������

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
