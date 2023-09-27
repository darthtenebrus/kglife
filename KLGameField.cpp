//
// Created by esorochinskiy on 26.09.23.
//

#include <QPainter>
#include "KLGameField.h"
#define FIELD_OFFSET 1
#define SPACE 1
#define CELL_SIZE 12

KLGameField::KLGameField(QWidget *parent) : QWidget(parent) {

}

void KLGameField::paintEvent(QPaintEvent *e) {
    rePaint();
}

void KLGameField::rePaint() {

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    int h = height();
    int w = width();
    int fw = w - FIELD_OFFSET * 2;
    int fh = h - FIELD_OFFSET * 2;

    painter.fillRect(QRect(FIELD_OFFSET,FIELD_OFFSET, fw,fh), QBrush("#232323"));

    fw -= SPACE * 2;
    fh -= SPACE * 2;

    int qx = fw / (CELL_SIZE + SPACE);
    int remX = (fw % (CELL_SIZE + SPACE)) / 2;
    int qy = fh / (CELL_SIZE + SPACE);
    int remY = (fh % (CELL_SIZE + SPACE)) / 2;

    painter.translate(FIELD_OFFSET + SPACE + remX,FIELD_OFFSET + SPACE + remY);
    for (int i = 0; i < qx; ++i) {
        for (int j = 0; j < qy; ++j) {
            painter.fillRect(QRect(i * (CELL_SIZE + SPACE), j * (CELL_SIZE + SPACE),
                                   CELL_SIZE, CELL_SIZE), QBrush("#000000"));

        }

    }

}
