//
// Created by esorochinskiy on 12.12.23.
//

#ifndef KGLIFE_GENERATORPAGE_H
#define KGLIFE_GENERATORPAGE_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class GeneratorPage; }
QT_END_NAMESPACE

class GeneratorPage : public QWidget {
Q_OBJECT

public:
    explicit GeneratorPage(QWidget *parent = nullptr);

    ~GeneratorPage() override;

private:
    Ui::GeneratorPage *ui;
};


#endif //KGLIFE_GENERATORPAGE_H
