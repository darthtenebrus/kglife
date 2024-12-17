//
// Created by esorochinskiy on 26.09.23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <KLocalizedString>
#include <KStandardAction>
#include <KActionCollection>
#include <KToolBar>
#include <KXMLGUIFactory>

#include <QDomElement>
#include <QDesktopWidget>
#include <QApplication>
#include <QStatusBar>
#include "mainwindow.h"
#include "kglife.h"
#include "myslider.h"


MainWindow::MainWindow(QWidget *parent) :
        KXmlGuiWindow(parent) {

    mySlider = new MySlider(Settings::slidervalue(), Qt::Horizontal, toolBar());
    mySlider->retranslateUi();

    gameField = new KLGameField(mySlider->value(), this);
    connect(mySlider, &QSlider::valueChanged, gameField, &KLGameField::timerChanged);

    gameField->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

    setCentralWidget(gameField);
    setupToolbar();
    setupStatusBar();

    connect(gameField, &KLGameField::changeControls, this, &MainWindow::controlsChanged);
    connect(gameField, &KLGameField::changeGeneration, this, &MainWindow::generationChanged);
    connect(gameField, &KLGameField::emptyColony, this, &MainWindow::colonyIsEmpty);

    connect(gameField, &KLGameField::changeZoomIn, this, &MainWindow::zoomInChanged);
    connect(gameField, &KLGameField::changeZoomOut, this, &MainWindow::zoomOutChanged);
    connect(gameField, &KLGameField::changeRestore, this, &MainWindow::restoreChanged);
    connect(gameField, &KLGameField::changeCurrentFile, this, &MainWindow::currentFileChanged);
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

void MainWindow::changeSelectMode(bool enabled) {
    stateChanged(enabled ? "select_mode_state" : "paused_state");
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
    QAction *printAction = KStandardAction::print(gameField, &KLGameField::printGame, actionCollection());
    printAction->setWhatsThis(i18n("Print Pattern"));

    QAction *prefAction = KStandardAction::preferences(gameField, &KLGameField::setupGame, actionCollection());
    prefAction->setWhatsThis(i18n("Open configuration dialog"));
    QAction *actionOpen = KStandardAction::open(gameField, &KLGameField::openAction, actionCollection());
    actionOpen->setWhatsThis(i18n("Load saved colony"));
    QAction *actionSave = KStandardAction::save(gameField, &KLGameField::saveAction, actionCollection());
    actionSave->setWhatsThis(i18n("Save current colony"));

    QAction *actionSaveAs = KStandardAction::saveAs(gameField, &KLGameField::saveAsAction, actionCollection());
    actionSaveAs->setWhatsThis(i18n("Save current colony with different name"));

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

    QAction *actionGenerate = actionCollection()->addAction(QStringLiteral("generate_random"));
    actionGenerate->setText(i18n("Generate &Random"));
    actionGenerate->setWhatsThis(i18n("Generates random pattern"));
    actionCollection()->setDefaultShortcut(actionGenerate,  Qt::ALT + Qt::Key_R);
    connect(actionGenerate, &QAction::triggered, gameField, &KLGameField::startCellsGenerator);

    QAction *actionNextStep = actionCollection()->addAction(QStringLiteral("game_next_step"));
    actionNextStep->setText(i18n("&Next Step"));
    actionNextStep->setIcon(QIcon::fromTheme("media-skip-forward"));
    actionNextStep->setWhatsThis(i18n("Next single evolution step"));
    actionCollection()->setDefaultShortcut(actionNextStep,  Qt::ALT + Qt::Key_N);
    connect(actionNextStep, &QAction::triggered, gameField, &KLGameField::singleAction);

    QAction *actionStartStop = actionCollection()->addAction(QStringLiteral("game_start_stop"));
    actionStartStop->setText(i18n("&Start/Stop Game"));
    actionStartStop->setIcon(QIcon::fromTheme("media-playback-start-symbolic"));
    actionStartStop->setWhatsThis(i18n("Start or stop continuous evolution.<br> The icon changes to match the current mode"));
    actionCollection()->setDefaultShortcut(actionStartStop,  Qt::ALT + Qt::Key_S);
    connect(actionStartStop, &QAction::triggered, gameField, &KLGameField::checkTimerAndUpdate);

    // Move
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

    // Analysis

    QAction *actionAnalys = actionCollection()->addAction(QStringLiteral("sort_chordes"));
    actionAnalys->setText(i18n("Object chordes coverage"));
    connect(actionAnalys, &QAction::triggered, gameField, &KLGameField::chordesAction);

    // Select

    QAction *actionSelect = actionCollection()->addAction(QStringLiteral("select_mode"));
    actionSelect->setText(i18n("&Mouse selection"));
    actionSelect->setIcon(QIcon::fromTheme("edit-select-all-symbolic"));
    actionSelect->setCheckable(true);
    actionSelect->setChecked(false);
    actionSelect->setWhatsThis(i18n("Click here to enter select mode.<br>In this mode you can select alive cells to cut and copy"));
    actionCollection()->setDefaultShortcut(actionSelect,  Qt::ALT + Qt::Key_L);
    connect(actionSelect, &QAction::triggered, gameField, &KLGameField::changeSelectMode);
    connect(actionSelect, &QAction::triggered, this, &MainWindow::changeSelectMode);

    QAction *actionClearSel = actionCollection()->addAction(QStringLiteral("clear_selection"));
    actionClearSel->setText(i18n("&Clear selection"));
    actionClearSel->setIcon(QIcon::fromTheme("edit-select-none"));
    actionClearSel->setWhatsThis(i18n("Click here to clear selection"));
    actionCollection()->setDefaultShortcut(actionClearSel,  Qt::ALT + Qt::Key_C);
    connect(actionClearSel, &QAction::triggered, gameField, &KLGameField::onSelectClear);
    // Zoom In
    QAction *actionZoomIn = KStandardAction::zoomIn(gameField, &KLGameField::cZoomIn, actionCollection());
    actionZoomIn->setWhatsThis(i18n("Zoom scale in. You can also use mouse wheel.<br> Dimmed on maximum zoom"));
    QAction *actionZoomOut = KStandardAction::zoomOut(gameField, &KLGameField::cZoomOut, actionCollection());
    actionZoomOut->setWhatsThis(i18n("Zoom scale out. You can also use mouse wheel.<br>  Dimmed on minimal zoom"));
    actionZoomOut->setEnabled(false);

    QAction *actZoom = KStandardAction::actualSize(gameField, &KLGameField::cRestore, actionCollection());
    actZoom->setWhatsThis(i18n("Restore initial zoom.<br>Dimmed when the zoom is minimal"));
    actZoom->setEnabled(false);

    _toggleMenuBarAction = KStandardAction::showMenubar(menuBar(), &QMenuBar::setVisible,
                                                                       actionCollection());
    actionCollection()->setDefaultShortcut(_toggleMenuBarAction, Qt::SHIFT + Qt::Key_M);

    const QSize &wsize = QApplication::desktop()->size() * 0.7;

    // Hamburger menu for when the menubar is hidden
    _hamburgerMenu = KStandardAction::hamburgerMenu(nullptr, nullptr, actionCollection());
    _hamburgerMenu->setShowMenuBarAction(_toggleMenuBarAction);
    _hamburgerMenu->setMenuBar(menuBar());
    connect(_hamburgerMenu, &KHamburgerMenu::aboutToShowMenu, this, &MainWindow::updateHamburgerMenu);

    setupGUI();

    setMinimumSize(wsize);

}

void MainWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        if (windowState() & Qt::WindowMinimized) {
            gameField->cancelTimerInstantly();
        }
    }
    QWidget::changeEvent(event);
}

void MainWindow::setupStatusBar() {
    mCurrentFile = new QLabel(i18n("Empty file"), toolBar());
    statusBar()->addPermanentWidget(mCurrentFile);
}

void MainWindow::currentFileChanged(const QString &current) {
    mCurrentFile->setText(i18n("File name: %1", current));
}

void MainWindow::updateHamburgerMenu() {

    KActionCollection *collection = actionCollection();

    QMenu *menu = _hamburgerMenu->menu();
    if (!menu) {
        menu = new QMenu(widget());
        _hamburgerMenu->setMenu(menu);
    } else {
        menu->clear();
    }


    fillUpperHamburger(menu);
    menu->addSeparator();

    auto configureMenu = menu->addMenu(QIcon::fromTheme(QStringLiteral("configure")),
                                       dynamic_cast<QMenu *>(factory()->container(QStringLiteral("settings"),
                                                                                 nullptr))->title());
    configureMenu->addAction(toolBarMenuAction());
    configureMenu->addSeparator();
    configureMenu->addAction(collection->action(KStandardAction::name(KStandardAction::SwitchApplicationLanguage)));
    configureMenu->addAction(collection->action(KStandardAction::name(KStandardAction::KeyBindings)));
    configureMenu->addAction(collection->action(KStandardAction::name(KStandardAction::ConfigureToolbars)));
    configureMenu->addAction(collection->action(KStandardAction::name(KStandardAction::Preferences)));
    _hamburgerMenu->hideActionsOf(configureMenu);

}

void MainWindow::fillUpperHamburger(QMenu *menu) {

    QStringList qsl = {
            QStringLiteral("file"),
            QStringLiteral("game"),
            QStringLiteral("view"),
            QStringLiteral("selection"),
            QStringLiteral("analysis")

    };

    for (const auto &mName : qsl) {
        auto fileMenu = dynamic_cast<QMenu *>(factory()->container(mName,this));
        menu->addMenu(fileMenu);
        if (mName == "analysis") {
            QPoint minOrigin = QPoint(gameField->getCellsX(), gameField->getCellsY());
            QPoint maxOrigin = QPoint(0, 0);

            bool isToAnalis = gameField->doMiniMaxTestsOnLayer(gameField->getMainLayer(), minOrigin, maxOrigin);
            fileMenu->setEnabled(isToAnalis);

        }
    }

}
