//
// Created by esorochinskiy on 26.09.23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <QVBoxLayout>
#include <QSlider>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    timerSlider = new QSlider(Qt::Horizontal, this);
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

    gameField = new KLGameField(timerSlider->value(), this);
    gameField->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    gameField->setStatusTip(tr("Set or erase a single cell by double click or drag a line with left button pressed"));

    ui->centralwidget->layout()->addWidget(gameField);

    connect(ui->actionNewGame, &QAction::triggered, gameField, &KLGameField::newAction);
    connect(ui->actionNextStep, &QAction::triggered, gameField, &KLGameField::nextAction);
    connect(ui->actionStartStop, &QAction::triggered, gameField, &KLGameField::checkTimerAndUpdate);
    connect(ui->actionAboutQt, &QAction::triggered, this, [=]{
        QMessageBox::aboutQt(this);

    });

    connect(ui->actionAbout, &QAction::triggered, this, [=]{
        QMessageBox::about(this, "KGLife", tr("A simple Game Of Life Qt realization"));

    });

    connect(timerSlider, &QSlider::valueChanged, gameField, &KLGameField::timerChanged);

    connect(gameField, &KLGameField::changeControls, this, &MainWindow::controlsChanged);
    connect(gameField, &KLGameField::changeGeneration, this, &MainWindow::generationChanged);
    connect(gameField, &KLGameField::emptyColony, this, &MainWindow::colonyIsEmpty);

}

MainWindow::~MainWindow() {
    delete gameField;
    delete ui;
}

void MainWindow::controlsChanged(bool enabled) {

    ui->actionNextStep->setEnabled(enabled);
    ui->actionStartStop->setIcon(QIcon::fromTheme(enabled ? "media-playback-start-symbolic" : "media-playback-stop"));
    ui->actionStartStop->setToolTip(enabled ? tr("Start evolution") : tr("Stop evolution"));
}

void MainWindow::generationChanged(int cgen) {
    ui->statusbar->showMessage(tr("Generation: %1").arg(cgen));

}

void MainWindow::colonyIsEmpty(void) {
    ui->statusbar->showMessage(tr("Colony is empty"));
}

