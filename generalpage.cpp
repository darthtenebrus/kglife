//
// Created by esorochinskiy on 26.11.23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GeneralPage.h" resolved

#include "generalpage.h"



GeneralPage::GeneralPage(QWidget *parent) :
        QWidget(parent), Ui::GeneralPage() {
    setupUi(this);
}

