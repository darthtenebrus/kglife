//
// Created by esorochinskiy on 26.09.23.
//

#ifndef KGLIVE_KLGAMEFIELD_H
#define KGLIVE_KLGAMEFIELD_H


#include <QWidget>

class KLGameField : public QWidget {
Q_OBJECT
public:
    KLGameField(const QColor &cellsColor, const QColor &BackgroundColor, int timerInterval, QWidget *parent = nullptr);
    ~KLGameField() override;
    void cancelTimerInstantly(void);

protected:
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:

    void recalculate(void);
    void actualDoRePaint();
    uchar *initLayer(uchar *);
    void swapLayers(void);
    int calculateNeighbors(int, int);
    uchar fromMainLayer(int, int);
    void copyToLayer(uchar *, int, int, uchar);

    bool checkMousePosition(QPoint &) const;

    QPoint getMainOffset() const;
    int m_TimerInterval;
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
    QCursor m_Cursor;
    bool m_bPressed = false;

    QColor m_ColorCells;
    QColor m_ColorBackground;

public slots:
    void newAction(bool);
    void nextAction(bool);
    void openAction(bool);
    void saveAction(bool);
    void checkTimerAndUpdate(bool);
    void timerChanged(int);

    void changeCellsColor(bool);
    void changeBackgroundColor(bool);

private slots:
    void nextGeneration(void);

signals:
    void changeControls(bool);
    void changeGeneration(int);
    void emptyColony(void);



};


#endif //KGLIVE_KLGAMEFIELD_H
