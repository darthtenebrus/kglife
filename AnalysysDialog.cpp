//
// Created by esorochinskiy on 07.12.2024.
//

#include "AnalysysDialog.h"

AnalysysDialog::AnalysysDialog(const QStringList &objList, QWidget *parent) : QDialog(parent), Ui_analysDialog(), objList(objList) {
    setupUi(this);
    for (QString o : objList) {
        lifeObjects->addItem(o);
    }
}

AnalysysDialog::~AnalysysDialog() = default;
