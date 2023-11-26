#include <QApplication>
#include <QCommandLineParser>
#include <QPushButton>
#include <QDir>
#include <iostream>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <KLocalizedString>
#include <KCrash>
#include <KAboutData>
#include <KMessageBox>
#include "mainwindow.h"
#include "version.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("kglife");

    try {
        const QStringList &cmd_args = QCoreApplication::arguments();
        if (!cmd_args.empty() && cmd_args.size() > 1) {
            const QString &firstArg = cmd_args.value(0);
            const QString &progName = firstArg.contains(QDir::separator()) ?
                                      firstArg.section(QDir::separator(), -1, -1) : firstArg;
            std::cout << progName.toStdString() << " " << i18n("Simple Game Of Life realization").toStdString() << "\n";
            std::cout << i18n("Created by: ")
                    .toStdString() << "Eugene E. Sorochinskiy <manager@darkguard.net>\n\n";
            const QString &optVal = cmd_args.value(1);
            if (optVal.toLower() == "-h" || optVal == "--help") {
                std::cout << i18n("Usage %1 [options]", progName).toStdString() << "\n";
                std::cout << i18n("Valid options:").toStdString() << "\n";
                std::cout << "-h, -H, --help\t\t" << i18n("show this help").toStdString() << "\n";
                std::cout << "-v, -V, --version\t" << i18n("display version").toStdString() << "\n\n";

            } else if (optVal.toLower() == "-v" || optVal == "--version") {
                std::cout << progName.toStdString() << " v." << APP_VERSION << "\n\n";
            } else {
                std::cout << i18n("Unknown option %1", cmd_args.value(1)).toStdString() << "\n";
                std::cout << i18n("Launch %1 -h or %1 --help for help", progName).toStdString() << "\n\n";

            }
            std::cout << i18n("Launch this application without any parameters to see its main functional")
                    .toStdString() << "\n\n";
            return 0;
        }
    } catch (const std::exception &ex) {
        return 1;
    }

    const QString &semaph_id = "kglife_semaphore";
    const QString &shared_id = "kglife_shared_mem";
    QSystemSemaphore semaphore(semaph_id, 1);
    semaphore.acquire();

#ifndef Q_OS_WIN32
    QSharedMemory nix_fix_shared_memory(shared_id);
    if (nix_fix_shared_memory.attach())
    {
        nix_fix_shared_memory.detach();
    }
#endif

    QSharedMemory sharedMemory(shared_id);
    bool is_running = false;
    if (sharedMemory.attach())
    {
        is_running = true;
    } else {
        sharedMemory.create(1);
    }
    semaphore.release();

    if (is_running) {
        KMessageBox::error(nullptr, i18n("Application Already Running"), i18n("Already Running"));
        return 1;
    }

    KAboutData aboutData(QStringLiteral("kglife"),
                         i18n("Game Of Life"), APP_VERSION,
                         i18n("Convey's Game Of Life for KDE"),
                         KAboutLicense::GPL_V3, "2023 E.Sorochinskiy",
                         i18n("Construct and experement with many cell colonies"),
                         "https://www.darkguard.net");

    aboutData.addAuthor("Eugene E. Sorochinskiy",
                        i18n("Original author"),
                        "manager@darkguard.net",
                        "https://darkguard.net");
    aboutData.setTranslator("Eugene E. Sorochinskiy",
                            "webmaster@darkguard.net");
    KAboutData::setApplicationData(aboutData);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/desktop/64x64/kglife.png"), QSize(), QIcon::Normal, QIcon::Off);
    QApplication::setWindowIcon(icon);
    KCrash::initialize();

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    auto *w(new MainWindow());

    w->showMaximized();
    return QApplication::exec();
}
