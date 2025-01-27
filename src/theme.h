#pragma once
#include <QSyntaxHighlighter>
#include <QPlainTextEdit>
#include <QStringList>
#include <QTextCharFormat>
#include <QRegularExpression>

class CodeHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    explicit CodeHighlighter(QTextDocument *parent = nullptr);
    void ReloadThemeData();
    void RetrieveThemeData();
    void SetSyntaxFormat();

    QString GPC_Constants;
    QString GPC_Functions;
    QString GPC_Keywords;
    QString GPC_Datatypes;
    QString editorColor;
    QString menuBarColor;
    QString statusBarColor;
    QString terminalColor;
    QString backgroundColor;
    QString explorerColor;
    QString editorFont;
    QString explorerFont;
    QString textColor;
    QString lineColor;

protected:
    void highlightBlock(const QString& text) override;

private:
    struct HighlightRule {
        QRegularExpression pattern;
        QTextCharFormat* format;
    };

    static const QRegularExpression singleLineCommentPattern;
    static const QRegularExpression multiLineCommentStartPattern;
    static const QRegularExpression multiLineCommentEndPattern;

    const QString GPC_Boolean = { "(TRUE|FALSE)" };
    const QString functionNamePattern = R"(\b([A-Za-z_]\w*)(?=\())";
    const QString numberPatterns = { "\\b(\\d+(\\.\\d+)?|0x[0-9a-fA-F]+)\\b" };
    const QString stringsPatterns = { R"(".*?"|(?<=@include)\s+<([^>]+)>)" };

    QTextCharFormat keywordFormat;
    QTextCharFormat booleanFormat;
    QTextCharFormat builtinFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat constantsFormat;
    QTextCharFormat functionNameFormat;
    QTextCharFormat numbersFormat;
    QTextCharFormat stringsFormat;
    QTextCharFormat datatypesFormat;

    QString numbersColor;
    QString keywordsColor;
    QString builtinFunctionsColor;
    QString datatypesColor;
    QString udfunctionsColor;
    QString booleanColor;
    QString stringsColor;
    QString commentColor;
    QString constantsColor;

    QString constantsFont;
    QString numbersFont;
    QString keywordsFont;
    QString builtinFunctionsFont;
    QString datatypesFont;
    QString udfunctionsFont;
    QString booleanFont;
    QString stringsFont;
    QString commentsFont;

    QVector<HighlightRule> highlightRules;

    void RetrieveGPCData();
    void initializeHighlightRules();
    void handleComments(const QString& text);
    void addHighlightRule(const QString& pattern, QTextCharFormat* format);
};
