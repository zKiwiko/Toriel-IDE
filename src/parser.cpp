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

    qDebug() << "Raw JSON data: " << data;

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        QMessageBox::critical(nullptr, "Error", "Failed to parse JSON.");
        return;
    }

    QJsonObject obj = doc.object();
    qDebug() << "Parsed JSON object:" << obj;

    if (obj.contains("project") && obj["project"].isObject()) {
        QJsonObject projectObj = obj["project"].toObject();
        qDebug() << "Parsed 'project' object:" << projectObj;

        if (projectObj.contains("name") && projectObj["name"].isString()) {
            pr_src = projectObj["name"].toString();
            qDebug() << "Project Name: " << pr_name;
        } else {
            qDebug() << "Key 'name' is missing or not a string.";
        }

        if (projectObj.contains("version") && projectObj["version"].isString()) {
            pr_ver = projectObj["version"].toString();
            qDebug() << "Project Version: " << pr_ver;
        } else {
            qDebug() << "Key 'version' is missing or not a string.";
        }

        if (projectObj.contains("source") && projectObj["source"].isString()) {
            pr_src = projectObj["source"].toString();
            qDebug() << "Project Source: " << pr_src;
        } else {
            qDebug() << "Key 'source' is missing or not a string.";
        }
    } else {
        qDebug() << "Key 'project' is missing or not an object.";
    }
}

QString Parser::processMain(const QString &mainPath, QStringList& processedFiles) {
    if (processedFiles.contains(mainPath)) {
        QMessageBox::critical(nullptr, "Error", "Circular import detected for file: " + mainPath);
    }
    processedFiles.append(mainPath);

    qDebug() << "Processing file:" << mainPath;
    qDebug() << "Current processed files:" << processedFiles;

    QFile inputFile(mainPath);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << mainPath;
        throw std::runtime_error(
            QString("Unable to open file: %1").arg(mainPath).toStdString());
    }

    QTextStream inStream(&inputFile);
    QString result;
    QString baseDir = QFileInfo(mainPath).absolutePath();
    qDebug() << "Base directory:" << baseDir;

    while (!inStream.atEnd()) {
        QString line = inStream.readLine();
        // Changed the parsing to remove the "// include " prefix completely
        if (line.trimmed().startsWith("// include")) {
            // Remove the "// include " prefix and trim any whitespace
            QString includeFileName = line.mid(line.indexOf("include") + 7).trimmed();
            qDebug() << "Include filename from directive:" << includeFileName;

            if (includeFileName.isEmpty()) {
                throw std::runtime_error("Empty include filename");
            }

            QString includeFilePath;
            if (QFileInfo(includeFileName).isAbsolute()) {
                includeFilePath = includeFileName;
            } else {
                includeFilePath = baseDir + QDir::separator() + includeFileName;
            }

            qDebug() << "Attempting to open file at:" << includeFilePath;
            qDebug() << "File exists check:" << QFile(includeFilePath).exists();

            if (!QFile(includeFilePath).exists()) {
                qDebug() << "File not found at path:" << includeFilePath;
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

