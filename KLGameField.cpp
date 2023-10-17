//
// Created by esorochinskiy on 26.09.23.
//

#include <QPainter>
#include <QApplication>
#include<QDesktopWidget>
#include "KLGameField.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColorDialog>
#include "LoadGameException.h"

#ifdef _DEBUG

#include <QDebug>

#endif

#define FIELD_OFFSET 1
#define SPACE 1
#define MIN_CELL_SIZE 16
#define MAX_CELL_SIZE 128
#define DIVISOR 2000

KLGameField::KLGameField(const QColor &cellsColor, const QColor &backgroundColor,
                         int timerInterval, QWidget *parent) : QWidget(parent),
                                                               m_ColorCells(cellsColor),
                                                               m_ColorBackground(backgroundColor),
                                                               evoTimer(new QTimer()) {
    m_cellSize = MIN_CELL_SIZE;
    setMouseTracking(true);
    changeMoveMode(false);
    m_Cursor = cursor();
    m_TimerInterval = DIVISOR / timerInterval;
    initTotalCells();
    connect(evoTimer, &QTimer::timeout, this, &KLGameField::nextGeneration);
}

KLGameField::~KLGameField() {

    delete m_MainLayer;
    delete m_NextStepLayer;
    delete evoTimer;
}

void KLGameField::restoreScreen() {
    m_cellSize = MIN_CELL_SIZE;
    m_CurrMemOffsetX = m_CurrMemOffsetY = 0;
    recalcScreenCells();
    repaint();
}


void KLGameField::changeDelta(int delta) {
    int tmpSize = 0;
    tmpSize = m_cellSize + delta;

    if (tmpSize > MAX_CELL_SIZE) {
        m_cellSize = MAX_CELL_SIZE;
    } else if (tmpSize < MIN_CELL_SIZE) {
        m_cellSize = MIN_CELL_SIZE;
    } else {
        m_cellSize = tmpSize;
    }
    recalcScreenCells();
    repaint();
}

void KLGameField::wheelEvent(QWheelEvent *event) {
    cancelTimerInstantly();
    QPoint numDegrees = event->angleDelta() / 8;
    changeDelta(numDegrees.y());

}

void KLGameField::paintEvent(QPaintEvent *e) {
    actualDoRePaint();
}

void KLGameField::actualDoRePaint() {

    QPainter painter(this);

    painter.setBackground(QBrush("#232323"));

    const QPoint &mainOffset = getMainOffset();
    painter.translate(mainOffset.x(), mainOffset.y());
    for (int y = 0; y < m_ScrCellsY; ++y) {
        for (int x = 0; x < m_ScrCellsX; ++x) {
            QBrush color;

            color = (fromMainLayer(m_CurrMemOffsetX + x, m_CurrMemOffsetY + y)) ? QBrush(m_ColorCells) : QBrush(
                    m_ColorBackground);
            painter.fillRect(QRect(x * (m_cellSize + SPACE), y * (m_cellSize + SPACE),
                                   m_cellSize, m_cellSize), color);

        }

    }

    emit changeZoomIn(m_cellSize < MAX_CELL_SIZE);
    emit changeZoomOut(m_cellSize > MIN_CELL_SIZE);
    emit changeRestore(m_CurrMemOffsetX || m_CurrMemOffsetY || (m_cellSize != MIN_CELL_SIZE));

}

void KLGameField::recalcScreenCells() {

    int h = height();
    int w = width();
    QPoint fd = getStandardFieldDefs(w, h);

    m_ScrCellsX = fd.x() / (m_cellSize + SPACE);
    m_ScrCellsY = fd.y() / (m_cellSize + SPACE);

    if (m_ScrCellsX > m_cellsX) {
        m_ScrCellsX = m_cellsX;
    }
    m_MaxMemOffsetX = m_cellsX - m_ScrCellsX;

    if (m_CurrMemOffsetX > m_MaxMemOffsetX) {
        m_CurrMemOffsetX = m_MaxMemOffsetX;
    }


    if (m_ScrCellsY > m_cellsY) {
        m_ScrCellsY = m_cellsY;
    }
    m_MaxMemOffsetY = m_cellsY - m_ScrCellsY;

    if (m_CurrMemOffsetY > m_MaxMemOffsetY) {
        m_CurrMemOffsetY = m_MaxMemOffsetY;
    }


    m_remScrX = (fd.x() - (m_cellSize + SPACE) * m_ScrCellsX) / 2;
    m_remScrY = (fd.y() - (m_cellSize + SPACE) * m_ScrCellsY) / 2;

}

void KLGameField::resizeEvent(QResizeEvent *event) {
    cancelTimerInstantly();

    recalcScreenCells();
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

    int nEmpties = 0;
    for (int y = 0; y < m_cellsY; ++y) {
        for (int x = 0; x < m_cellsX; ++x) {

            nEmpties += (!fromMainLayer(x, y));

            int countNeighbors = calculateNeighbors(x, y);
            uchar targetVal;
            switch (countNeighbors) {
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
    m_Generation++;

    if (nEmpties == m_cellsX * m_cellsY) {

        cancelTimerInstantly();
        emit emptyColony();
    } else {
        emit changeGeneration(m_Generation);
    }

}

int KLGameField::calculateNeighbors(int x, int y) {
    int locNeighbors = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {

            if (!dx && !dy) {
                continue;
            }

            int newY = y + dy;
            int newX = x + dx;

            if (newY < 0) {
                newY = m_cellsY - 1;
            }

            if (newY >= m_cellsY) {
                newY = 0;
            }


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

QPoint KLGameField::getMainOffset() const {
    return {FIELD_OFFSET + SPACE + m_remScrX, FIELD_OFFSET + SPACE + m_remScrY};
}

void KLGameField::mouseDoubleClickEvent(QMouseEvent *event) {

    if (m_MoveMode) {
        return;
    }

    QPoint newPos = event->pos();
    if (!checkMousePosition(newPos)) {
        return;
    }

    cancelTimerInstantly();
    m_LeftbPressed = false;

    int cellX = newPos.x() / (m_cellSize + SPACE);
    int cellY = newPos.y() / (m_cellSize + SPACE);

    m_Generation = 0;
    copyToLayer(m_MainLayer, m_CurrMemOffsetX + cellX, m_CurrMemOffsetY + cellY,
                !fromMainLayer(m_CurrMemOffsetX + cellX, m_CurrMemOffsetY + cellY));
    repaint();
}

void KLGameField::mouseMoveEvent(QMouseEvent *event) {

    static int oldCellX = -1;
    static int oldCellY = -1;

    QPoint newPos = event->pos();
    if (!checkMousePosition(newPos)) {
        setCursor(m_Cursor);
        m_LeftbPressed = false;
        return;
    } else {

        if (m_LeftbPressed) {
            m_Generation = 0;
            cancelTimerInstantly();
            int cellX = newPos.x() / (m_cellSize + SPACE);
            int cellY = newPos.y() / (m_cellSize + SPACE);

            if (!m_MoveMode) {

                if (cellX != oldCellX || cellY != oldCellY) {
                    oldCellX = cellX;
                    oldCellY = cellY;
                    copyToLayer(m_MainLayer, m_CurrMemOffsetX + cellX, m_CurrMemOffsetY + cellY,
                                !fromMainLayer(m_CurrMemOffsetX + cellX, m_CurrMemOffsetY + cellY));
                    repaint();
                }
            } else {
                setCursor(Qt::ClosedHandCursor);
                if (cellX != oldCellX || cellY != oldCellY) {

                    if (oldCellX == -1 && oldCellY == -1) {
                        oldCellX = cellX;
                        oldCellY = cellY;
                        return;
                    }

                    int dirX = sgn(cellX - oldCellX);
                    int dirY = sgn(cellY - oldCellY);

                    oldCellX = cellX;
                    oldCellY = cellY;
                    intentToMoveField(dirX, dirY);
                }

            }
        } else {
            setCursor(m_MoveMode ? Qt::OpenHandCursor : Qt::CrossCursor);
        }
    }
}

void KLGameField::nextAction(bool) {
    recalculate();
    repaint();
}

void KLGameField::nextGeneration(void) {
    nextAction(true);
}

void KLGameField::checkTimerAndUpdate(bool) {
    if (!evoTimer->isActive()) {
        evoTimer->start(m_TimerInterval);
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

void KLGameField::timerChanged(int timerInterval) {
    m_TimerInterval = DIVISOR / timerInterval;
    if (evoTimer->isActive()) {
        evoTimer->stop();
        evoTimer->start(m_TimerInterval);
    }
}

void KLGameField::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QPoint newPos = event->pos();
        if (!checkMousePosition(newPos)) {
            return;
        }
        cancelTimerInstantly();
        m_LeftbPressed = true;
    }
    QWidget::mousePressEvent(event);
}

void KLGameField::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QPoint newPos = event->pos();
        if (!checkMousePosition(newPos)) {
            return;
        }
        cancelTimerInstantly();
        m_LeftbPressed = false;
    }

    QWidget::mouseReleaseEvent(event);
}

bool KLGameField::checkMousePosition(QPoint &mpos) const {
    mpos -= getMainOffset();
    return (mpos.x() >= 0 && mpos.y() >= 0 &&
            mpos.x() <= m_ScrCellsX * (m_cellSize + SPACE) && mpos.y() <= m_ScrCellsY * (m_cellSize + SPACE));
}

void KLGameField::newAction(bool) {
    cancelTimerInstantly();
    m_MainLayer = initLayer(m_MainLayer);
    m_NextStepLayer = initLayer(m_NextStepLayer);
    m_Generation = 0;
    repaint();
    emit changeGeneration(m_Generation);
}

void KLGameField::openAction(bool) {
    cancelTimerInstantly();
    const QString &path = QFileDialog::getOpenFileName(this, tr("Load colony from file"),
                                                       QDir::homePath(), tr("This application (*.kgol)"));

#ifdef _DEBUG
    qDebug() << "File = " << path;
#endif

    if ("" == path) {
        return;
    }

    try {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw LoadGameException(tr("Open file failed").toStdString());
        }

        const QByteArray &header = file.read(8);
        const QString &strhead = QString::fromLatin1(header);
        if (strhead != "KGOL_SIG") {
            throw LoadGameException(tr("Invalid file format").toStdString());
        }

#ifdef _DEBUG
        qDebug() << "Header Ok = " << strhead;
#endif

        QByteArray content;
        while (!file.atEnd()) {
            content = file.readAll();
        }
        file.close();
        const QString &strContent = QString::fromLatin1(content);
        const QStringList &coords = strContent.split("CX");
        if (coords.empty()) {
            throw LoadGameException(tr("Invalid file format").toStdString());
        }

        m_MainLayer = initLayer(m_MainLayer);
        m_NextStepLayer = initLayer(m_NextStepLayer);

        for (const QString &item: coords) {
            if (item.isEmpty()) {
                continue;
            } else if (!item.contains('Y')) {
                throw LoadGameException(tr("Invalid file format").toStdString());
            }

            const QStringList &pairXY = item.split('Y');
            bool ready = false;
            int resX;
            int resY = pairXY[1].toInt(&ready);
            if (ready) {
                resX = pairXY[0].toInt(&ready);
            }

            if (!ready) {
                throw LoadGameException(tr("Invalid file format").toStdString());
            }

            if (resX < 0 || resX >= m_cellsX || resY < 0 || resY >= m_cellsY) {
                continue;
            }


            copyToLayer(m_MainLayer, resX, resY, 1);
        }
    } catch (const LoadGameException &ex) {

        QMessageBox::critical(this, tr("Error"), QString::fromStdString(ex.what()));
    }
}

void KLGameField::saveAction(bool) {
    cancelTimerInstantly();
    const QString &path = QFileDialog::getSaveFileName(this, tr("Save colony current state"),
                                                       QDir::homePath(), tr("This application (*.kgol)"));
#ifdef _DEBUG
    qDebug() << "File = " << path;
#endif


    if ("" == path) {
        return;
    }


    try {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            throw LoadGameException(tr("Open file failed").toStdString());
        }
        QTextStream out(&file);
#ifdef _DEBUG
        qDebug() << "Out created";
#endif
        out << "KGOL_SIG";
        for (int y = 0; y < m_cellsY; ++y) {
            for (int x = 0; x < m_cellsX; ++x) {
                if (fromMainLayer(x, y)) {
                    out << "CX" << x << "Y" << y;
                }
            }
        }
#ifdef _DEBUG
        qDebug() << "Out written";
#endif
        file.close();
    } catch (const LoadGameException &ex) {
        QMessageBox::critical(this, tr("Error"), QString::fromStdString(ex.what()));
    }
}

void KLGameField::changeCellsColor(bool) {

    const QColor &ccolor = QColorDialog::getColor(m_ColorCells, this,
                                                  tr("Choose cells color"));

    if (ccolor.isValid()) {
        m_ColorCells = ccolor;
        repaint();
        emit changeSetting("cellsColor", m_ColorCells);
    }

}

void KLGameField::changeBackgroundColor(bool) {

    const QColor &ccolor = QColorDialog::getColor(m_ColorBackground, this,
                                                  tr("Choose background color"));

    if (ccolor.isValid()) {
        m_ColorBackground = ccolor;
        repaint();
        emit changeSetting("backColor", m_ColorBackground);
    }

}

void KLGameField::changeMoveMode(bool mMode) {
    m_MoveMode = mMode;
    setStatusTip(!m_MoveMode ?
                 tr("Set or erase a single cell by double click or drag a line with left button pressed") :
                 tr("Drag the mouse to move field"));
}

int KLGameField::sgn(int val) {
    return (0 < val) - (val < 0);
}

void KLGameField::intentToMoveField(int dx, int dy) {
    bool changed = false;
    int tmpMemOffsetX = m_CurrMemOffsetX;
    int tmpMemOffsetY = m_CurrMemOffsetY;
    tmpMemOffsetX -= dx;
    tmpMemOffsetY -= dy;
    if (tmpMemOffsetX >= 0 && tmpMemOffsetX <= m_MaxMemOffsetX) {
        m_CurrMemOffsetX = tmpMemOffsetX;
        changed = true;
    }

    if (tmpMemOffsetY >= 0 && tmpMemOffsetY <= m_MaxMemOffsetY) {
        m_CurrMemOffsetY = tmpMemOffsetY;
        changed = true;
    }

    if (changed) {
        repaint();
    }
}

void KLGameField::initTotalCells() {

    QDesktopWidget *desktopwidget = QApplication::desktop(); // Get display resolution
    int dw = desktopwidget->width();
    int dh = desktopwidget->height();
    QPoint fd = getStandardFieldDefs(dw, dh);

    m_cellsX = fd.x() / (m_cellSize + SPACE);
    m_cellsY = fd.y() / (m_cellSize + SPACE);
    m_MainLayer = initLayer(m_MainLayer);
    m_NextStepLayer = initLayer(m_NextStepLayer);
    m_Generation = 0;
    emit changeGeneration(m_Generation);

}

QPoint KLGameField::getStandardFieldDefs(int &x, int &y) const {
    return {x - (FIELD_OFFSET + SPACE) * 2, y - (FIELD_OFFSET + SPACE) * 2};
}

void KLGameField::cZoomIn(bool) {
    cancelTimerInstantly();
    changeDelta(12);
}

void KLGameField::cZoomOut(bool) {
    cancelTimerInstantly();
    changeDelta(-12);
}

void KLGameField::cRestore(bool) {
    cancelTimerInstantly();
    restoreScreen();
}







