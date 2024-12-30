#include "explorer.h"

void Explorer::Init(QPlainTextEdit* editor, QListView* listView, const QString& dataTypes) {
    auto model = new QStringListModel(listView);
    listView->setModel(model);

    auto parser = new CodeParser(dataTypes);

    auto updateList = [this, parser, editor, listView, model]() {
        QList<CodeElement> elements = parser->parseCodeElements(editor->toPlainText());

        QStringList displayList;
        for (const auto& element : elements) {
            QString display;
            if (element.type == "Var" || element.type == "CVar" || element.type == "Arr") {
                display = QString("%1: %2 %3 (line %4)")
                .arg(element.type)
                    .arg(element.dataType)
                    .arg(element.name)
                    .arg(element.lineNumber);
            } else {
                display = QString("%1: %2 (line %3)")
                .arg(element.type)
                    .arg(element.name)
                    .arg(element.lineNumber);
            }
            displayList << display;
        }

        model->setStringList(displayList);
        listView->setProperty("codeElements", QVariant::fromValue(elements));
    };

    connect(editor, &QPlainTextEdit::textChanged, updateList);

    connect(listView, &QListView::clicked, [editor, listView](const QModelIndex& index) {
        auto elements = listView->property("codeElements").value<QList<CodeElement>>();
        if (index.row() < elements.size()) {
            const auto& element = elements[index.row()];
            QTextCursor cursor(editor->document());
            cursor.movePosition(QTextCursor::Start);
            cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor,
                                element.lineNumber - 1);
            editor->setTextCursor(cursor);
            editor->centerCursor();
            editor->setFocus();
        }
    });
}
