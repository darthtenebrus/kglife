//
// Created by esorochinskiy on 26.11.23.
//

#ifndef KGLIFE_GENERALPAGE_H
#define KGLIFE_GENERALPAGE_H

#include <QWidget>
#include "ui_generalpage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GeneralPage; }
QT_END_NAMESPACE

class GeneralPage : public QWidget, public Ui::GeneralPage {
Q_OBJECT

public:
    explicit GeneralPage(QWidget *parent = nullptr);

};


#endif //KGLIFE_GENERALPAGE_H
