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

void Parser::parse_File(QString &path) {
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

        if (line.startsWith("// include")) {
            QStringList tokens = line.split(' ', Qt::SkipEmptyParts);
            if (tokens.size() < 2) {
                throw std::runtime_error(
                    QString("Malformed include directive: %1").arg(line).toStdString());
            }
            QString includeFileName = tokens[1].trimmed();

            QString includeFilePath = baseDir + "/" + includeFileName;
            QString includeContent = processMain(includeFilePath, processedFiles);
            result += includeContent + "\n";
        } else {
            result += line + "\n";
        }
    }

    return result;
}
