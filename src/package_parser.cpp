#include "package_parser.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QByteArray>
#include <QJsonDocument>
#include <QmessageBox>

void Package::parse_File(QString &path) {
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = file.readAll();

    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    if(doc.isNull() || !doc.isObject()) {
        return;
    }

    if(!obj.contains("main")) {
        QMessageBox::critical(nullptr, "Error", "Your 'project.json' file does not contain the main file definition.");
        return;
    }
    if(!obj.contains("title")) {
        QMessageBox::critical(nullptr, "Error", "Your 'project.json' file does not contain the projects name definition.");
        return;
    }
    if(!obj.contains("version")) {
        QMessageBox::critical(nullptr, "Error", "Your 'project.json' file does not contains the version definition.");
        return;
    }

    project_main = obj["main"].toString();
    project_version = obj["version"].toString();
    project_name = obj["name"].toString();

}
