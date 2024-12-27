#pragma once

#include <QString>
#include <QStringList>

class Parser {
    // Not really a parser, but i couldnt think of another name for this class
private:

public:
    static QString processMain(const QString &mainPath, QStringList& processedFiles);
};
