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

    //��д��ͼ�¼���������ͼ
    void paintEvent(QPaintEvent */*event*/);

    MainWindow * w = NULL;
    QMediaPlayer *startSound/* = new QMediaPlayer*/;//��Ϸ��������


signals:

public slots:
};

#endif // STARTWINDOW_H
