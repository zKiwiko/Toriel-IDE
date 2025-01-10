#include "parser.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QByteArray>
#include <QJsonDocument>
#include <QCoreApplication>

Parser::Parser() {
    QString appPath = QCoreApplication::applicationDirPath();
    systemIncludePath = QDir(appPath).filePath("bin/data/gpc/libs");
}

const QRegularExpression Parser::include_pattern_system(R"(@include\s*<([^>]+)>)");
const QRegularExpression Parser::include_pattern_project(R"(@include\s*\"([^"]+)\")");
const QRegularExpression Parser::const_pattern(R"(@const\s+(\w+)\s*=?\s*([\w\.]+)?;?)");

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
            pr_name = projectObj["name"].toString();
        }
        if (projectObj.contains("version") && projectObj["version"].isString()) {
            pr_ver = projectObj["version"].toString();
        }
        if (projectObj.contains("source") && projectObj["source"].isString()) {
            pr_src = projectObj["source"].toString();
        }
    }
}

QString Parser::resolveIncludePath(const QString& includePath, const QString& baseDir, bool isSystemInclude) {
    if (isSystemInclude) {
        QString fileName = includePath;
        if (!fileName.endsWith(".gpc")) {
            fileName += ".gpc";
        }
        return QDir(systemIncludePath).filePath(fileName);
    } else {
        if (QFileInfo(includePath).isAbsolute()) {
            return includePath;
        }
        return QDir(baseDir).filePath(includePath);
    }
}

QString Parser::processMain(const QString &mainPath, QStringList& processedFiles) {
    if (processedFiles.contains(mainPath)) {
        QMessageBox::critical(nullptr, "Error", "Circular import detected for file: " + mainPath);
        throw std::runtime_error("Circular import detected");
    }
    processedFiles.append(mainPath);
    QFile inputFile(mainPath);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error(
            QString("Unable to open file: %1").arg(mainPath).toStdString());
    }
    QString content = QTextStream(&inputFile).readAll();
    inputFile.close();
    return processContent(content, QFileInfo(mainPath).absolutePath(), processedFiles);
}

QString Parser::processContent(const QString& content, const QString& baseDir, QStringList& processedFiles) {
    QRegularExpressionMatchIterator constMatches = const_pattern.globalMatch(content);
    while (constMatches.hasNext()) {
        QRegularExpressionMatch match = constMatches.next();
        constants[match.captured(1)] = match.captured(2).trimmed();
    }

    QString processedContent = content;
    processedContent.replace(const_pattern, "");
    QStringList lines = processedContent.split('\n');
    QString result;

    for (const QString& line : lines) {
        QString processedLine = line;

        QRegularExpressionMatch systemIncludeMatch = include_pattern_system.match(line);
        if (systemIncludeMatch.hasMatch()) {
            QString includeFileName = systemIncludeMatch.captured(1).trimmed();
            if (includeFileName.isEmpty()) {
                throw std::runtime_error("Empty include filename");
            }

            QString includeFilePath = resolveIncludePath(includeFileName, baseDir, true);
            if (!QFile(includeFilePath).exists()) {
                throw std::runtime_error(
                    QString("System include file not found: %1").arg(includeFilePath).toStdString());
            }
            processedLine = processMain(includeFilePath, processedFiles);
        } else {
            QRegularExpressionMatch projectIncludeMatch = include_pattern_project.match(line);
            if (projectIncludeMatch.hasMatch()) {
                QString includeFileName = projectIncludeMatch.captured(1).trimmed();
                if (includeFileName.isEmpty()) {
                    throw std::runtime_error("Empty include filename");
                }

                QString includeFilePath = resolveIncludePath(includeFileName, baseDir, false);
                if (!QFile(includeFilePath).exists()) {
                    throw std::runtime_error(
                        QString("Project include file not found: %1").arg(includeFilePath).toStdString());
                }
                processedLine = processMain(includeFilePath, processedFiles);
            }
        }

        for (auto it = constants.constBegin(); it != constants.constEnd(); ++it) {
            QRegularExpression refPattern(QString(R"(\b%1\b)").arg(it.key()));
            processedLine.replace(refPattern, it.value());
        }

        result += processedLine + "\n";
    }

    return result;
}
