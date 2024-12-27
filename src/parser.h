#pragma once

#include <QString>
#include <QStringList>

class Parser {
private:

public:
    QString project_main;
    QString project_version;
    QString project_name;

    void parse_File(QString &path); // parses the package file to define main, name and version
    QString processMain(const QString &mainPath, QStringList& processedFiles);
};
