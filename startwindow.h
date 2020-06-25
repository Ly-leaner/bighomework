#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include<QMediaplayer>
#include<QMediaPlaylist>
#include"mainwindow.h"

class StartWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit StartWindow(QWidget *parent = nullptr);

    //重写绘图事件，画背景图
    void paintEvent(QPaintEvent */*event*/);

    MainWindow * w = NULL;
    QMediaPlayer *startSound/* = new QMediaPlayer*/;//游戏背景音乐
    QMediaPlaylist * musiclist;
    bool isBacking;//判断是从主界面第一次进入游戏，还是从游戏界面返回主界面后再次进入
    bool ispausing;//判断从暂停状态进入主界面


signals:

public slots:
};

#endif // STARTWINDOW_H
