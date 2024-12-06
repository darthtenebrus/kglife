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
    explicit GLifeObject(QObject *parent = nullptr);

    ~GLifeObject() override;

    void add(const std::vector<int> &);
    [[nodiscard]]
    const QList<std::vector<int>> &listChordes() const;

    void setAbsol(const QPoint &);

private:

    QPoint absol;
    QList<std::vector<int>> chordes;

};


#endif //KGLIFE_GLIFEOBJECT_H
