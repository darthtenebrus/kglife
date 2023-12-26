//
// Created by esorochinskiy on 12.12.23.
//

#ifndef KGLIFE_CELLSGENERATOR_H
#define KGLIFE_CELLSGENERATOR_H

#include <QThread>
#include <QSize>
#include <QMutex>


class CellsGenerator : public QThread {

public:
    explicit CellsGenerator(const QSize &maxLimit = QSize(-1, -1), int quantity = -1, QObject *parent = nullptr);

    void init(const QSize &ml, int mQuantity);

Q_OBJECT
protected:
    void run() override;

signals:
    void resultReady(int, int);
    void resultFinished();


private:
    QSize mLimit;
    int mQuantity;
    QMutex mutex;

    int localRand(int max);

};


#endif //KGLIFE_CELLSGENERATOR_H
