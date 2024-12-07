//
// Created by esorochinskiy on 07.12.2024.
//

#ifndef KGLIFE_ANALYSYSDIALOG_H
#define KGLIFE_ANALYSYSDIALOG_H
#include "ui_AnalysysDialog.h"
#include "GLifeObject.h"


class AnalysysDialog : public QDialog, public Ui_analysDialog {

public:
    AnalysysDialog(const QList<GLifeObject> &objList,  QWidget *parent = nullptr);

    ~AnalysysDialog() override;
private:
    QList<GLifeObject> objList;
};


#endif //KGLIFE_ANALYSYSDIALOG_H
