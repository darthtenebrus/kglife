//
// Created by esorochinskiy on 23.11.23.
//

#include <KLocalizedString>
#include "myslider.h"

MySlider::MySlider(int value, Qt::Orientation orientation, QWidget *parent) :
            QSlider(orientation, parent) {

    setValue(value);
    setSliderPosition(value);
    setSingleStep(1);
    setPageStep(1);
    setMinimum(1);
    setMaximum(10);
    setTracking(true);

    setFixedWidth(200);

}

void MySlider::retranslateUi() {
    setToolTip(i18n("Generation Change Rate"));
    setWhatsThis(i18n("You can change generation rate even when evolution is in progress"));

}
