#include "parser.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QException>
#include <QMessageBox>

QString Parser::processMain(const QString &mainPath, QStringList& processedFiles) {
    if (processedFiles.contains(mainPath)) {
        QMessageBox::critical(nullptr, "Error", "Circular import detected for file: " + mainPath);
    }
    processedFiles.append(mainPath);

    QFile inputFile(mainPath);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error(
            QString("Unable to open file: %1").arg(mainPath).toStdString());
    }

    QTextStream inStream(&inputFile);
    QString result;

    QString baseDir = QFileInfo(mainPath).absolutePath();

    while (!inStream.atEnd()) {
        QString line = inStream.readLine();

        if (line.startsWith("// include")) {
            QStringList tokens = line.split(' ', Qt::SkipEmptyParts);
            if (tokens.size() < 2) {
                throw std::runtime_error(
                    QString("Malformed include directive: %1").arg(line).toStdString());
            }
            QString includeFileName = tokens[1].trimmed();

            QString includeFilePath = baseDir + "/" + includeFileName;
            QString includeContent = processMain(includeFilePath, processedFiles);
            result += includeContent + "\n";
        } else {
            result += line + "\n";
        }
    }

    return result;
}
