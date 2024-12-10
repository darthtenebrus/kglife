//
// Created by esorochinskiy on 05.12.2024.
//

#include "GLifeObject.h"

void GLifeObject::add(const QVector<int> &chorde) {
    chordes.append(chorde);

}

GLifeObject::GLifeObject(const QPoint &absPoint, const QString &lifeObjectName, QObject *parent)
        : QObject(parent), lifeObjectName(lifeObjectName), absol(absPoint) {}

GLifeObject::~GLifeObject() = default;

const QList<QVector<int>> &GLifeObject::listChordes() const {
    return chordes;
}

const QString &GLifeObject::getlifeObjectName() const {
    return lifeObjectName;
}


GLifeObject::GLifeObject(const GLifeObject &src) : QObject(nullptr) {
    absol = src.absol;
    chordes = src.chordes;
    lifeObjectName = src.lifeObjectName;
}

bool GLifeObject::hasLinkedForChorde(const QVector<int> &checked) {
    for (QVector<int> &cChorde: chordes) {
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
    lifeObjectName = src.lifeObjectName;
    return *this;
}

[[maybe_unused]] void GLifeObject::setlifeObjectName(const QString &l) {
    lifeObjectName = l;
}

const QPoint &GLifeObject::getAbsol() const {
    return absol;
}
