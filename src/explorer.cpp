#include "explorer.h"

#include <QStandardItemModel>

void Explorer::Init(QPlainTextEdit* editor, QListView* listView, const QString& dataTypes) {
    auto model = new QStandardItemModel(listView);
    listView->setModel(model);
    auto parser = new CodeParser(dataTypes);

    static const QHash<QString, QIcon> typeToIcon = {
        { "Func", QIcon("bin/assets/intelliSense/functions.svg") },
        { "Combo", QIcon("bin/assets/intelliSense/functions.svg") },
        { "Var", QIcon("bin/assets/intelliSense/variables.svg") },
        { "CVar", QIcon("bin/assets/intelliSense/constants.svg") },
        { "Arr", QIcon("bin/assets/intelliSense/variables.svg") }
    };

    auto updateList = [model, parser, editor, &typeToIcon]() {
        model->clear();
        QList<CodeElement> elements = parser->parseCodeElements(editor->toPlainText());

        QVector<QStandardItem*> items;
        items.reserve(elements.size());

        for (const auto& element : elements) {
            QString display;
            if (element.type == "Var" || element.type == "CVar" || element.type == "Arr") {
                display = QString("%2 %3 (line %4)")
                .arg(element.dataType)
                    .arg(element.name)
                    .arg(element.lineNumber);
            } else {
                display = QString("%1 (line %2)")
                .arg(element.name)
                    .arg(element.lineNumber);
            }

            QIcon icon = typeToIcon.value(element.type, QIcon());
            auto item = new QStandardItem(icon, display);
            item->setData(QVariant::fromValue(element), Qt::UserRole + 1);  // Store element for navigation
            items.append(item);
        }

        if (!items.isEmpty()) {
            model->appendColumn(items);
        }
    };

    connect(editor, &QPlainTextEdit::textChanged, updateList);

    connect(listView, &QListView::clicked, [editor](const QModelIndex& index) {
        auto element = index.data(Qt::UserRole + 1).value<CodeElement>();
        QTextCursor cursor(editor->document());
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor,
                            element.lineNumber - 1);
        editor->setTextCursor(cursor);
        editor->centerCursor();
        editor->setFocus();
    });

    // Initial update
    updateList();
}
