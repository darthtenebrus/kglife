//
// Created by esorochinskiy on 26.09.23.
//

#ifndef KGLIVE_MAINWINDOW_H
#define KGLIVE_MAINWINDOW_H

#include <QMenuBar>
#include <QMainWindow>
#include <QSlider>
#include <QSettings>
#include <KXmlGuiWindow>
#include <QLabel>
#include <KToggleAction>
#include <KHamburgerMenu>
#include "KLGameField.h"
#include "myslider.h"


class MainWindow : public KXmlGuiWindow {
Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void changeEvent(QEvent *event) override;


private:
    MySlider *mySlider;
    KLGameField *gameField;
    QLabel *mCurrentFile;

    KToggleAction *_toggleMenuBarAction;
    KHamburgerMenu *_hamburgerMenu;
    void setupToolbar();
    void setupStatusBar();
    void fillUpperHamburger(QMenu *);



public slots:
    void controlsChanged(bool);
    void generationChanged(int);
    void colonyIsEmpty(void);

    void zoomInChanged(bool);
    void zoomOutChanged(bool);
    void restoreChanged(bool);
    void changeMoveMode(bool);
    void changeSelectMode(bool);
    void currentFileChanged(const QString &);

private slots:
        void updateHamburgerMenu();

};


#endif //KGLIVE_MAINWINDOW_H
