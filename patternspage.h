//
// Created by esorochinskiy on 26.11.23.
//

#ifndef KGLIFE_PATTERNSPAGE_H
#define KGLIFE_PATTERNSPAGE_H

#include <QListWidget>
#include <KConfigSkeleton>
#include <KConfigDialog>
#include "ui_patternspage.h"


QT_BEGIN_NAMESPACE
namespace Ui { class PatternsPage; }
QT_END_NAMESPACE

class PatternsPage : public QWidget, public Ui::PatternsPage {
Q_OBJECT

public:
    explicit PatternsPage(KConfigSkeleton *pSkeleton, KConfigDialog *config, QWidget *parent = nullptr);

    ~PatternsPage() override;

private:

    void fillPatternList();
    void setupData(KConfigSkeleton *pSkeleton);
    void cleanup();

    QGraphicsScene *m_Scene;

private slots:
    void patternChanged(int);
    void restoreSettingsClicked(bool);

};


#endif //KGLIFE_PATTERNSPAGE_H
