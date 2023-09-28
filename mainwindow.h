//
// Created by esorochinskiy on 26.09.23.
//

#ifndef KGLIVE_MAINWINDOW_H
#define KGLIVE_MAINWINDOW_H

#include <QMainWindow>
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

public slots:
    void controlsChanged(bool);
    void generationChanged(int);

};


#endif //KGLIVE_MAINWINDOW_H
