//
// Created by esorochinskiy on 12.12.23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GeneratorPage.h" resolved

#include "generatorpage.h"
#include "ui_generatorpage.h"
#include "kglife.h"


GeneratorPage::GeneratorPage(int expectValX, int expectValY, QWidget *parent) :
        QWidget(parent), Ui::GeneratorPage() {
    setupUi(this);
    kcfg_distributionforX->addItem(i18n("Uniform"));
    kcfg_distributionforX->addItem(i18n("Binomial"));
    kcfg_distributionforX->addItem(i18n("Poisson"));
    kcfg_probtrialforX->setDisabled(true);
    kcfg_expectedvalforX->setDisabled(true);

    connect(kcfg_distributionforX, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GeneratorPage::indexIsChanged);
    kcfg_expectedvalforX->setMaximum(expectValX);


    for (int i = 0; i < kcfg_distributionforX->count(); ++i) {
        kcfg_distributionforY->addItem(kcfg_distributionforX->itemText(i));
    }

    kcfg_probtrialforY->setDisabled(true);
    kcfg_expectedvalforY->setDisabled(true);

    connect(kcfg_distributionforY, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GeneratorPage::indexIsChanged);
    kcfg_expectedvalforY->setMaximum(expectValY);
}

void GeneratorPage::indexIsChanged(int index) {
    auto distType = static_cast<Settings::DistribType>(index);
    QObject *objSender = sender();
    QDoubleSpinBox *prtrial;
    QSpinBox *expected;
    if (objSender->objectName() == "kcfg_distributionforX") {
        prtrial = kcfg_probtrialforX;
        expected = kcfg_expectedvalforX;
    } else if(objSender->objectName() == "kcfg_distributionforY") {
        prtrial = kcfg_probtrialforY;
        expected = kcfg_expectedvalforY;
    } else {
        return;
    }
    prtrial->setEnabled(distType == Settings::BINOMIAL);
    expected->setEnabled(distType == Settings::POISSON);
}

