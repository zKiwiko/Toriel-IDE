#pragma once

#include <QString>

class Package {
public:
    QString project_main;
    QString project_version;
    QString project_name;

    void parse_File(QString &path);
};
