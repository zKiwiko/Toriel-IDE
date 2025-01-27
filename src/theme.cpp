#include "theme.h"

#include <QRegularExpressionMatchIterator>
#include <QColor>
#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>

CodeHighlighter::CodeHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    RetrieveGPCData();
    RetrieveThemeData();
    SetSyntaxFormat();
    initializeHighlightRules();
}

void CodeHighlighter::highlightBlock(const QString &text) {
    if (text.isEmpty() || text.trimmed().isEmpty()) {
        return;
    }

    QTextBlock block;
    if(!block.isVisible()) {
        return;
    }

    for (const HighlightRule &rule : highlightRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), *rule.format);
        }
    }

    handleComments(text);
}

void CodeHighlighter::initializeHighlightRules() {
    highlightRules.reserve(9);

    addHighlightRule(functionNamePattern, &functionNameFormat);
    addHighlightRule(GPC_Keywords, &keywordFormat);
    addHighlightRule(GPC_Functions, &builtinFormat);
    addHighlightRule(GPC_Constants, &constantsFormat);
    addHighlightRule(GPC_Datatypes, &datatypesFormat);
    addHighlightRule(GPC_Boolean, &booleanFormat);
    addHighlightRule(numberPatterns, &numbersFormat);
    addHighlightRule(stringsPatterns, &stringsFormat);
}

void CodeHighlighter::addHighlightRule(const QString& pattern, QTextCharFormat* format) {
    HighlightRule rule;
    rule.pattern = QRegularExpression(pattern);
    rule.format = format;
    highlightRules.append(rule);
}

void CodeHighlighter::ReloadThemeData() {
    QString selectedFile = QFileDialog::getOpenFileName(nullptr, "Select a JSON File", "", "*.json");

    if (selectedFile.isEmpty()) {
        return;
    }

    QString fileName = QFileInfo(selectedFile).fileName();

    QFile settingsFile("bin/data/settings/settings.json");
    if (!settingsFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }

    QByteArray settingsData = settingsFile.readAll();
    QJsonDocument settingsDoc = QJsonDocument::fromJson(settingsData);
    QJsonObject settingsObj = settingsDoc.object();

    settingsObj["theme"] = fileName;

    settingsFile.resize(0);
    settingsFile.write(QJsonDocument(settingsObj).toJson());
    settingsFile.close();

    RetrieveThemeData();
}

void CodeHighlighter::SetSyntaxFormat() {
    keywordFormat.setForeground(QColor(keywordsColor));
    booleanFormat.setForeground(QColor(booleanColor));
    builtinFormat.setForeground(QColor(builtinFunctionsColor));
    commentFormat.setForeground(QColor(commentColor));
    constantsFormat.setForeground(QColor(constantsColor));
    functionNameFormat.setForeground(QColor(udfunctionsColor));
    numbersFormat.setForeground(QColor(numbersColor));
    stringsFormat.setForeground(QColor(stringsColor));
    datatypesFormat.setForeground(QColor(datatypesColor));


    if(keywordsFont != "default") {
        QFont font = keywordFormat.font();
        if(keywordsFont == "bold") {
            font.setBold(true);
        } else if(keywordsFont == "italic") {
            font.setItalic(true);
        }
        font.setFamily(editorFont);
        keywordFormat.setFont(font);
    }

    if(booleanFont != "default") {
        QFont font = booleanFormat.font();
        if(booleanFont == "bold") {
            font.setBold(true);
        } else if(booleanFont == "italic") {
            font.setItalic(true);
        }
        font.setFamily(editorFont);
        booleanFormat.setFont(font);
    }

    if(builtinFunctionsFont != "default") {
        QFont font = builtinFormat.font();
        if(builtinFunctionsFont == "bold") {
            font.setBold(true);
        } else if(builtinFunctionsFont == "italic") {
            font.setItalic(true);
        }
        font.setFamily(editorFont);
        builtinFormat.setFont(font);
    }

    if (commentsFont != "default") {
        QFont font = commentFormat.font();
        if (commentsFont == "bold") {
            font.setBold(true);
        } else if (commentsFont == "italic") {
            font.setItalic(true);
        }
        font.setFamily(editorFont);
        commentFormat.setFont(font);
    }

    if(constantsFont != "default") {
        QFont font = constantsFormat.font();
        if(constantsFont == "bold") {
            font.setBold(true);
        } else if(constantsFont == "italic") {
            font.setItalic(true);
        }
        font.setFamily(editorFont);
        constantsFormat.setFont(font);
    }

    if(udfunctionsFont != "default") {
        QFont font = functionNameFormat.font();
        if(udfunctionsFont == "bold") {
            font.setBold(true);
        } else if(udfunctionsFont == "italic") {
            font.setItalic(true);
        }
        font.setFamily(editorFont);
        functionNameFormat.setFont(font);
    }

    if(numbersFont != "default") {
        QFont font = numbersFormat.font();
        if(numbersFont == "bold") {
            font.setBold(true);
        } else if(numbersFont == "italic") {
            font.setItalic(true);
        }
        font.setFamily(editorFont);
        numbersFormat.setFont(font);
    }

    if(stringsFont != "default") {
        QFont font = stringsFormat.font();
        if(stringsFont == "bold") {
            font.setBold(true);
        } else if(stringsFont == "italic") {
            font.setItalic(true);
        }
        font.setFamily(editorFont);
        stringsFormat.setFont(font);
    }

    if(datatypesFont != "default") {
        QFont font = datatypesFormat.font();
        if(datatypesFont == "bold") {
            font.setBold(true);
        } else if(datatypesFont == "italic") {
            font.setItalic(true);
        }
        font.setFamily(editorFont);
        datatypesFormat.setFont(font);
    }

    initializeHighlightRules();
}

void CodeHighlighter::RetrieveGPCData() {
    QStringList constants;
    QStringList functions;
    QStringList keywords;
    QStringList datatypes;

    QString constantsPath = "bin/data/gpc/constants.json";
    QString functionsPath = "bin/data/gpc/functions.json";
    QString keywordsPath = "bin/data/gpc/keywords.json";
    QString datatypesPath = "bin/data/gpc/datatypes.json";

    QFile constantsFile(constantsPath);
    QFile functionsFile(functionsPath);
    QFile keywordsFile(keywordsPath);
    QFile datatypesFile(datatypesPath);

    if(!constantsFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Couldnt open constants.json.";
        return;
    }
    if(!functionsFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Couldnt open functions.json.";
        return;
    }
    if(!keywordsFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Couldnt open keywords.json.";
        return;
    }
    if(!datatypesFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Couldnt open datatypes.json.";
        return;
    }

    QByteArray constantsData = constantsFile.readAll();
    QByteArray functionsData = functionsFile.readAll();
    QByteArray keywordsData = keywordsFile.readAll();
    QByteArray datatypesData = datatypesFile.readAll();

    constantsFile.close();
    functionsFile.close();
    keywordsFile.close();
    datatypesFile.close();

    QJsonDocument constantsDoc = QJsonDocument::fromJson(constantsData);
    QJsonDocument functionsDoc = QJsonDocument::fromJson(functionsData);
    QJsonDocument keywordsDoc = QJsonDocument::fromJson(keywordsData);
    QJsonDocument datatypesDoc = QJsonDocument::fromJson(datatypesData);


    if (constantsDoc.isNull() || !constantsDoc.isObject()) {
        qDebug() << "Invalid Constants structure.";
        return;
    }
    if (functionsDoc.isNull() || !functionsDoc.isObject()) {
        qDebug() << "Invalid Functions structure.";
        return;
    }
    if (keywordsDoc.isNull() || !keywordsDoc.isObject()) {
        qDebug() << "Invalid Keywords structure.";
        return;
    }
    if (datatypesDoc.isNull() || !datatypesDoc.isObject()) {
        qDebug() << "Invalid datatypes structure.";
        return;
    }

    QJsonObject constantsObj = constantsDoc.object();
    QJsonObject functionsObj = functionsDoc.object();
    QJsonObject keywordsObj = keywordsDoc.object();
    QJsonObject datatypesObj = datatypesDoc.object();

    QJsonArray constantsArr = constantsObj.value("constants").toArray();
    QJsonArray functionsArr = functionsObj.value("functions").toArray();
    QJsonArray keywordsArr = keywordsObj.value("keywords").toArray();
    QJsonArray datatypesArr = datatypesObj.value("datatypes").toArray();

    for(const QJsonValue& value : constantsArr) {
        if(value.isString()) {
            constants.append(value.toString());
        }
    }
    for(const QJsonValue& value : functionsArr) {
        if(value.isString()) {
            functions.append(value.toString());
        }
    }
    for(const QJsonValue& value : keywordsArr) {
        if(value.isString()) {
            keywords.append(value.toString());
        }
    }
    for(const QJsonValue& value : datatypesArr) {
        if(value.isString()) {
            datatypes.append(value.toString());
        }
    }

    GPC_Keywords = ("\\b(" + keywords.join("|") + ")\\b");
    GPC_Functions = ("\\b(" + functions.join("|") + ")\\b");
    GPC_Constants = ("\\b(" + constants.join("|") + ")\\b");
    GPC_Datatypes = ("\\b(" + datatypes.join("|") + ")\\b");
}

void CodeHighlighter::RetrieveThemeData() {
    QString settingsPath = "bin/data/settings/settings.json";
    QString themePath = "";
    QFile settingsFile(settingsPath);

    if(!settingsFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Couldn't open settings.json";
        return;
    }

    QByteArray settingsData = settingsFile.readAll();
    settingsFile.close();

    QJsonDocument settingsDoc = QJsonDocument::fromJson(settingsData);

    if (settingsDoc.isNull() || !settingsDoc.isObject()) {
        qDebug() << "Invalid settings structure.";
        return;
    }

    QJsonObject settingsObj = settingsDoc.object();

    themePath = ("bin/data/themes/" + (settingsObj.contains("theme") && !settingsObj.value("theme").toString().isEmpty() ? settingsObj.value("theme").toString() : ""));

    QFile themeFile(themePath);
    if(!themeFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to find/open theme file.";
        return;
    }

    QByteArray themeData = themeFile.readAll();
    themeFile.close();

    QJsonDocument themeDoc = QJsonDocument::fromJson(themeData);

    if (themeDoc.isNull() || !themeDoc.isObject()) {
        qDebug() << "Invalid theme structure.";
        return;
    }

    QJsonObject themeObj = themeDoc.object();

    numbersColor = themeObj.contains("numbers") ? themeObj["numbers"].toObject()["color"].toString() : "#ffffff";
    keywordsColor = themeObj.contains("keywords") ? themeObj["keywords"].toObject()["color"].toString() : "#ffffff";
    builtinFunctionsColor = themeObj.contains("functions") ? themeObj["functions"].toObject()["built-in-color"].toString() : "#ffffff";
    udfunctionsColor = themeObj.contains("functions") ? themeObj["functions"].toObject()["user-defined-color"].toString() : "#ffffff";
    datatypesColor = themeObj.contains("data-types") ? themeObj["data-types"].toObject()["color"].toString() : "#ffffff";
    booleanColor = themeObj.contains("boolean") ? themeObj["boolean"].toObject()["color"].toString() : "#ffffff";
    constantsColor = themeObj.contains("constants") ? themeObj["constants"].toObject()["color"].toString() : "#ffffff";
    commentColor = themeObj.contains("comments") ? themeObj["comments"].toObject()["color"].toString() : "#ffffff";
    stringsColor = themeObj.contains("strings") ? themeObj["strings"].toObject()["color"].toString() : "#ffffff";
    textColor = themeObj.contains("editor") ? themeObj["editor"].toObject()["text-color"].toString() : "#ffffff";

    editorColor = themeObj.contains("editor") ? themeObj["editor"].toObject()["background-color"].toString() : "#ffffff";
    menuBarColor = themeObj.contains("toriel") ? themeObj["toriel"].toObject()["menu-bar-color"].toString() : "#ffffff";
    statusBarColor = themeObj.contains("toriel") ? themeObj["toriel"].toObject()["status-bar-color"].toString() : "#ffffff";
    terminalColor = themeObj.contains("toriel") ? themeObj["toriel"].toObject()["terminal-color"].toString() : "#ffffff";
    explorerColor = themeObj.contains("explorer") ? themeObj["explorer"].toObject()["background-color"].toString() : "#ffffff";
    backgroundColor = themeObj.contains("toriel") ? themeObj["toriel"].toObject()["background-color"].toString() : "#ffffff";
    lineColor = themeObj.contains("editor") ? themeObj["editor"].toObject()["line-highlight-color"].toString() : "#ffffff";

    // Font Families
    editorFont = themeObj.contains("editor") ? themeObj["editor"].toObject()["font-family"].toString() : "Monospace";
    explorerFont = themeObj.contains("explorer") ? themeObj["explorer"].toObject()["font-family"].toString() : "Monospace";

    // Font styles, Italic bold etc
    numbersFont = themeObj.contains("numbers") ? themeObj["numbers"].toObject()["font-style"].toString() : "default";
    keywordsFont = themeObj.contains("keywords") ? themeObj["keywords"].toObject()["font-style"].toString() : "default";
    builtinFunctionsFont = themeObj.contains("functions") ? themeObj["functions"].toObject()["built-in-font-style"].toString() : "default";
    udfunctionsFont = themeObj.contains("functions") ? themeObj["functions"].toObject()["user-defined-font-style"].toString() : "default";
    datatypesFont = themeObj.contains("data-types") ? themeObj["data-types"].toObject()["font-style"].toString() : "default";
    booleanFont = themeObj.contains("boolean") ? themeObj["boolean"].toObject()["font-style"].toString() : "default";
    constantsFont = themeObj.contains("constants") ? themeObj["constants"].toObject()["font-style"].toString() : "default";
    commentsFont = themeObj.contains("comments") ? themeObj["comments"].toObject()["font-style"].toString() : "default";
    stringsFont = themeObj.contains("strings") ? themeObj["strings"].toObject()["font-style"].toString() : "default";
}

const QRegularExpression CodeHighlighter::singleLineCommentPattern(R"(//[^\n]*)");
const QRegularExpression CodeHighlighter::multiLineCommentStartPattern(R"(/\*)");
const QRegularExpression CodeHighlighter::multiLineCommentEndPattern(R"(\*/)");

void CodeHighlighter::handleComments(const QString& text) {
    int startIndex = 0;
    if (previousBlockState() != 1) {
        startIndex = text.indexOf(multiLineCommentStartPattern);
    }

    while (startIndex >= 0) {
        QRegularExpressionMatch endMatch = multiLineCommentEndPattern.match(text, startIndex);
        int endIndex;

        if (endMatch.hasMatch()) {
            endIndex = endMatch.capturedEnd();
            setFormat(startIndex, endIndex - startIndex, commentFormat);
            startIndex = text.indexOf(multiLineCommentStartPattern, endIndex);
        } else {
            setFormat(startIndex, text.length() - startIndex, commentFormat);
            setCurrentBlockState(1);
            return;
        }
    }

    QRegularExpressionMatchIterator singleLineMatches = singleLineCommentPattern.globalMatch(text);
    while (singleLineMatches.hasNext()) {
        QRegularExpressionMatch match = singleLineMatches.next();
        setFormat(match.capturedStart(), match.capturedLength(), commentFormat);
    }

    setCurrentBlockState(0);
}
