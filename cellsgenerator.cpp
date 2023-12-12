//
// Created by esorochinskiy on 12.12.23.
//

#include <random>
#include "cellsgenerator.h"


void CellsGenerator::run() {

    if (mLimit.width() > 0 && mLimit.height() > 0 && mQuantity > 0) {
        mutex.lock();
        for (int i = 1; i < mQuantity; i++) {
            int locX = localRand(mLimit.width());
            int locY = localRand(mLimit.height());
            emit resultReady(locX, locY);
        }
        mutex.unlock();
    }
}

CellsGenerator::CellsGenerator(const QSize &maxLimit, int quantity, QObject *parent) : QThread(parent) {
    mLimit = maxLimit;
    mQuantity = quantity;
}

int CellsGenerator::localRand(int max) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, max);
    int ret = abs(dist(mt));

    return ret;
}

void CellsGenerator::setLimit(const QSize &ml) {
    mutex.lock();
    mLimit = ml;
    mutex.unlock();
}

void CellsGenerator::setQuantity(int mq) {
    mutex.lock();
    mQuantity = mq;
    mutex.unlock();
}
