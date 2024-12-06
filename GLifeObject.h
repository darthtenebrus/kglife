//
// Created by esorochinskiy on 05.12.2024.
//

#ifndef KGLIFE_GLIFEOBJECT_H
#define KGLIFE_GLIFEOBJECT_H


#include <QObject>
#include <QPoint>

class GLifeObject : public QObject {
Q_OBJECT
public:
    explicit GLifeObject(const QPoint &, QObject *parent = nullptr);

    GLifeObject(const GLifeObject &);

    ~GLifeObject() override;

    void add(const QVector<int> &);

    bool hasLinkedForChorde(const QVector<int> &checked);

    GLifeObject& operator=(const GLifeObject& other) noexcept ;

    [[nodiscard]]
    const QList<QVector<int>> &listChordes() const;

    void setAbsol(const QPoint &);

private:

    QPoint absol;
    QList<QVector<int>> chordes;

};


#endif //KGLIFE_GLIFEOBJECT_H
