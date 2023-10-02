//
// Created by esorochinskiy on 26.09.23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <QVBoxLayout>
#include <QSlider>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "version.h"

QString MainWindow::orgName = "kglife";

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow),
        settings(QSettings::NativeFormat, QSettings::UserScope,
                 MainWindow::orgName, "config"),
        timerSlider(new QSlider(Qt::Horizontal, this)) {
    ui->setupUi(this);
    timerSlider->setValue(5);
    timerSlider->setSliderPosition(5);
    timerSlider->setSingleStep(1);
    timerSlider->setPageStep(1);
    timerSlider->setMinimum(1);
    timerSlider->setMaximum(10);
    timerSlider->setTracking(true);
    timerSlider->setToolTip(tr("Generation Change Speed"));
    timerSlider->setFixedWidth(200);
    ui->toolBar->addWidget(timerSlider);

    fillDataFromSettings();

    gameField = new KLGameField(m_data["cellsColor"].value<QColor>(),
                                m_data["backColor"].value<QColor>(),
                                timerSlider->value(), this);
    gameField->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    gameField->setStatusTip(tr("Set or erase a single cell by double click or drag a line with left button pressed"));

    ui->centralwidget->layout()->addWidget(gameField);

    connect(ui->actionNewGame, &QAction::triggered, gameField, &KLGameField::newAction);
    connect(ui->actionOpen, &QAction::triggered, gameField, &KLGameField::openAction);
    connect(ui->actionSave, &QAction::triggered, gameField, &KLGameField::saveAction);

    connect(ui->actionNextStep, &QAction::triggered, gameField, &KLGameField::nextAction);
    connect(ui->actionStartStop, &QAction::triggered, gameField, &KLGameField::checkTimerAndUpdate);
    connect(ui->actionAboutQt, &QAction::triggered, this, [=]{
        QMessageBox::aboutQt(this);

    });

    connect(ui->actionAbout, &QAction::triggered, this, [=]{
        QMessageBox::about(this, "KGLife",
                           QString("KGLife v. %1\n").arg(APP_VERSION) +
        tr("A simple Game Of Life Qt realization") +
        "\n© 2023 E.Sorochinskiy");

    });

    connect(ui->actionCellsColor, &QAction::triggered, gameField,
            &KLGameField::changeCellsColor);
    connect(ui->actionBackgroundColor, &QAction::triggered, gameField,
            &KLGameField::changeBackgroundColor);
    connect(timerSlider, &QSlider::valueChanged, gameField, &KLGameField::timerChanged);

    connect(gameField, &KLGameField::changeControls, this, &MainWindow::controlsChanged);
    connect(gameField, &KLGameField::changeGeneration, this, &MainWindow::generationChanged);
    connect(gameField, &KLGameField::emptyColony, this, &MainWindow::colonyIsEmpty);

    connect(gameField, &KLGameField::changeSetting, this, &MainWindow::settingChanged);

}

MainWindow::~MainWindow() {
    delete gameField;
    delete ui;
}

void MainWindow::controlsChanged(bool enabled) {

    ui->actionNextStep->setEnabled(enabled);
    ui->actionStartStop->setIcon(QIcon::fromTheme(enabled ? "media-playback-start-symbolic" : "media-playback-pause"));
    ui->actionStartStop->setToolTip(enabled ? tr("Start evolution") : tr("Stop evolution"));
}

void MainWindow::generationChanged(int cgen) {
    ui->statusbar->showMessage(tr("Generation: %1").arg(cgen));

}

void MainWindow::colonyIsEmpty(void) {
    ui->statusbar->showMessage(tr("Colony is empty"));
}

void MainWindow::fillDataFromSettings(void)  {

    for (const QString &key: defs.keys()) {
        m_data.insert(key, settings.value(key, defs[key]));
    }

}


void MainWindow::closeEvent(QCloseEvent *event) {
    gameField->cancelTimerInstantly();
    writeSettingsDirect();
    QWidget::closeEvent(event);
}

void MainWindow::writeSettingsDirect(void) {
    for (const QString &key: m_data.keys()) {
        settings.setValue(key, m_data.value(key));
    }
    settings.sync();

}

void MainWindow::settingChanged(const QString &setting, const QColor &color) {
    m_data[setting] = QVariant(color);
}

