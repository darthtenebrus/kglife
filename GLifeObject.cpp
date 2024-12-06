//
// Created by esorochinskiy on 05.12.2024.
//

#include "GLifeObject.h"

void GLifeObject::add(const std::vector<int>& chorde) {
    chordes.append(chorde);

}

GLifeObject::GLifeObject(QObject *parent) : QObject(parent) {}

GLifeObject::~GLifeObject() = default;

const QList<std::vector<int>> &GLifeObject::listChordes() const {
    return chordes;
}

void GLifeObject::setAbsol(const QPoint &absPoint) {
    absol = absPoint;
}
