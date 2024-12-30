#include "closingpairs.h"
#include <QKeyEvent>

class AutoClosingPairs::Filter : public QObject {
public:
    Filter(QPlainTextEdit* parent) : QObject(parent), editor(parent) {}
protected:
    bool eventFilter(QObject* obj, QEvent* event) override {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            QString key = keyEvent->text();
            if (keyEvent->key() == Qt::Key_Backspace) {
                QTextCursor cursor = editor->textCursor();
                if (!cursor.hasSelection()) {
                    int pos = cursor.position();
                    if (pos > 0) {
                        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
                        QString charBeforeCursor = cursor.selectedText();
                        if (closingPairs.contains(charBeforeCursor)) {
                            cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 2);
                            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
                            QString nextChar = cursor.selectedText();
                            if (nextChar == closingPairs[charBeforeCursor]) {
                                cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor);
                                cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 2);
                                cursor.removeSelectedText();
                                return true;
                            }
                        }
                    }
                }
            }
            else {
                QTextCursor cursor = editor->textCursor();
                bool isClosingPair = false;
                QString matchingOpenChar;
                for (auto it = closingPairs.begin(); it != closingPairs.end(); ++it) {
                    if (it.value() == key) {
                        isClosingPair = true;
                        matchingOpenChar = it.key();
                        break;
                    }
                }

                if (isClosingPair) {
                    if (cursor.position() < editor->toPlainText().length()) {
                        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
                        QString nextChar = cursor.selectedText();
                        if (nextChar == key) {
                            cursor.clearSelection();
                            editor->setTextCursor(cursor);
                            return true;
                        }
                        cursor.movePosition(QTextCursor::Left);
                    }
                }

                if (closingPairs.contains(key)) {
                    cursor.insertText(key + closingPairs[key]);
                    cursor.movePosition(QTextCursor::Left);
                    editor->setTextCursor(cursor);
                    return true;
                }
            }
        }
        return QObject::eventFilter(obj, event);
    }
private:
    QPlainTextEdit* editor;
    static const inline QMap<QString, QString> closingPairs = {
        {"(", ")"},
        {"[", "]"},
        {"{", "}"},
        {"\"", "\""},
        {"'", "'"}
    };
};

AutoClosingPairs::AutoClosingPairs(QObject* parent) : QObject(parent) {}

void AutoClosingPairs::Setup(QPlainTextEdit* editor) {
    editor->installEventFilter(new Filter(editor));
}
