//
// Created by esorochinskiy on 07.12.2024.
//

#include "AnalysysDialog.h"

AnalysysDialog::AnalysysDialog(const QList<GLifeObject> &objList, QWidget *parent) : QDialog(parent), Ui_analysDialog(), objList(objList) {
    setupUi(this);
    for (GLifeObject o : objList) {
        lifeObjects->addItem(o.getlifeObjectName());
    }

}

AnalysysDialog::~AnalysysDialog() = default;
