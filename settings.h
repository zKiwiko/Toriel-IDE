#pragma once

#include <QByteArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QString>

class Settings
{
private:
    QByteArray JSON_Data; // Raw JSON data
    QJsonObject JSON_Object; // Parsed Json Obj

    void loadJson(const QString& path);
    QJsonValue getValue(const QString& key) const;
    bool contains(const QString& key) const;
public:
    explicit Settings(const QString& path);


};
