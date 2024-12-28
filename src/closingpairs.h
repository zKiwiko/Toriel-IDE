#pragma once

#include <QObject>
#include <QPlainTextEdit>

class AutoClosingPairs : public QObject {
    Q_OBJECT

public:
    explicit AutoClosingPairs(QObject* parent = nullptr);
    static void Setup(QPlainTextEdit* editor);

private:
    class Filter;
};
