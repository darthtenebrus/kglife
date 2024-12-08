//
// Created by esorochinskiy on 07.12.2024.
//

#ifndef KGLIFE_ANALYSYSDIALOG_H
#define KGLIFE_ANALYSYSDIALOG_H
#include "ui_AnalysysDialog.h"
#include "GLifeObject.h"


class AnalysysDialog : public QDialog, public Ui_analysDialog {

public:
    AnalysysDialog(const QStringList &objList,  QWidget *parent = nullptr);

    ~AnalysysDialog() override;
private:
    QStringList objList;
};


#endif //KGLIFE_ANALYSYSDIALOG_H
