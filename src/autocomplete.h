#pragma once

#include <QString>
#include <QStringList>
#include <QPlainTextEdit>
#include <QCompleter>
#include <QObject>
#include <QRegularExpression>

class AutoComplete : public QObject {
    Q_OBJECT

private:
    QString getWordUnderCursor(QPlainTextEdit* editor);
    QRect getCursorRect(QPlainTextEdit* editor);

    QStringList a_keywords;
    QStringList a_builtinFunctions;

    QStringList a_dataTypes;
    QStringList a_constants;

    QCompleter* completer;

public:
    void SetupWords(QString kw, QString bif, QString dt, QString c);
    void Setup(QPlainTextEdit* editor);
    void Style(QString Color, QString line);

};
