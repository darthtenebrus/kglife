//
// Created by esorochinskiy on 10.11.23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ConfigDialog.h" resolved

#include "configdialog.h"
#include "ui_configdialog.h"
#include "DialogItemDelegate.h"
#include <QPushButton>
#include <QColorDialog>
#include <QIcon>
#ifdef _DEBUG
#include <QDebug>
#endif


ConfigDialog::ConfigDialog(QColor &bColor, QColor &cellColor,
                           QColor &betweenColor,
                           QWidget *parent) :
        QDialog(parent), ui(new Ui::ConfigDialog) {
    ui->setupUi(this);
    ui->listWidget->setItemDelegate(new DialogItemDelegate(ui->listWidget));
    mBackColor = bColor;
    mCellColor = cellColor;
    mBetweenColor = betweenColor;
    setButtonIconColor(ui->buttonCellColor, mCellColor);
    setButtonIconColor(ui->buttonBackColor, mBackColor);
    setButtonIconColor(ui->buttonBetweenColor, mBetweenColor);

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked,
            this, &QDialog::accept);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked,
            this, &QDialog::reject);
    connect(ui->listWidget->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &ConfigDialog::currentChanged);

    connect(ui->buttonBackColor, &QPushButton::clicked, this, [=]() {
        const QColor &ccolor = QColorDialog::getColor(mBackColor, this,
                                                      tr("Choose background color"));

        if (ccolor.isValid()) {
            mBackColor = ccolor;
            setButtonIconColor(ui->buttonBackColor, mBackColor);
        }
    });

    connect(ui->buttonCellColor, &QPushButton::clicked, this, [=]() {
        const QColor &ccolor = QColorDialog::getColor(mCellColor, this,
                                                      tr("Choose cells color"));

        if (ccolor.isValid()) {
            mCellColor = ccolor;
            setButtonIconColor(ui->buttonCellColor, mCellColor);
        }
    });

    connect(ui->buttonBetweenColor, &QPushButton::clicked, this, [=]() {
        const QColor &ccolor = QColorDialog::getColor(mBetweenColor, this,
                                                      tr("Choose border color"));

        if (ccolor.isValid()) {
            mBetweenColor = ccolor;
            setButtonIconColor(ui->buttonBetweenColor, mBetweenColor);
        }
    });



}

ConfigDialog::~ConfigDialog() {
    delete ui;
}

void ConfigDialog::currentChanged(const QModelIndex &current, const QModelIndex &prev) {
    ui->stackedWidget->setCurrentIndex(current.row());
}

const QColor &ConfigDialog::getMCellColor() const {
    return mCellColor;
}

const QColor &ConfigDialog::getMBackColor() const {
    return mBackColor;
}

const QColor &ConfigDialog::getMBetweenColor() const {
    return mBetweenColor;
}

void ConfigDialog::setButtonIconColor(QPushButton *button, QColor &color) {

    const QSize &iSize = button->iconSize();
    QPixmap pixmap(iSize.width(),iSize.height());
    pixmap.fill(color);
    button->setIcon(QIcon(pixmap));
}

