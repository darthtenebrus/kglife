//
// Created by esorochinskiy on 05.12.2024.
//

#include "GLifeObject.h"

void GLifeObject::add(const QVector<int>& chorde) {
    chordes.append(chorde);

}

GLifeObject::GLifeObject(QObject *parent) : QObject(parent) {}

GLifeObject::~GLifeObject() = default;

const QList<QVector<int>> &GLifeObject::listChordes() const {
    return chordes;
}

void GLifeObject::setAbsol(const QPoint &absPoint) {
    absol = absPoint;
}

GLifeObject::GLifeObject(const GLifeObject &src) : QObject(nullptr) {
    absol = src.absol;
    chordes = src.chordes;
}

bool GLifeObject::hasLinkedForChorde(const QVector<int> &checked) {
    for (QVector<int> &cChorde : chordes) {
        bool isLinked = ((abs(cChorde[1] - checked[1]) == 1) &&
                ((checked[0] >= cChorde[0] && checked[0] <= cChorde[0] + cChorde[2]) ||
                (cChorde[0] >= checked[0] && cChorde[0] <= checked[0] + checked[2])));
        if (isLinked) {
            return isLinked;
        }
    }
    return false;
}

GLifeObject &GLifeObject::operator=(const GLifeObject &src) noexcept {
    absol = src.absol;
    chordes = src.chordes;
    return *this;
}
