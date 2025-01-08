#include "autocomplete.h"

#include <QAbstractItemView>
#include <QRegularExpression>
#include <QTextCursor>
#include <QRect>
#include <QScrollBar>
#include <QEvent>
#include <QElapsedTimer>
#include <QRegularExpressionMatch>

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

// optimized the fuck out of this and am proud
void AutoComplete::updateCompletionList(const QString& text) {
    completionModel->setRowCount(0);

    QSet<QString> addedItems;
    addedItems.reserve(100);

    QVector<QStandardItem*> items;
    items.reserve(100);

    static const QHash<QString, QIcon> typeToIcon = {
        { "keyword", QIcon("bin/assets/intelliSense/keywords.svg") },
        { "function", QIcon("bin/assets/intelliSense/functions.svg") },
        { "variable", QIcon("bin/assets/intelliSense/variables.svg") },
        { "constant", QIcon("bin/assets/intelliSense/constants.svg") }
    };

    auto addItemWithIcon = [&items, &addedItems, &typeToIcon](const QString& text, const QString& type) {
        if (!addedItems.contains(text)) {
            QIcon icon = typeToIcon.value(type, QIcon());
            items.append(new QStandardItem(icon, text));
            addedItems.insert(text);
        }
    };

    for (const QString& keyword : a_keywords) addItemWithIcon(keyword, "keyword");
    for (const QString& function : a_builtinFunctions) addItemWithIcon(function, "function");
    for (const QString& variable : a_dataTypes) addItemWithIcon(variable, "variable");
    for (const QString& constant : a_constants) addItemWithIcon(constant, "constant");

    for (const QRegularExpression& regex : patterns) {
        QRegularExpressionMatchIterator iterator = regex.globalMatch(text);
        while (iterator.hasNext()) {
            QRegularExpressionMatch match = iterator.next();
            QString captured = regex.pattern().contains("function") || regex.pattern().contains("combo") ? match.captured(1) : match.captured(2);
            QString type = regex.pattern().contains("function") || regex.pattern().contains("combo") ? "function" : "variable";
            addItemWithIcon(captured, type);
        }
    }

    if (!items.isEmpty()) {
        completionModel->insertColumn(0, items);
    }
}

void AutoComplete::SetupWords(QString kw, QString bif, QString dt, QString c) {
    auto parseWords = [](const QString& input) -> QStringList {
        if (input.isEmpty()) {
            return {};
        }
        QString cleaned = input;
        cleaned.remove(QRegularExpression(R"(\\b)"));
        cleaned.remove(QRegularExpression(R"([\(\)])"));
        return cleaned.split('|', Qt::SkipEmptyParts);
    };

    patterns.append(QRegularExpression(
        R"(\b)" + dt + R"(\s+(\w+)\s*(?:=\s*[^;]+)?;)",
        QRegularExpression::MultilineOption
        ));

    patterns.append(QRegularExpression(
        "\\b(?:const\\s+)?" + dt + R"(\s+(\w+)\s*(?:=\s*[^;]+)?;)",
        QRegularExpression::MultilineOption
        ));

    patterns.append(QRegularExpression(
        "\\b(?:const\\s+)?" + dt + R"(\s+(\w+)\s*\[\s*\w*\s*\]\s*(?:=\s*\{[^}]*\})?;)",
        QRegularExpression::MultilineOption
        ));

    patterns.append(QRegularExpression(
        "\\b(?:const\\s+)?" + dt + R"(\s+(\w+)\s*(?:\[\s*\w*\s*\]){1,2}\s*=\s*\{[^}]*\};)",
        QRegularExpression::MultilineOption
        ));

    patterns.append(QRegularExpression(
        R"(\bfunction\s+(\w+)\s*\([^)]*\)\s*\{?)",
        QRegularExpression::MultilineOption
        ));

    patterns.append(QRegularExpression(
        R"(\bcombo\s+(\w+)\s*\{)",
        QRegularExpression::MultilineOption
        ));

    a_keywords = parseWords(kw);
    a_builtinFunctions = parseWords(bif);
    a_dataTypes = parseWords(dt);
    a_constants = parseWords(c);
}

void AutoComplete::Init(QPlainTextEdit *editor) {
    completer = new QCompleter(editor);
    completer->setWidget(editor);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    completionModel = new QStandardItemModel(completer);
    completer->setModel(completionModel);

    editor->installEventFilter(this);

    QObject::connect(editor, &QPlainTextEdit::textChanged, [this, editor]() {
        QString word = getWordUnderCursor(editor);
        if (word.length() < 2) {
            completer->popup()->hide();
            return;
        }

        updateCompletionList(editor->toPlainText());

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
