//
// Created by esorochinskiy on 26.11.23.
//

#ifndef KGLIFE_PATTERNSPAGE_H
#define KGLIFE_PATTERNSPAGE_H

#include <QListWidget>
#include <KConfigSkeleton>
#include "ui_patternspage.h"


QT_BEGIN_NAMESPACE
namespace Ui { class PatternsPage; }
QT_END_NAMESPACE

class PatternsPage : public QWidget, public Ui::PatternsPage {
Q_OBJECT

public:
    explicit PatternsPage(KConfigSkeleton * config, QWidget *parent = nullptr);

private:
    void fillPatternList();
    void setupData(KConfigSkeleton *pSkeleton);

private slots:
    void patternChanged(int);

};


#endif //KGLIFE_PATTERNSPAGE_H
