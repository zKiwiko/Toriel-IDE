#include "parser.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QException>
#include <QMessageBox>

#include <QJsonArray>
#include <QJsonObject>
#include <QByteArray>
#include <QJsonDocument>

void Parser::parse_File(const QString &path) {
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, "Error", "Unable to open project.json file.");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        QMessageBox::critical(nullptr, "Error", "Failed to parse JSON.");
        return;
    }

    QJsonObject obj = doc.object();

    if (obj.contains("project") && obj["project"].isObject()) {
        QJsonObject projectObj = obj["project"].toObject();

        if (projectObj.contains("name") && projectObj["name"].isString()) {
            pr_src = projectObj["name"].toString();
        }

        if (projectObj.contains("version") && projectObj["version"].isString()) {
            pr_ver = projectObj["version"].toString();
        }

        if (projectObj.contains("source") && projectObj["source"].isString()) {
            pr_src = projectObj["source"].toString();
        }
    }
}

QString Parser::processMain(const QString &mainPath, QStringList& processedFiles) {
    if (processedFiles.contains(mainPath)) {
        QMessageBox::critical(nullptr, "Error", "Circular import detected for file: " + mainPath);
    }
    processedFiles.append(mainPath);

    QFile inputFile(mainPath);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error(
            QString("Unable to open file: %1").arg(mainPath).toStdString());
    }

    QTextStream inStream(&inputFile);
    QString result;
    QString baseDir = QFileInfo(mainPath).absolutePath();

    while (!inStream.atEnd()) {
        QString line = inStream.readLine();
        if (line.trimmed().startsWith("// include")) {
            QString includeFileName = line.mid(line.indexOf("include") + 7).trimmed();

            if (includeFileName.isEmpty()) {
                throw std::runtime_error("Empty include filename");
            }

            QString includeFilePath;
            if (QFileInfo(includeFileName).isAbsolute()) {
                includeFilePath = includeFileName;
            } else {
                includeFilePath = baseDir + QDir::separator() + includeFileName;
            }

            if (!QFile(includeFilePath).exists()) {
                throw std::runtime_error(
                    QString("Include file not found: %1").arg(includeFilePath).toStdString());
            }

            QString includeContent = processMain(includeFilePath, processedFiles);
            result += includeContent + "\n";
        } else {
            result += line + "\n";
        }
    }
    return result;
}

