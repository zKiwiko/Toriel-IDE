#pragma once

#include <QString>
#include <QStringList>
#include <QPlainTextEdit>
#include <QCompleter>
#include <QObject>
#include <QList>
#include <QRegularExpression>
#include <QStandardItemModel>

class AutoComplete : public QObject {
    Q_OBJECT

public:
    QSet<QString> addedItems;
    void SetupWords(QString kw, QString bif, QString dt, QString c);
    void Init(QPlainTextEdit *editor);
    void Style(QString Color, QString line);
    void resyncHeaders(const QString& dir, const QStringList& files);
    void resyncStd(QString dir, QStringList files);
    QString headerContent = "";

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
