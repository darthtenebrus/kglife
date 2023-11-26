//
// Created by esorochinskiy on 26.09.23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <KLocalizedString>
#include <KStandardAction>
#include <KActionCollection>
#include <KToolBar>
#include <QDomElement>
#include <QDesktopWidget>
#include <QApplication>
#include <QStatusBar>
#include "mainwindow.h"
#include "myslider.h"



MainWindow::MainWindow(QWidget *parent) :
        KXmlGuiWindow(parent) {

    mySlider = new MySlider(Qt::Horizontal, toolBar());
    mySlider->retranslateUi();

    gameField = new KLGameField(mySlider->value(), this);
    connect(mySlider, &QSlider::valueChanged, gameField, &KLGameField::timerChanged);

    gameField->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

    setCentralWidget(gameField);
    setupToolbar();

    connect(gameField, &KLGameField::changeControls, this, &MainWindow::controlsChanged);
    connect(gameField, &KLGameField::changeGeneration, this, &MainWindow::generationChanged);
    connect(gameField, &KLGameField::emptyColony, this, &MainWindow::colonyIsEmpty);

    connect(gameField, &KLGameField::changeZoomIn, this, &MainWindow::zoomInChanged);
    connect(gameField, &KLGameField::changeZoomOut, this, &MainWindow::zoomOutChanged);
    connect(gameField, &KLGameField::changeRestore, this, &MainWindow::restoreChanged);

}

MainWindow::~MainWindow() {
    delete gameField;
}

void MainWindow::controlsChanged(bool enabled) {

    QAction *actionStartStop = actionCollection()->action(QStringLiteral("game_start_stop"));
    actionStartStop->setIcon(QIcon::fromTheme(enabled ? "media-playback-start-symbolic" : "media-playback-pause"));
    actionStartStop->setToolTip(enabled ? i18n("Start evolution") : i18n("Stop evolution"));
    stateChanged(enabled ? "paused_state" : "gameplay_state");
}

void MainWindow::changeMoveMode(bool enabled) {
    stateChanged(enabled ? "move_mode_state" : "paused_state");
}

void MainWindow::generationChanged(int cgen) {
    statusBar()->showMessage(i18n("Generation: %1", cgen));

}

void MainWindow::colonyIsEmpty(void) {

    statusBar()->showMessage(i18n("Colony is empty"));
}



void MainWindow::zoomInChanged(bool enable) {

    actionCollection()->action(QStringLiteral("view_zoom_in"))->setEnabled(enable);
}

void MainWindow::zoomOutChanged(bool enable) {
    actionCollection()->action(QStringLiteral("view_zoom_out"))->setEnabled(enable);
}

void MainWindow::restoreChanged(bool enable) {
    actionCollection()->action(QStringLiteral("view_actual_size"))->setEnabled(enable);
}



void MainWindow::setupToolbar() {
    QAction *prefAction = KStandardAction::preferences(gameField, &KLGameField::setupGame, actionCollection());
    prefAction->setWhatsThis(i18n("Open configuration dialog"));
    QAction *actionOpen = KStandardAction::open(gameField, &KLGameField::openAction, actionCollection());
    actionOpen->setWhatsThis(i18n("Load saved colony"));
    QAction *actionSave = KStandardAction::save(gameField, &KLGameField::saveAction, actionCollection());
    actionSave->setWhatsThis(i18n("Save current colony"));
    
    auto *wa = new QWidgetAction(this);
    wa->setDefaultWidget(mySlider);

    // Add actions for the timestep widget's functions
    QAction *resWAction = actionCollection()->addAction(QStringLiteral("timestep_control"), wa);
    resWAction->setText(i18n("Time step control"));

    QAction *actionNewGame = actionCollection()->addAction(QStringLiteral("game_new"));
    actionNewGame->setText(i18n("New &Game"));
    actionNewGame->setIcon(QIcon::fromTheme("document-new"));
    actionNewGame->setWhatsThis(i18n("Clear the field and stop evolution"));
    actionCollection()->setDefaultShortcut(actionNewGame,  Qt::ALT + Qt::Key_G);
    connect(actionNewGame, &QAction::triggered, gameField, &KLGameField::newAction);

    QAction *actionNextStep = actionCollection()->addAction(QStringLiteral("game_next_step"));
    actionNextStep->setText(i18n("&Next Step"));
    actionNextStep->setIcon(QIcon::fromTheme("media-skip-forward"));
    actionNextStep->setWhatsThis(i18n("Next single evolution step"));
    actionCollection()->setDefaultShortcut(actionNextStep,  Qt::ALT + Qt::Key_N);
    connect(actionNextStep, &QAction::triggered, gameField, &KLGameField::nextAction);

    QAction *actionStartStop = actionCollection()->addAction(QStringLiteral("game_start_stop"));
    actionStartStop->setText(i18n("&Start/Stop Game"));
    actionStartStop->setIcon(QIcon::fromTheme("media-playback-start-symbolic"));
    actionStartStop->setWhatsThis(i18n("Start or stop continuous evolution.<br> The icon changes to match the current mode"));
    actionCollection()->setDefaultShortcut(actionStartStop,  Qt::ALT + Qt::Key_S);
    connect(actionStartStop, &QAction::triggered, gameField, &KLGameField::checkTimerAndUpdate);


    QAction *actionMove = actionCollection()->addAction(QStringLiteral("move_mode"));
    actionMove->setText(i18n("&Move"));
    actionMove->setIcon(QIcon(":/images/move.png"));
    actionMove->setCheckable(true);
    actionMove->setChecked(false);
    actionMove->setWhatsThis(i18n("Click here to enter field move mode.<br>In this mode you can move the field to see "
                                  "the cells that don't fit the screen due to current zoom"));
    actionCollection()->setDefaultShortcut(actionMove,  Qt::ALT + Qt::Key_M);
    connect(actionMove, &QAction::triggered, gameField, &KLGameField::changeMoveMode);
    connect(actionMove, &QAction::triggered, this, &MainWindow::changeMoveMode);

    QAction *actionZoomIn = KStandardAction::zoomIn(gameField, &KLGameField::cZoomIn, actionCollection());
    actionZoomIn->setWhatsThis(i18n("Zoom scale in. You can also use mouse wheel.<br> Dimmed on maximum zoom"));
    QAction *actionZoomOut = KStandardAction::zoomOut(gameField, &KLGameField::cZoomOut, actionCollection());
    actionZoomOut->setWhatsThis(i18n("Zoom scale out. You can also use mouse wheel.<br>  Dimmed on minimal zoom"));
    actionZoomOut->setEnabled(false);

    QAction *actZoom = KStandardAction::actualSize(gameField, &KLGameField::cRestore, actionCollection());
    actZoom->setWhatsThis(i18n("Restore initial zoom.<br>Dimmed when the zoom is minimal"));
    actZoom->setEnabled(false);

    const QSize &wsize = QApplication::desktop()->size() * 0.7;
    setupGUI();
    setMinimumSize(wsize);

}
