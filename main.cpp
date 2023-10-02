#include <QApplication>
#include <QTranslator>
#include <QPushButton>
#include <QDir>
#include <iostream>
#include "mainwindow.h"
#include "version.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTranslator mainTranslator;
    // look up e.g. :/i18n/myapp_de.qm
    if (mainTranslator.load(
            QLocale(),
            QLatin1String("mainwindow"),
            QLatin1String("_"),
            QLatin1String(":/i18n"))) {
        QApplication::installTranslator(&mainTranslator);
    }

    try {
        const QStringList &cmd_args = QCoreApplication::arguments();
        if (!cmd_args.empty() && cmd_args.size() > 1) {
            const QString &firstArg = cmd_args.value(0);
            const QString &progName = firstArg.contains(QDir::separator()) ?
                                      firstArg.section(QDir::separator(), -1, -1) : firstArg;
            std::cout << progName.toStdString() << " " << QObject::tr("Simple Game Of Life realization for KDE").toStdString() << "\n";
            std::cout << QObject::tr("Created by: ")
                    .toStdString() << "Eugene E. Sorochinskiy <manager@darkguard.net>\n\n";
            const QString &optVal = cmd_args.value(1);
            if (optVal.toLower() == "-h" || optVal == "--help") {
                std::cout << QObject::tr("Usage %1 [options]")
                        .arg(progName).toStdString() << "\n";
                std::cout << QObject::tr("Valid options:").toStdString() << "\n";
                std::cout << "-h, -H, --help\t\t" << QObject::tr("show this help").toStdString() << "\n";
                std::cout << "-v, -V, --version\t" << QObject::tr("display version").toStdString() << "\n\n";

            } else if (optVal.toLower() == "-v" || optVal == "--version") {
                std::cout << progName.toStdString() << " v." << APP_VERSION << "\n\n";
            } else {
                std::cout << QObject::tr("Unknown option %1").arg(cmd_args.value(1)).toStdString() << "\n";
                std::cout << QObject::tr("Launch %1 -h or %1 --help for help")
                        .arg(progName).toStdString() << "\n\n";

            }
            std::cout << QObject::tr("Launch this application without any parameters to see its main functional")
                    .toStdString() << "\n\n";
            return 0;
        }
    } catch (const std::exception &ex) {
        return 1;
    }



    auto *w(new MainWindow());

    w->showMaximized();
    int res = QApplication::exec();
    delete w;
    return res;
}
