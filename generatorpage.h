//
// Created by esorochinskiy on 12.12.23.
//

#ifndef KGLIFE_GENERATORPAGE_H
#define KGLIFE_GENERATORPAGE_H

#include <QWidget>
#include "ui_generatorpage.h"


QT_BEGIN_NAMESPACE
namespace Ui { class GeneratorPage; }
QT_END_NAMESPACE

class GeneratorPage : public QWidget, public Ui::GeneratorPage {
Q_OBJECT

public:
    explicit GeneratorPage(int expectVal, QWidget *parent = nullptr);


};


#endif //KGLIFE_GENERATORPAGE_H
