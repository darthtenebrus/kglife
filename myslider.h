//
// Created by esorochinskiy on 23.11.23.
//

#ifndef KQSNAKE_MYSLIDER_H
#define KQSNAKE_MYSLIDER_H
#include <QSlider>

class MySlider : public QSlider {
Q_OBJECT

public:
    explicit MySlider(Qt::Orientation orientation, QWidget *parent = nullptr);
    void retranslateUi();


};


#endif //KQSNAKE_MYSLIDER_H
