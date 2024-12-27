#include "package_parser.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QByteArray>
#include <QJsonDocument>
#include <QmessageBox>

void Package::parse_File(QString &path) {
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, "Error", "Unable to open project.json file.");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        QMessageBox::critical(nullptr, "Error", "Invalid JSON format in project.json.");
        return;
    }

    QJsonObject obj = doc.object();

    if (!obj.contains("main") || !obj.contains("name") || !obj.contains("version")) {
        QMessageBox::critical(nullptr, "Error", "Missing required fields in project.json.");
        return;
    }

    project_main = obj["main"].toString();
    project_version = obj["version"].toString();
    project_name = obj["name"].toString();

}
