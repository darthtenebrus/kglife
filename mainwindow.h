//
// Created by esorochinskiy on 26.09.23.
//

#ifndef KGLIVE_MAINWINDOW_H
#define KGLIVE_MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include "KLGameField.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;


private:
    Ui::MainWindow *ui;
    KLGameField *gameField;
    QSlider *timerSlider;

public slots:
    void controlsChanged(bool);
    void generationChanged(int);
    void colonyIsEmpty(void);


};


#endif //KGLIVE_MAINWINDOW_H
