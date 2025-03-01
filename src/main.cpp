#include <core.hpp>

#include <X11/X-Utils.hpp>

#include <DesktopsUtils/DesktopFile.hpp>

#include <Universe/MainLayout.hpp>
#include <Universe/Widgets/Dock.hpp>

void logToFile3(QString msg, QString file = "/home/_maqix_/programowanie/C++/Universe/log.txt") {
    QFile logFile(file);
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&logFile);
        out << msg << "\n";
        logFile.close();
    } else {
        qWarning() << "Unable to open log file: " << file;
    }
}

int main(int argc, char* argv[]) {
    QApplication Universe(argc, argv);

    QWidget w;
    w.setWindowTitle("Universe");

    w.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
    w.setWindowFlags(Qt::Window);
    w.setAttribute(Qt::WA_X11NetWmWindowTypeDesktop);
    w.resize(1920, 1080);

    MainLayout layout(&w);
    DockWidget* dock = layout.dock();
    auto apps = DesktopFile::loadAll();
    for (auto& app : apps.mid(0, apps.length() - 80)) {
        QIcon icon;
        bool success = app.getIcon(icon);
        dock->addButton(success ? icon : QIcon(), [&]() {
            logToFile3("H");
            logToFile3(QString("running ") + /*DesktopFile::getLocalizedValue(app.desktopEntry.name)*/ "k" + " btw");
            app.run();
            // system(app.desktopEntry.exec.toStdString().c_str());
            // system("tilix &");
        });
        // QPushButton* button = new QPushButton(success ? icon : QIcon(), "", &w);
        // QWidget::connect(button, &QPushButton::clicked, [&]() {
        //     logToFile3("H");
        //     logToFile3(QString("running ") + DesktopFile::getLocalizedValue(app.desktopEntry.name) + " btw");
        //     app.run();
        //     system(app.desktopEntry.exec.toStdString().c_str());
        //     system("tilix &");
        // });
    }
    // for (int i = 10; i > 0; i--) {
    //     auto brave = loadDesktop("/usr/share/applications/brave-browser.desktop");
    //     QIcon icon;
    //     bool success = brave.getIcon(icon);
    //     dock->addButton(success ? icon : QIcon(), [&brave]() { brave.run(); });
    // }
    w.setLayout(&layout);

    // QVBoxLayout* layout = new QVBoxLayout(&w);
    // layout->setContentsMargins(0, 0, 0, 0);
    // layout->setSpacing(0);

    // DockWidget* dock = new DockWidget();
    // layout->addWidget(dock);

    setDesktopWindowType(&w);
    w.show();

    return Universe.exec();
}