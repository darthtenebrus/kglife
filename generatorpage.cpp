//
// Created by esorochinskiy on 12.12.23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GeneratorPage.h" resolved

#include "generatorpage.h"
#include "ui_generatorpage.h"
#include "kglife.h"



GeneratorPage::GeneratorPage(int expectVal, QWidget *parent) :
        QWidget(parent), Ui::GeneratorPage() {
    setupUi(this);
    kcfg_distribution->addItem(i18n("Uniform"));
    kcfg_distribution->addItem(i18n("Binomial"));
    kcfg_distribution->addItem(i18n("Poisson"));
    kcfg_probtrial->setDisabled(true);
    kcfg_expectedval->setDisabled(true);

   connect(kcfg_distribution, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
       auto distType = static_cast<Settings::DistribType>(index);
       kcfg_probtrial->setEnabled(distType == Settings::BINOMIAL);
       kcfg_expectedval->setEnabled(distType == Settings::POISSON);
   });

   kcfg_expectedval->setMaximum(expectVal);
}

