#include "autocomplete.h"

#include <QCompleter>
#include <QStringListModel>
#include <QObject>
#include <QAbstractItemView>
#include <QRegularExpression>
#include <QTextCursor>
#include <QRect>
#include <QScrollBar>
#include <QEvent>

QString AutoComplete::getWordUnderCursor(QPlainTextEdit *editor) {
    QTextCursor tc = editor->textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

QRect AutoComplete::getCursorRect(QPlainTextEdit *editor) {
    QTextCursor tc = editor->textCursor();
    QRect cr = editor->cursorRect(tc);
    cr.setWidth(editor->viewport()->width() - cr.x());
    return cr;
}

void AutoComplete::SetupWords(QString kw, QString bif, QString dt, QString c) {
    auto parseWords = [](const QString& input) -> QStringList {
        QRegularExpression regex(R"(\\b\(([^)]+)\)\\b)");
        QRegularExpressionMatch match = regex.match(input);
        if (match.hasMatch()) {
            QString captured = match.captured(1);
            return captured.split('|');
        }
        return {};
    };

    a_keywords.append(parseWords(kw));
    a_builtinFunctions.append(parseWords(bif));
    a_dataTypes.append(parseWords(dt));
    a_constants.append(parseWords(c));
}

void AutoComplete::Setup(QPlainTextEdit *editor) {
    completer = new QCompleter(editor);
    completer->setWidget(editor);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    QStringListModel* model = new QStringListModel((a_keywords + a_dataTypes + a_builtinFunctions + a_constants), completer);
    completer->setModel(model);

    editor->installEventFilter(this);

    QObject::connect(editor, &QPlainTextEdit::textChanged, [this, editor]() {
        QString word = getWordUnderCursor(editor);

        if (word.length() < 2) {
            completer->popup()->hide();
            return;
        }

        if (completer->completionPrefix() != word) {
            completer->setCompletionPrefix(word);
            completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));
        }

        QRect cr = getCursorRect(editor);
        cr.setWidth(completer->popup()->sizeHintForColumn(0) +
                    completer->popup()->verticalScrollBar()->sizeHint().width());
        completer->complete(cr);
    });

    QObject::connect(completer, QOverload<const QString&>::of(&QCompleter::activated),
                     [editor](const QString& completion) {
                         QTextCursor tc = editor->textCursor();
                         tc.select(QTextCursor::WordUnderCursor);
                         tc.removeSelectedText();
                         tc.insertText(completion);
                     });
}

void AutoComplete::Style(QString Color, QString line) {
    completer->popup()->setStyleSheet(QString(
    "QListView {"
    "   background-color: %1;"
    "   color: white;"
    "   border: 1px solid %2; border-radius: 5px; outline: 0;"
    "}"
    "QListView::item {"
    "   border: none; margin: 5px;"
    "}"
    "QListView::item:selected {"
    "   background-color: %2;"
    "   color: white;"
    "   border: none;"
    "}"
    ).arg(Color).arg(line));
}
