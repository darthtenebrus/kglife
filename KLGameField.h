//
// Created by esorochinskiy on 26.09.23.
//

#ifndef KGLIVE_KLGAMEFIELD_H
#define KGLIVE_KLGAMEFIELD_H


#include <QWidget>

class KLGameField : public QWidget {

public:
    KLGameField(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    void rePaint();

};


#endif //KGLIVE_KLGAMEFIELD_H
