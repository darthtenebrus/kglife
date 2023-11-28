//
// Created by esorochinskiy on 26.09.23.
//

#ifndef KGLIVE_MAINWINDOW_H
#define KGLIVE_MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QSettings>
#include <KXmlGuiWindow>
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
    void setupToolbar();



public slots:
    void controlsChanged(bool);
    void generationChanged(int);
    void colonyIsEmpty(void);

    void zoomInChanged(bool);
    void zoomOutChanged(bool);
    void restoreChanged(bool);

    void changeMoveMode(bool);
};


#endif //KGLIVE_MAINWINDOW_H
