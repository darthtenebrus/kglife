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
    auto *vbox = new QVBoxLayout(ui->centralwidget);
    gameField = new KLGameField(ui->centralwidget);
    vbox->addWidget(gameField);
    ui->centralwidget->setLayout(vbox);
}

MainWindow::~MainWindow() {
    delete gameField;
    delete ui;
}
