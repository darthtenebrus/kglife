//
// Created by esorochinskiy on 26.11.23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PatternsPage.h" resolved

#include <QDir>
#include <QStandardPaths>
#include <KConfig>
#include <KConfigGroup>
#include "patternspage.h"
#include <QPushButton>
#include <KConfigDialog>


PatternsPage::PatternsPage(KConfigSkeleton *pSkeleton, KConfigDialog *cdialog, QWidget *parent) :
        QWidget(parent),  Ui::PatternsPage() {
    setupUi(this);
    m_Scene = new QGraphicsScene(layoutPreview);
    layoutPreview->setScene(m_Scene);
    connect(patternsList, &QListWidget::currentRowChanged, this, &PatternsPage::patternChanged);
    connect(cdialog->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this,
            &PatternsPage::restoreSettingsClicked);
    fillPatternList();
    setupData(pSkeleton);

}

PatternsPage::~PatternsPage() {
    delete m_Scene;
}


void PatternsPage::fillPatternList() {

    patternsList->clear();

    auto *firstItem = new QListWidgetItem();
    firstItem->setData(Qt::DisplayRole, i18n("<Not selected>"));
    patternsList->addItem(firstItem);

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

            const QString &imageFilePath = finfo.absolutePath() +
                                          QDir::separator() + group.readEntry("ImageName");
            const QString &displayTitle = group.readEntry("Name");

            auto *item = new QListWidgetItem();
            item->setData(Qt::DisplayRole, displayTitle);
            item->setData(Qt::UserRole, kgolFilePath);
            item->setData(Qt::UserRole + 100, group.readEntry("Description"));
            item->setData(Qt::UserRole + 101, group.readEntry("Author"));
            item->setData(Qt::UserRole + 102, group.readEntry("AuthorEmail"));
            item->setData(Qt::UserRole + 103, imageFilePath);

            item->setSizeHint(QSize(item->sizeHint().width(), 72));
            patternsList->addItem(item);

        }
    }
}

void PatternsPage::patternChanged(int cRow) {
    QListWidgetItem *item = patternsList->item(cRow);

    if (!cRow) {
        patternGroupBox->hide();
        layoutPreview->hide();
    } else {
        patternGroupBox->show();
        layoutPreview->show();
        descData->setText(item->data(Qt::UserRole + 100).value<QString>());
        authorData->setText(item->data(Qt::UserRole + 101).value<QString>());
        emailData->setText(item->data(Qt::UserRole + 102).value<QString>());
        const QString &imageFileData = item->data(Qt::UserRole + 103).value<QString>();

        if (QFile::exists(imageFileData)) {

            const QPixmap &pix = QPixmap(imageFileData);
            const QSize &siz = {pix.width() + 10, pix.height() + 10};
            layoutPreview->setMinimumSize(siz);
            layoutPreview->resize(siz);

            QGraphicsScene *scene = layoutPreview->scene();
            scene->clear();
            scene->addPixmap(pix);

        }

    }
    kcfg_templatefile->setText(item->data(Qt::UserRole).value<QString>());
}

void PatternsPage::setupData(KConfigSkeleton *pSkeleton) {

    kcfg_templatefile->hide();
    if (patternsList->count() > 1) {

        KConfig * config = pSkeleton->config();
        KConfigGroup group = config->group(QStringLiteral("General"));
        QString initialGroup = group.readEntry("templatefile");

        if (!initialGroup.isEmpty()) {
            for (int i = 0; i < patternsList->count(); i++) {
                QListWidgetItem *item = patternsList->item(i);
                const QString &cPath = item->data(Qt::UserRole).value<QString>();
                if (cPath == initialGroup) {
                    patternsList->setCurrentRow(i);
                    return;
                }
            }
        }

    }
    cleanup();
}

void PatternsPage::restoreSettingsClicked(bool) {
    cleanup();
}

void PatternsPage::cleanup() {
    patternsList->setCurrentRow(0);
}
