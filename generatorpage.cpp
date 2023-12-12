//
// Created by esorochinskiy on 12.12.23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GeneratorPage.h" resolved

#include "generatorpage.h"
#include "ui_generatorpage.h"


GeneratorPage::GeneratorPage(QWidget *parent) :
        QWidget(parent), ui(new Ui::GeneratorPage) {
    ui->setupUi(this);
}

GeneratorPage::~GeneratorPage() {
    delete ui;
}
