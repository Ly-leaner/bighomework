#include "mypushbutton.h"
#include<QDebug>
#include<qpropertyanimation>

MyPushButton::MyPushButton(QWidget *parent) : QPushButton(parent)
{

}

MyPushButton::MyPushButton(QString normalImg, QString pressImg/* = ""*/)
{
    this->normalImgPath =  normalImg;
    this->pressImgPath = pressImg;
    state = 0;

    QPixmap pix;
    bool ret = pix.load(normalImg);
    if(!ret){
        qDebug() << "fail to load pix!";
        return;
    }

    //����ͼƬ�̶���С
    this->setFixedSize(pix.width(), pix.height());

    //���ò�����ͼƬ����ʽ
    this->setStyleSheet("QPushButton{border:0px;}");

    //����ͼ��
    this->setIcon(pix);
    //����ͼ���С
    this->setIconSize(QSize(pix.width(), pix.height()));



}


void MyPushButton::zoom1()
{
    //����������Ч
    QPropertyAnimation * animation = new QPropertyAnimation(this, "geometry");
    //���ö�����ʱ����
    animation->setDuration(200);

    //��ʼλ��
    animation->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    //����λ��
    animation->setEndValue(QRect(this->x(), this->y()+5, this->width(), this->height()));

    //���õ�������
    animation->setEasingCurve(QEasingCurve::OutBounce);

    //ִ�ж���
    animation->start();

}

void MyPushButton::zoom2()
{
    //����������Ч
    QPropertyAnimation * animation = new QPropertyAnimation(this, "geometry");
    //���ö�����ʱ����
    animation->setDuration(300);

    //��ʼλ��
    animation->setStartValue(QRect(this->x(), this->y()+5, this->width(), this->height()));
    //����λ��
    animation->setEndValue(QRect(this->x(), this->y(), this->width(), this->height()));

    //���õ�������
    animation->setEasingCurve(QEasingCurve::OutBounce);

    //ִ�ж���
    animation->start();
}

void MyPushButton::mousePressEvent(QMouseEvent *e)
{
    if(this->pressImgPath!="")//����İ���ͼƬ��Ϊ�� ˵����Ҫ�а��µ�״̬
    {
        QPixmap pix;
        if(state == 0){
            bool ret = pix.load(this->pressImgPath);
            state = 1;
            if(!ret){
                qDebug() << "fail to load pix!";
                return;
            }
        }
        else{
            bool ret = pix.load(this->normalImgPath);
            state = 0;
            if(!ret){
                qDebug() << "fail to load pix!";
                return;
            }
        }


        //����ͼƬ�̶���С
        this->setFixedSize(pix.width(), pix.height());

        //���ò�����ͼƬ����ʽ
        this->setStyleSheet("QPushButton{border:0px;}");

        //����ͼ��
        this->setIcon(pix);
        //����ͼ���С
        this->setIconSize(QSize(pix.width(), pix.height()));

    }
    else{}

    //�ø���ִ������������
    return QPushButton::mousePressEvent(e);

}

//void MyPushButton::mouseReleaseEvent(QMouseEvent *e)
//{

//}





