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

    //÷ÿ–¥ªÊÕº ¬º˛£¨ª≠±≥æ∞Õº
    void paintEvent(QPaintEvent */*event*/);

    MainWindow * w = NULL;
    QMediaPlayer *startSound/* = new QMediaPlayer*/;//”Œœ∑±≥æ∞“Ù¿÷


signals:

public slots:
};

#endif // STARTWINDOW_H
