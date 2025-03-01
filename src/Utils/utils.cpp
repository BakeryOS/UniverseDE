#include <Utils/utils.hpp>

QString getStyle(const QString& name) {
    QFile file(":/" + name);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString content = stream.readAll();
        file.close();
        return content;
    }
    return (QString) "";
}
