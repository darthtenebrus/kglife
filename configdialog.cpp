//
// Created by esorochinskiy on 10.11.23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ConfigDialog.h" resolved

#include "configdialog.h"
#include "ui_configdialog.h"
#include "DialogItemDelegate.h"
#include "KLGameField.h"
#include <QPushButton>
#include <QColorDialog>
#include <QIcon>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <KConfig>
#include <KConfigGroup>


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
    connect(ui->buttonBox->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked,
            this, &ConfigDialog::restoreDefaults);
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked,
            dynamic_cast<KLGameField *>(parent), &KLGameField::cdApply);
    connect(dynamic_cast<KLGameField *>(parent), &KLGameField::settingsApplied, this, [=]() {
        toggleButtons(false);
    });
    connect(ui->listWidget->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &ConfigDialog::currentChanged);

    connect(ui->patternList->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &ConfigDialog::patternSelected);

    connect(ui->buttonBackColor, &QPushButton::clicked, this, [=]() {
        const QColor &ccolor = QColorDialog::getColor(mBackColor, this,
                                                      tr("Choose background color"));

        if (ccolor.isValid()) {
            mBackColor = ccolor;
            setButtonIconColor(ui->buttonBackColor, mBackColor);
            toggleButtons(true);
        }
    });

    connect(ui->buttonCellColor, &QPushButton::clicked, this, [=]() {
        const QColor &ccolor = QColorDialog::getColor(mCellColor, this,
                                                      tr("Choose cells color"));

        if (ccolor.isValid()) {
            mCellColor = ccolor;
            setButtonIconColor(ui->buttonCellColor, mCellColor);
            toggleButtons(true);
        }
    });

    connect(ui->buttonBetweenColor, &QPushButton::clicked, this, [=]() {
        const QColor &ccolor = QColorDialog::getColor(mBetweenColor, this,
                                                      tr("Choose border color"));

        if (ccolor.isValid()) {
            mBetweenColor = ccolor;
            setButtonIconColor(ui->buttonBetweenColor, mBetweenColor);
            toggleButtons(true);
        }
    });

    toggleButtons(false);

}

ConfigDialog::~ConfigDialog() {
    delete ui;
}

void ConfigDialog::currentChanged(const QModelIndex &current, const QModelIndex &prev) {
    int cr = current.row();
    if (cr == 1) {
        fillPatternList(ui->patternList);
    }
    ui->stackedWidget->setCurrentIndex(cr);
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

void ConfigDialog::restoreDefaults(bool) {
    mBackColor = QColor("#000000");
    mBetweenColor = QColor("#232323");
    mCellColor = QColor("#00ff00");
    setButtonIconColor(ui->buttonBackColor, mBackColor);
    setButtonIconColor(ui->buttonCellColor, mCellColor);
    setButtonIconColor(ui->buttonBetweenColor, mBetweenColor);
    templatePath.clear();
    toggleButtons(true);

}

void ConfigDialog::fillPatternList(QListWidget *list) {

    if (list->count()) {
        return;
    }
    const QStringList &fnlist = QStandardPaths::locateAll(QStandardPaths::DataLocation, "templates",
                                                          QStandardPaths::LocateDirectory);
    for(const QString &fn : fnlist) {
        const QDir &fndir = QDir(fn);
        if (!fndir.exists() || fndir.isEmpty()) {
            return;
        }

        for (QFileInfo &finfo: fndir.entryInfoList(QDir::nameFiltersFromString("*.desktop"))) {
            const QString &fabsPath = finfo.absoluteFilePath();

            KConfig kdfile(fabsPath, KConfig::SimpleConfig);

            KConfigGroup group = kdfile.group(QStringLiteral("GOLColony"));
            const QString &kgolFilePath = finfo.absolutePath() +
                                          QDir::separator() + group.readEntry("FileName");
            const QString &displayTitle = group.readEntry("Name");

            auto *item = new QListWidgetItem();
            item->setData(Qt::DisplayRole, displayTitle);
            item->setData(Qt::UserRole, kgolFilePath);
            item->setData(Qt::UserRole + 100, group.readEntry("Description"));
            item->setData(Qt::UserRole + 101, group.readEntry("Author"));
            item->setData(Qt::UserRole + 102, group.readEntry("AuthorEmail"));
            item->setSizeHint(QSize(item->sizeHint().width(), 72));
            list->addItem(item);

        }
    }
    ui->descData->clear();
    ui->authorData->clear();
    ui->emailData->clear();
}

const QString &ConfigDialog::getTemplatePath() const {
    return templatePath;
}

void ConfigDialog::patternSelected(const QModelIndex &current, const QModelIndex &prev) {
    QListWidgetItem *item = ui->patternList->item(current.row());
    ui->descData->setText(item->data(Qt::UserRole + 100).value<QString>());
    ui->authorData->setText(item->data(Qt::UserRole + 101).value<QString>());
    ui->emailData->setText(item->data(Qt::UserRole + 102).value<QString>());
    templatePath = item->data(Qt::UserRole).value<QString>();
    toggleButtons(true);
}

void ConfigDialog::toggleButtons(bool enable) {
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(enable);
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(enable);
}

