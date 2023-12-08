//
// Created by esorochinskiy on 26.09.23.
//

#ifndef KGLIVE_KLGAMEFIELD_H
#define KGLIVE_KLGAMEFIELD_H


#include <QWidget>

class KLGameField : public QWidget {
Q_OBJECT
public:
    KLGameField(int, QWidget * = nullptr);
    ~KLGameField() override;
    void cancelTimerInstantly(void);


protected:
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void recalcScreenCells(void);

    int sgn(int val);
    void intentToMoveField(int x, int y);

private:
    void initTotalCells();
    void changeDelta(int);
    void restoreScreen();

    void recalculate(void);
    void actualDoRePaint();
    uchar *initLayer(uchar *);
    void swapLayers(void);
    int calculateNeighbors(int, int);
    uchar fromMainLayer(int, int);
    void copyToLayer(uchar *, int, int, uchar);
    bool checkMousePosition(QPoint &) const;

    void tryLoadFromFile(const QString &);
    void trySaveToFile(const QString &);
    QString forceFileNameDialog();


    [[nodiscard]]
    QPoint getMainOffset() const;

    [[nodiscard]]
    QSize getStandardFieldDefs(int &, int &) const;


    int m_TimerInterval;
    int m_Generation = 0;

    int m_cellsX = 0;
    int m_remScrX;
    int m_cellsY = 0;
    int m_remScrY;
    int m_MaxMemOffsetX = 0;
    int m_MaxMemOffsetY = 0;

    int m_CurrMemOffsetX = 0;
    int m_CurrMemOffsetY = 0;


    int m_cellSize;
    int m_ScrCellsX;
    int m_ScrCellsY;

    uchar *m_MainLayer = nullptr;
    uchar *m_NextStepLayer = nullptr;
    QTimer *evoTimer = nullptr;
    QCursor m_Cursor;
    bool m_LeftbPressed = false;
    bool m_MoveMode = false;

    static QString CurrentFilePath;
    static void setCurrentPath(KLGameField *th, QString cpath);


public slots:
    void newAction(bool);
    void nextAction(bool);
    void openAction(bool);
    void saveAction(bool);
    void saveAsAction(bool);
    void checkTimerAndUpdate(bool);
    void timerChanged(int);

    void setupGame();
    void printGame();
    void changeMoveMode(bool);

    void cZoomIn(bool);
    void cZoomOut(bool);
    void cRestore(bool);
    void cdApply(const QString &);

private slots:
    void nextGeneration(void);

signals:
    void changeControls(bool);
    void changeGeneration(int);
    void emptyColony(void);

    void changeZoomIn(bool);
    void changeZoomOut(bool);
    void changeRestore(bool);
    void changeCurrentFile(const QString &);
};


#endif //KGLIVE_KLGAMEFIELD_H
