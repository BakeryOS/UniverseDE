#include <core.hpp>

#include <DesktopsUtils/DesktopFile.hpp>

#include <KConfig>
#include <KConfigGroup>

DesktopFile loadDesktop(const QString& filePath) {
    DesktopFile desktopFile;
    desktopFile.path = filePath;

    if (!QFile::exists(filePath)) {
        return desktopFile;
    }

    KConfig config(filePath, KConfig::SimpleConfig);
    KConfigGroup group = config.group("Desktop Entry");

    // ----- Langugage specific fields ----- //
    QMap<QString, QString> entryMap = group.entryMap();

    for (auto it = entryMap.begin(); it != entryMap.end(); ++it) {
        if (it.key().startsWith("Name")) {
            desktopFile.desktopEntry.name[it.key().mid(4)] = it.value();
        } else if (it.key().startsWith("Comment")) {
            desktopFile.desktopEntry.comment[it.key().mid(7)] = it.value();
        } else if (it.key().startsWith("Icon")) {
            desktopFile.desktopEntry.icon[it.key().mid(4)] = it.value();
        }
    }

    // ------ Core ------ //
    desktopFile.desktopEntry.type = group.readEntry("Type", "");
    desktopFile.desktopEntry.exec = "/usr/bin/tilix"; // group.readEntry("Exec", "");
    desktopFile.desktopEntry.startupWMClass = group.readEntry("StartupWMClass", "");
    desktopFile.desktopEntry.singleMainWindow = group.readEntry("SingleMainWindow", "");

    // ----- Lists ------ //
    desktopFile.desktopEntry.categories = group.readEntry("Categories", "").split(';', Qt::SkipEmptyParts);
    desktopFile.desktopEntry.keywords = group.readEntry("Keywords", "").split(';', Qt::SkipEmptyParts);
    desktopFile.desktopEntry.mimeTypes = group.readEntry("MimeType", "").split(';', Qt::SkipEmptyParts);
    desktopFile.desktopEntry.onlyShowIn = group.readEntry("OnlyShowIn", "").split(';', Qt::SkipEmptyParts);
    desktopFile.desktopEntry.notShowIn = group.readEntry("NotShowIn", "").split(';', Qt::SkipEmptyParts);

    // ------- Booleans ------- //
    desktopFile.desktopEntry.terminal = group.readEntry("Terminal", "").toLower() == "true";
    desktopFile.desktopEntry.noDisplay = group.readEntry("NoDisplay", "false").toLower() == "true";
    desktopFile.desktopEntry.hidden = group.readEntry("Hidden", "false").toLower() == "true";
    desktopFile.desktopEntry.dBusActivatable = group.readEntry("DBusActivatable", "false").toLower() == "true";
    desktopFile.desktopEntry.prefersNonDefaultGPU = group.readEntry("PrefersNonDefaultGPU", "false").toLower() == "true";

    // ------ Actions -------- //
    QStringList actionKeys = group.readEntry("Actions", QString()).split(';', Qt::SkipEmptyParts);
    for (const QString& actionKey : actionKeys) {
        KConfigGroup actionGroup = config.group("Desktop Action " + actionKey);
        DesktopFile::DesktopAction action;
        QMap<QString, QString> actionEntryMap = actionGroup.entryMap();

        for (auto it = actionEntryMap.begin(); it != actionEntryMap.end(); ++it) {
            if (it.key().startsWith("Name")) {
                action.name[it.key().mid(4)] = it.value();
            }
        }

        action.exec = actionGroup.readEntry("Exec", "");
        action.icon = actionGroup.readEntry("Icon", "");

        desktopFile.actions.insert(actionKey, action);
    }

    // ---------------------- //

    return desktopFile;
}
