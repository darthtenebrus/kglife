//
// Created by esorochinskiy on 26.09.23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <QVBoxLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    gameField = new KLGameField(this);

    ui->centralwidget->layout()->addWidget(gameField);

    connect(ui->actionNextStep, &QAction::triggered, gameField, &KLGameField::nextAction);
    connect(ui->nextStepButton, &QToolButton::clicked, gameField, &KLGameField::nextAction);
    connect(ui->playButton, &QToolButton::clicked, gameField, &KLGameField::checkTimerAndUpdate);
    connect(gameField, &KLGameField::changeControls, this, &MainWindow::controlsChanged);

}

MainWindow::~MainWindow() {
    delete gameField;
    delete ui;
}

void MainWindow::controlsChanged(bool enabled) {

    ui->actionNextStep->setEnabled(enabled);
    ui->nextStepButton->setEnabled(enabled);
    ui->playButton->setIcon(QIcon::fromTheme(enabled ? "media-playback-start-symbolic" : "media-playback-stop"));
    ui->playButton->setToolTip(enabled ? tr("Stop evolution") : tr("Start evolution"));
}

