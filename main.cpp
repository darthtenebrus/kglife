#include <QApplication>
#include <QPushButton>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto *w = new MainWindow();

    w->showMaximized();
    int res = QApplication::exec();
    delete w;
    return res;
}
