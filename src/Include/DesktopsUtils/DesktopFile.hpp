#pragma once

#include <core.hpp>

/*
    DesktopFile

    this struct represents the .desktop file
*/
class DesktopFile {
  public:
    QString path;

    struct DesktopEntry {
        QMap<QString, QString> name;
        QMap<QString, QString> comment;
        QMap<QString, QString> icon;

        QString type;
        QString exec;

        QStringList categories;
        QStringList keywords;
        QStringList mimeTypes;

        QStringList onlyShowIn;
        QStringList notShowIn;

        bool terminal;
        bool noDisplay;
        bool hidden;
        bool dBusActivatable;
        bool prefersNonDefaultGPU;

        QString startupWMClass;
        QString singleMainWindow;
    } desktopEntry;

    struct DesktopAction {
        QMap<QString, QString> name;
        QString exec;
        QString icon;
    };

    QMap<QString, DesktopAction> actions;

    int run();
    bool getIcon(QIcon& icon);

    static QString getLocalizedValue(const QMap<QString, QString>& map);
    static QList<DesktopFile> loadAll(const QString& dir = "/usr/share/applications");
};

DesktopFile loadDesktop(const QString& filePath);