//
// Created by esorochinskiy on 26.09.23.
//

#include <QPainter>
#include "KLGameField.h"

#define FIELD_OFFSET 1
#define SPACE 1
#define CELL_SIZE 16

KLGameField::KLGameField(QWidget *parent) : QWidget(parent) {

}

void KLGameField::paintEvent(QPaintEvent *e) {
    actualDoRePaint();
}

void KLGameField::actualDoRePaint() {

    QPainter painter(this);

    painter.fillRect(QRect(FIELD_OFFSET, FIELD_OFFSET, m_fieldWidth, m_fieldHeight),
                     QBrush("#232323"));
    painter.translate(FIELD_OFFSET + SPACE + m_remX, FIELD_OFFSET + SPACE + m_remY);
    for (int y = 0; y < m_cellsY; ++y) {
        for (int x = 0; x < m_cellsX; ++x) {
            QBrush color;
            color = (fromMainLayer(x, y)) ? QBrush("#00FF00") : QBrush("#000000");
            painter.fillRect(QRect(x * (CELL_SIZE + SPACE), y * (CELL_SIZE + SPACE),
                                   CELL_SIZE, CELL_SIZE), color);

        }

    }

}

void KLGameField::resizeEvent(QResizeEvent *event) {

    int h = height();
    int w = width();
    m_fieldWidth = w - FIELD_OFFSET * 2;
    m_fieldHeight = h - FIELD_OFFSET * 2;

    int fw = m_fieldWidth - SPACE * 2;
    int fh = m_fieldHeight - SPACE * 2;

    m_cellsX = fw / (CELL_SIZE + SPACE);
    m_remX = (fw % (CELL_SIZE + SPACE)) / 2;
    m_cellsY = fh / (CELL_SIZE + SPACE);
    m_remY = (fh % (CELL_SIZE + SPACE)) / 2;

    m_MainLayer = initLayer(m_MainLayer);
    m_NextStepLayer = initLayer(m_NextStepLayer);
    setInitialCells();

    QWidget::resizeEvent(event);
}

uchar *KLGameField::initLayer(uchar *layer) {
    if (nullptr != layer) {
        delete layer;
    }

    layer = new uchar[m_cellsX * m_cellsY];
    memset(layer, 0, m_cellsX * m_cellsY);
    return layer;

}

KLGameField::~KLGameField() {

    delete m_MainLayer;
    delete m_NextStepLayer;
}

void KLGameField::setInitialCells(void) {


    m_MainLayer[m_cellsX * (m_cellsY / 2) + m_cellsX / 4 + 1] = 1;
    m_MainLayer[m_cellsX * (m_cellsY / 2) + m_cellsX / 4 + 2] = 1;
    m_MainLayer[m_cellsX * (m_cellsY / 2) + m_cellsX / 4 + 3] = 1;


}

void KLGameField::swapLayers(void) {

    uchar *tmp;
    tmp = m_MainLayer;
    m_MainLayer = m_NextStepLayer;
    m_NextStepLayer = tmp;
}

void KLGameField::recalculate(void) {

    for (int y = 0; y < m_cellsY; ++y) {
        for (int x = 0; x < m_cellsX; ++x) {

            int countNeighbors = calculateNeighbors(x, y);
            uchar targetVal;
            switch(countNeighbors) {
                case 3:
                    targetVal = 1;
                    break;
                case 2:
                    targetVal = fromMainLayer(x, y);
                    break;
                default:
                    targetVal = 0;
            }
            copyToNextStep(x, y, targetVal);

        }
    }
    swapLayers();

}

int KLGameField::calculateNeighbors(int x, int y) {
    int locNeighbors = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {

            if (!dx && !dy) {
                continue;
            }

            int newY = y + dy;
            if (newY < 0) {
                newY = m_cellsY - 1;
            }

            if (newY >= m_cellsY) {
                newY = 0;
            }

            int newX = x + dx;

            if (newX < 0) {
                newX = m_cellsX - 1;
            }

            if (newX >= m_cellsX) {
                newX = 0;
            }

            locNeighbors += fromMainLayer(newX, newY);
        }
    }
    return locNeighbors;
}

uchar KLGameField::fromMainLayer(int x, int y) {
    return m_MainLayer[y * m_cellsX + x];
}

void KLGameField::copyToNextStep(int x, int y, uchar val) {
    m_NextStepLayer[y * m_cellsX + x] = val;
}
