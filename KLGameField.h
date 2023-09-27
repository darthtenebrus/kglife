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

    void recalculate(void);

protected:
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void actualDoRePaint();
    unsigned char *initLayer(unsigned char *);
    void setInitialCells(void);
    void swapLayers(void);
    int calculateNeighbors(int x, int y);
    int m_fieldWidth;
    int m_fieldHeight;
    int m_cellsX;
    int m_remX;
    int m_cellsY;
    int m_remY;
    unsigned char *m_MainLayer = nullptr;
    unsigned char *m_NextStepLayer = nullptr;

};


#endif //KGLIVE_KLGAMEFIELD_H
