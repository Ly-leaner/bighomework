#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(QWidget *parent = nullptr);

    //构造函数，参数一为正常显示的图片路径，参数二为按下后显示的图片路径
    MyPushButton(QString normalImg, QString pressImg = "");

    //成员属性，保存用户传入的默认显示路径 以及按下后显示的图片路径
    QString normalImgPath;
    QString pressImgPath;
    int state;//状态，当构造函数有第二个参数时，用0标记按下前为第一张图片，用1标记按下前是第二张图片

    //弹跳特效
    void zoom1();//up
    void zoom2();//down

    //重写按钮的按下和释放事件
    void mousePressEvent(QMouseEvent *e);

//    void mouseReleaseEvent(QMouseEvent *e);

signals:

public slots:
};

#endif // MYPUSHBUTTON_H
