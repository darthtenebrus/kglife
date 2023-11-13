//
// Created by esorochinskiy on 26.09.23.
//

#ifndef KGLIVE_MAINWINDOW_H
#define KGLIVE_MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QSettings>
#include "KLGameField.h"
#include "preferences.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    static QString orgName;

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    KLGameField *gameField;
    QSlider *timerSlider;

    QSettings settings;

    PreferencesType m_data;

    void fillDataFromSettings(void);
    void writeSettingsDirect(void);

public slots:
    void controlsChanged(bool);
    void generationChanged(int);
    void colonyIsEmpty(void);
    void settingChanged(const QString &, const QColor &);

    void zoomInChanged(bool);
    void zoomOutChanged(bool);

    void restoreChanged(bool);



};


#endif //KGLIVE_MAINWINDOW_H
