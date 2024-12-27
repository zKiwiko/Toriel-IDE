#pragma once

#include <QString>
#include <QStringList>

class Parser {
private:

public:
    QString pr_src;
    QString pr_ver;
    QString pr_name;

    void parse_File(const QString &path); // parses the package file to define main, name and version
    QString processMain(const QString &mainPath, QStringList& processedFiles);
};
