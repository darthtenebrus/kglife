//
// Created by esorochinskiy on 10.11.23.
//

#ifndef KGLIFE_CONFIGDIALOG_H
#define KGLIFE_CONFIGDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class ConfigDialog; }
QT_END_NAMESPACE

class ConfigDialog : public QDialog {
Q_OBJECT

public:
    explicit ConfigDialog(QColor &, QColor &, QWidget *parent = nullptr);

    ~ConfigDialog() override;

    [[nodiscard]] const QColor &getMCellColor() const;

    [[nodiscard]] const QColor &getMBackColor() const;


    const QColor &getMBetweenColor() const;

private:
    Ui::ConfigDialog *ui;
    QColor mCellColor;
    QColor mBackColor;
    QColor mBetweenColor;

    void setButtonIconColor(QPushButton *, QColor &);

private slots:
    void currentChanged(const QModelIndex &, const QModelIndex &);

};


#endif //KGLIFE_CONFIGDIALOG_H
