//
// Created by esorochinskiy on 12.12.23.
//

#include <random>
#include "cellsgenerator.h"
#include "kglife.h"

void CellsGenerator::run() {

    if (mLimit.width() > 0 && mLimit.height() > 0 && mQuantity > 0) {
        mutex.lock();
        for (int i = 1; i < mQuantity; i++) {
            int locX = localRand(mLimit.width());
            int locY = localRand(mLimit.height());
            emit resultReady(locX, locY);
        }
        mutex.unlock();
        emit resultFinished();
    }
}

CellsGenerator::CellsGenerator(const QSize &maxLimit, int quantity, QObject *parent) : QThread(parent) {
    mLimit = maxLimit;
    mQuantity = quantity;
}

int CellsGenerator::localRand(int max) {

    auto distType = static_cast<Settings::DistribType>(Settings::distribution());

    int ret = 0;
    std::random_device rd;
    std::mt19937_64 mt(rd());
    double trialprop = Settings::probtrial();
    int settExpected = Settings::expectedval();
    int expectval = (settExpected <= max ? settExpected : max);

    std::binomial_distribution dist_binom(max, trialprop);
    std::poisson_distribution dist_poi(expectval);
    std::uniform_int_distribution<> dist_uni(0, max);

    switch (distType) {
        case Settings::BINOMIAL:
            ret = abs(dist_binom(mt));
            break;

        case Settings::POISSON:
            ret = abs(dist_poi(mt));
            break;
        case Settings::UNIFORM:
        default:
            ret = abs(dist_uni(mt));

    }

    return ret;
}

void CellsGenerator::init(const QSize &ml, int mq) {
    mutex.lock();
    mLimit = ml;
    mQuantity = mq;
    mutex.unlock();
}
