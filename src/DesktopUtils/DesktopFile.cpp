#include <DesktopsUtils/DesktopFile.hpp>

#include <core.hpp>

void logToFile(QString msg, QString file = "/home/_maqix_/programowanie/C++/Universe/log.txt") {
    QFile logFile(file);
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&logFile);
        out << msg << "\n";
        logFile.close();
    } else {
        qWarning() << "Unable to open log file: " << file;
    }
}

int DesktopFile::run() {
    logToFile("execCmd przed trimmed: " + desktopEntry.exec);
    logToFile("starting btw");
    // if (desktopEntry.noDisplay || desktopEntry.hidden) {
    //     qWarning() << "Application is marked as hidden or noDisplay, not launching.";
    //     return -1;
    // }

    logToFile("execCmd before trimmed: " + desktopEntry.exec);

    QString execCmd = desktopEntry.exec.trimmed();
    logToFile("t");

    if (execCmd.isEmpty()) {
        logToFile("x");
        qWarning() << "Exec command is empty!";
        return -1;
    }

    logToFile("fr");

    // execCmd.replace("%f", "").replace("%F", "");
    // execCmd.replace("%u", "").replace("%U", "");
    // execCmd.replace("%i", getLocalizedValue(desktopEntry.icon));
    // execCmd.replace("%c", getLocalizedValue(desktopEntry.name));
    // execCmd.replace("%k", path);
    // execCmd.replace("%d", QFileInfo(path).absolutePath());
    // execCmd.replace("%D", QFileInfo(path).absolutePath());
    // execCmd.replace("%n", QFileInfo(path).completeBaseName());
    // execCmd.replace("%N", QFileInfo(path).completeBaseName());

    auto cmd = (execCmd.toStdString() + " &").c_str();
    // qWarning() << "cmd: " << cmd;
    logToFile(QString("cmd: ") + cmd);

    system(cmd);

    // QStringList args = QProcess::splitCommand(execCmd);
    // if (args.isEmpty()) {
    //     qWarning() << "Invalid Exec format!";
    //     return -1;
    // }

    // QString program = args.takeFirst();

    // QProcess* process;
    // process->setProgram(program);
    // process->setArguments(args);
    // process->setProcessChannelMode(QProcess::ForwardedChannels);
    // process->start();
    // process->waitForStarted();

    // if (desktopEntry.terminal) {
    //     QString terminal = qEnvironmentVariable("TERMINAL", "x-terminal-emulator");
    //     QProcess::startDetached(terminal, QStringList() << "-e" << program << args);
    // } else {
    //     QProcess::startDetached(program, args);
    // }

    // if (!process->waitForStarted())
    //     delete process;
    // else
    //     process->setParent(nullptr);

    return 0;
}

QString DesktopFile::getLocalizedValue(const QMap<QString, QString>& map) {
    if (map.isEmpty()) {
        return QString();
    }

    QLocale locale = QLocale::system();
    QString langFull = locale.name();
    QString langShort = locale.name().split('_').takeFirst();

    if (map.contains(langFull)) {
        return map[langFull];
    }
    if (map.contains(langShort)) {
        return map[langShort];
    }
    if (map.contains("")) {
        return map[""];
    }
    if (map.contains("default")) { return map["default"]; }

    return QString();
}

bool DesktopFile::getIcon(QIcon& icon) {
    if (desktopEntry.icon.isEmpty()) {
        return false;
    }

    QString iconPath = DesktopFile::getLocalizedValue(desktopEntry.icon);

    if (QFile::exists(iconPath)) {
        icon = QIcon(iconPath);
        return !icon.isNull();
    }

    icon = QIcon::fromTheme(iconPath);
    return !icon.isNull();
}

QList<DesktopFile> DesktopFile::loadAll(const QString& dir) {
    QDir directory(dir);
    QStringList files = directory.entryList(QStringList() << "*.desktop", QDir::Files);

    QList<DesktopFile> desktopFiles;
    for (const QString& file : files) {
        DesktopFile desktopFile = loadDesktop(directory.filePath(file));
        if (!desktopFile.desktopEntry.noDisplay && !desktopFile.desktopEntry.hidden) {
            desktopFiles.append(desktopFile);
        }
    }

    return desktopFiles;
}