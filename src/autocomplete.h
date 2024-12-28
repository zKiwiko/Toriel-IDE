#pragma once

#include <QString>
#include <QStringList>
#include <QPlainTextEdit>
#include <QCompleter>
#include <QObject>

class AutoComplete : public QObject {
    Q_OBJECT
private:
    QString getWordUnderCursor(QPlainTextEdit* editor);
    QRect getCursorRect(QPlainTextEdit* editor);

    QStringList keywords;
    QStringList builtinFunctions;
    QStringList userFunctions;
    QStringList dataTypes;
    QStringList constants;

    QCompleter* completer;

public:
    AutoComplete();
    void SetupWords(QString kw, QString bif, QString dt, QString c);
    void Setup(QPlainTextEdit* editor);
    void Style(QString Color, QString line);

};
