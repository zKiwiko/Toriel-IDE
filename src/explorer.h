#pragma once

#include <QObject>
#include <QPlainTextEdit>
#include <QListView>
#include <QStringListModel>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

struct CodeElement {
    QString name;
    QString type;
    QString dataType;
    int lineNumber;
    bool isArray;
    bool isConst;
};

class Explorer : public QObject {
    Q_OBJECT
private:
    QList<CodeElement> parseCodeElements(const QString& text);
public:
    void Init(QPlainTextEdit* editor, QListView* listView, const QString& dataTypes);
};

class CodeParser {
public:
    CodeParser(const QString& dataTypes) {
        QString typePattern = dataTypes;

        patterns["simple_var"] = QRegularExpression(
            "^\\s*" + typePattern + R"(\s+(\w+)\s*(?:=\s*([^;]+))?;)",
            QRegularExpression::MultilineOption
            );

        patterns["const_var"] = QRegularExpression(
            "^\\s*const\\s+" + typePattern + R"(\s+(\w+)\s*(?:=\s*([^;]+))?;)",
            QRegularExpression::MultilineOption
            );

        patterns["toriel-cvar"] = QRegularExpression(
            R"((?:@const)\s+(\w+)(?:\s*=\s*([\w\.]+))?;)",
            QRegularExpression::MultilineOption
            );

        patterns["array_1d"] = QRegularExpression(
            "^\\s*(?:const\\s+)?" + typePattern + R"(\s+(\w+)\s*\[\s*\w*\s*\]\s*(?:=\s*\{([^\}]*)\})?;)",
            QRegularExpression::MultilineOption
            );

        patterns["array_multi"] = QRegularExpression(
            "^\\s*(?:const\\s+)?" + typePattern + R"(\s+(\w+)\s*(?:\[\s*\w*\s*\]){1,2}\s*=\s*\{[\s\S]*?\};)",
            QRegularExpression::MultilineOption
            );

        patterns["function"] = QRegularExpression(
            R"(^\s*function\s+(\w+)\s*\([^)]*\)\s*\{?)",
            QRegularExpression::MultilineOption
            );

        patterns["combo"] = QRegularExpression(
            R"(^\s*combo\s+(\w+)\s*\{)",
            QRegularExpression::MultilineOption
            );


    }

    QList<CodeElement> parseCodeElements(const QString& text) {
        QList<CodeElement> elements;

        QRegularExpressionMatchIterator multiArrayIt = patterns["array_multi"].globalMatch(text);
        QMap<int, CodeElement> multiLineElements;

        while (multiArrayIt.hasNext()) {
            QRegularExpressionMatch match = multiArrayIt.next();
            QString fullMatch = match.captured(0);
            int startPos = match.capturedStart(0);

            int lineNumber = text.left(startPos).count('\n') + 1;

            CodeElement element;
            element.type = "Arr";
            element.isArray = true;
            element.dataType = match.captured(1);
            element.name = match.captured(2);
            element.lineNumber = lineNumber;
            element.isConst = fullMatch.trimmed().startsWith("const");

            multiLineElements[lineNumber] = element;
        }

        QStringList lines = text.split('\n');
        for (int lineNum = 0; lineNum < lines.size(); ++lineNum) {
            if (multiLineElements.contains(lineNum + 1)) {
                elements.append(multiLineElements[lineNum + 1]);
                continue;
            }

            const QString& line = lines[lineNum];
            bool matched = false;

            for (auto it = patterns.begin(); it != patterns.end(); ++it) {
                if (it.key() == "array_multi") continue;

                const QRegularExpression& pattern = it.value();
                QRegularExpressionMatch match = pattern.match(line);

                if (match.hasMatch()) {
                    CodeElement element;
                    QString fullMatch = match.captured(0);

                    if (it.key() == "const_var") {
                        element.type = "CVar";
                        element.isConst = true;
                        element.dataType = match.captured(1);
                        element.name = match.captured(2);
                    }
                    else if (it.key().startsWith("array")) {
                        element.type = "Arr";
                        element.isArray = true;
                        element.dataType = match.captured(1);
                        element.name = match.captured(2);
                        element.isConst = fullMatch.trimmed().startsWith("const");
                    }
                    else if (it.key() == "simple_var") {
                        element.type = "Var";
                        element.isConst = false;
                        element.dataType = match.captured(1);
                        element.name = match.captured(2);
                    }
                    else if (it.key() == "function") {
                        element.type = "Func";
                        element.name = match.captured(1);
                    }
                    else if (it.key() == "combo") {
                        element.type = "Combo";
                        element.name = match.captured(1);
                    } else if(it.key() == "toriel-cvar") {
                        element.type = "CVar";
                        element.isConst = true;
                        element.name = match.captured(1);
                    }

                    element.lineNumber = lineNum + 1;
                    elements.append(element);
                    matched = true;
                    break;
                }
            }
        }

        return elements;
    }

private:
    QMap<QString, QRegularExpression> patterns;
};

