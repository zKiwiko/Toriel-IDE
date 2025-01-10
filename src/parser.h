#pragma once

#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <QMap>

class Parser {
private:
    static const QRegularExpression include_pattern_system;
    static const QRegularExpression include_pattern_project;
    static const QRegularExpression const_pattern;
    QString processContent(const QString& content, const QString& baseDir, QStringList& processedFiles);
    QString resolveIncludePath(const QString& includePath, const QString& baseDir, bool isSystemInclude);
    QMap<QString, QString> constants;
    QString systemIncludePath;

public:
    Parser();

    QString pr_src;
    QString pr_ver;
    QString pr_name;

    void parse_File(const QString &path); // parses the package file to define main, name and version
    QString processMain(const QString &mainPath, QStringList& processedFiles);
};
