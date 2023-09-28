//
// Created by esorochinskiy on 26.09.23.
//

#ifndef KGLIVE_KLGAMEFIELD_H
#define KGLIVE_KLGAMEFIELD_H


#include <QWidget>

class KLGameField : public QWidget {
Q_OBJECT
public:
    KLGameField(QWidget *parent = 0);

    ~KLGameField() override;


protected:
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void cancelTimerInstantly(void);
    void recalculate(void);
    void actualDoRePaint();
    uchar *initLayer(uchar *);
    void swapLayers(void);
    int calculateNeighbors(int, int);
    uchar fromMainLayer(int, int);
    void copyToLayer(uchar *, int, int, uchar);

    QPoint getMainOffset();
    int m_Generation = 0;
    int m_fieldWidth;
    int m_fieldHeight;
    int m_cellsX;
    int m_remX;
    int m_cellsY;
    int m_remY;
    uchar *m_MainLayer = nullptr;
    uchar *m_NextStepLayer = nullptr;
    QTimer *evoTimer = nullptr;


public slots:
    void nextAction(bool);
    void checkTimerAndUpdate(bool);
    void nextGeneration(void);

signals:
    void changeControls(bool);
    void changeGeneration(int);

};


#endif //KGLIVE_KLGAMEFIELD_H
