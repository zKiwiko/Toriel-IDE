#pragma once

#include <QString>
#include <QStringList>
#include <QPlainTextEdit>
#include <QCompleter>
#include <QObject>
#include <QRegularExpression>
#include <QList>
#include <QRegularExpression>
#include <QStandardItemModel>
#include <QIcon>

class AutoComplete : public QObject {
    Q_OBJECT

public:
    //explicit AutoComplete(QObject *parent = nullptr);
    QSet<QString> addedItems;
    void SetupWords(QString kw, QString bif, QString dt, QString c);
    void Init(QPlainTextEdit *editor);
    void Style(QString Color, QString line);

private:
    QString getWordUnderCursor(QPlainTextEdit *editor);
    QRect getCursorRect(QPlainTextEdit *editor);
    void updateCompletionList(const QString& text);
    QList<QRegularExpression> patterns;

    QCompleter *completer;
    QStringList a_keywords;
    QStringList a_builtinFunctions;
    QStringList a_dataTypes;
    QStringList a_constants;
    QStandardItemModel *completionModel;
};
