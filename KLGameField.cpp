//
// Created by esorochinskiy on 26.09.23.
//

#include <QPainter>
#include "KLGameField.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include <QTimer>


#define FIELD_OFFSET 1
#define SPACE 1
#define CELL_SIZE 16

KLGameField::KLGameField(QWidget *parent) : QWidget(parent) {
    evoTimer = new QTimer();
    connect(evoTimer, &QTimer::timeout, this, &KLGameField::nextGeneration);
}

KLGameField::~KLGameField() {

    delete m_MainLayer;
    delete m_NextStepLayer;
    delete evoTimer;
}

void KLGameField::paintEvent(QPaintEvent *e) {
    actualDoRePaint();
}

void KLGameField::actualDoRePaint() {

    QPainter painter(this);

    painter.fillRect(QRect(FIELD_OFFSET, FIELD_OFFSET, m_fieldWidth, m_fieldHeight),
                     QBrush("#232323"));
    const QPoint &mainOffset = getMainOffset();
    painter.translate(mainOffset.x(), mainOffset.y());
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
    cancelTimerInstantly();

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
            copyToLayer(m_NextStepLayer, x, y, targetVal);

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

void KLGameField::copyToLayer(uchar *layer, int x, int y, uchar val) {
    layer[y * m_cellsX + x] = val;
}

QPoint KLGameField::getMainOffset() {
    return QPoint(FIELD_OFFSET + SPACE + m_remX, FIELD_OFFSET + SPACE + m_remY);
}

void KLGameField::mouseDoubleClickEvent(QMouseEvent *event) {
    cancelTimerInstantly();
    const QPoint &newPos = event->pos() - getMainOffset();
    if(newPos.x() < 0 || newPos.y() < 0) {
        return;
    }

    int cellX = newPos.x() / (CELL_SIZE + SPACE);
    int cellY = newPos.y() / (CELL_SIZE + SPACE);

    copyToLayer(m_MainLayer, cellX, cellY, !fromMainLayer(cellX, cellY));
    repaint();
}

void KLGameField::nextAction(bool) {
    recalculate();
    repaint();
}

void KLGameField::nextGeneration(void) {
    nextAction(true);
}

void KLGameField::checkTimerAndUpdate(bool) {
    if(!evoTimer->isActive()) {
        evoTimer->start(1000);
        emit changeControls(false);
    } else {
        evoTimer->stop();
        emit changeControls(true);
    }
}

void KLGameField::cancelTimerInstantly() {
    if (evoTimer->isActive()) {
        evoTimer->stop();
        emit changeControls(true);
    }

}

