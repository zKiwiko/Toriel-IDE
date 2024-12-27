#include "torielwindow.h"
#include "../ui_torielwindow.h"

#include <QTextBlock>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QDir>
#include <QTreeView>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QThread>
#include <QDesktopServices>
#include <QUrl>
#include <QTime>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QVersionNumber>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>

TorielWindow::TorielWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TorielWindow)
{
    ui->setupUi(this);
    highlighter = new CodeHighlighter(ui->code_field->document());
    parse = new Parser();
    studio = new ZenStudio();
    connect(ui->code_field, &QPlainTextEdit::cursorPositionChanged, this, &TorielWindow::highlightCurrentLine);
    setWidgetThemes();
    checkForUpdate();
    tprint(QTime::currentTime().toString("hh:mm:ss") + " | Toriel IDE version: " + toriel_ver);
}

TorielWindow::~TorielWindow()
{
    delete ui;
    delete parse;
    delete studio;
}

void TorielWindow::tprint(const QString &what) {
    ui->terminal->appendPlainText(what);
}

void TorielWindow::checkForUpdate() {
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QUrl url("https://api.github.com/repos/zKiwiko/Toriel-IDE/releases/latest");
    QNetworkRequest request(url);

    QObject::connect(manager, &QNetworkAccessManager::finished, [this, manager](QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        tprint(QTime::currentTime().toString("hh:mm:ss") + " | Failed to check for updates: " + reply->errorString());
            reply->deleteLater();
        manager->deleteLater();
        return;
    }

        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject release = jsonDoc.object();

        QString latestTag = release.value("tag_name").toString();
        if (latestTag.isEmpty()) {
            tprint(QTime::currentTime().toString("hh:mm:ss") + " | No release information available.");
            reply->deleteLater();
            manager->deleteLater();
            return;
        }

        QVersionNumber currentVer = QVersionNumber::fromString(this->toriel_ver);
        QVersionNumber latestVer = QVersionNumber::fromString(latestTag);

        if (latestVer > currentVer) {
            tprint(QTime::currentTime().toString("hh:mm:ss") + " | A new version of Toriel is available.\nhttps://github.com/zKiwiko/Toriel-IDE\n");
        } else {
            tprint(QTime::currentTime().toString("hh:mm:ss") + " | You're Up-To-Date.");
        }

        reply->deleteLater();
        manager->deleteLater();
    });
    manager->get(request);
}

void TorielWindow::setWidgetThemes() {
    QFont cf_font(highlighter->editorFont);
    QFont ex_font(highlighter->explorerFont);
    QGraphicsDropShadowEffect* glowEffect = new QGraphicsDropShadowEffect;
    glowEffect->setBlurRadius(10);
    glowEffect->setOffset(0, 0);
    glowEffect->setColor(adjustGlow(highlighter->editorColor, 100).name());

    ui->code_field->setFont(cf_font);
    ui->explorer->setFont(ex_font);

    ui->centralwidget->setStyleSheet("background-color: " + highlighter->backgroundColor + ";");
    ui->code_field->setStyleSheet("background-color: " + highlighter->editorColor + ";\n"
                                   "color: " + highlighter->textColor + ";");
    ui->code_field->setGraphicsEffect(glowEffect);
    ui->menubar->setStyleSheet("background-color: " + highlighter->menuBarColor + ";");
    ui->statusbar->setStyleSheet("background-color: " + highlighter->statusBarColor + ";");
    ui->terminal->setStyleSheet("background-color: " + highlighter->terminalColor + ";");
    ui->explorer->setStyleSheet("background-color: " + highlighter->explorerColor + ";");
    ui->directory_view->setStyleSheet("background-color: " + highlighter->explorerColor + ";");
}

QString TorielWindow::StatusBarMsg() {
    const QString text = ui->code_field->toPlainText();
    const int charCount = text.length() - text.count('\n');
    return QString("Lines: %1 | Chars: %2")
        .arg(ui->code_field->document()->lineCount())
        .arg(charCount);
}

void TorielWindow::on_code_field_textChanged()
{
    if (currentFile.isEmpty()) {
        ui->statusbar->showMessage(StatusBarMsg());
    } else {
        ui->statusbar->showMessage("Current File: " + currentFileName + " | " + StatusBarMsg());
    }
}

void TorielWindow::on_actionHide_Buttons_changed()
{
    ui->Button_Container->setVisible(!ui->actionHide_Buttons->isChecked());
}

void TorielWindow::on_actionHide_Explorer_changed()
{
    ui->Extras_Container->setVisible(!ui->actionHide_Explorer->isChecked());
}

void TorielWindow::on_actionHide_Terminal_changed()
{
    ui->terminal->setVisible(!ui->actionHide_Terminal->isChecked());
}

void TorielWindow::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    QColor LineColor = highlighter->lineColor;

    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(LineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);

    QTextCursor cursor = ui->code_field->textCursor();
    selection.cursor = cursor;
    selection.cursor.clearSelection();

    extraSelections.append(selection);

    ui->code_field->setExtraSelections(extraSelections);
}

void TorielWindow::on_BuildAndRun_clicked() {

    if (currentDir.isEmpty() || !QDir(currentDir).exists()) {
        QMessageBox::critical(this, "Error", "Invalid project directory.");
        return;
    }

    QString package_location = currentDir + "/project.json";
    if (!QFile::exists(package_location)) {
        QMessageBox::critical(this, "Error", "Missing project.json file.");
        return;
    }

    try {
        tprint(QTime::currentTime().toString("hh:mm:ss") + " | Parsing Main source file...");
        parse->parse_File(package_location);
    } catch (const std::exception& ex) {
        QMessageBox::critical(this, "Error", QString("Exception in parse_File: %1").arg(ex.what()));
        return;
    } catch (...) {
        QMessageBox::critical(this, "Error", "Unknown exception in parse_File.");
        return;
    }

    QString main_location = currentDir + "/" + parse->pr_src;
    if (!QFile::exists(main_location)) {
        QMessageBox::critical(this, "Error", "Main file not found.");
        return;
    }
    QStringList processedFiles;

    try {
        tprint(QTime::currentTime().toString("hh:mm:ss") + " | Processing Main source file and inclusions...");
        QString processedCode = parse->processMain(main_location, processedFiles);
        if (processedCode.isEmpty()) {
            QMessageBox::critical(this, "Error", "Processed code is empty.");
            return;
        }
        tprint(QTime::currentTime().toString("hh:mm:ss") + " | Sending to Zen Studio");
        studio->SendToStudio(processedCode);
        tprint(QTime::currentTime().toString("hh:mm:ss") + " | Sent to Zen Studio");
    } catch (const std::exception& ex) {
        QMessageBox::critical(this, "Error", QString("Exception: %1").arg(ex.what()));
        return;
    } catch (...) {
        QMessageBox::critical(this, "Error", "Unknown exception occurred.");
        return;
    }
}

QColor TorielWindow::adjustGlow(const QColor &color, int adjustment) {
    QColor HSL = color.toHsl();
    int lightness = HSL.lightness();

    if(lightness < 128) {
        HSL.setHsl(HSL.hue(), HSL.saturation(), qMin(lightness + adjustment, 255));
    } else {
        HSL.setHsl(HSL.hue(), HSL.saturation(), qMax(lightness - adjustment, 0));
    }
    return HSL.toRgb();
}

void TorielWindow::setTreeView() {
    QFileSystemModel *Model = new QFileSystemModel();
    Model->setRootPath(QDir::rootPath());
    QModelIndex root = Model->index(currentDir);

    ui->directory_view->setModel(Model);
    ui->directory_view->setRootIndex(root);

    for (int column = 1; column < Model->columnCount(); ++column) {
        ui->directory_view->setColumnHidden(column, true);
    }

    ui->directory_view->setHeaderHidden(true);
    ui->directory_view->setAnimated(false);
    ui->directory_view->setColumnWidth(0, 250);
}

void TorielWindow::on_actionOpen_Folder_triggered()
{
    QString directory = QFileDialog::getExistingDirectory(nullptr, "Select Folder", QDir::homePath(), QFileDialog::ShowDirsOnly);
    currentDir = directory;
    setTreeView();
    tprint(QTime::currentTime().toString("hh:mm:ss") + " | Opened folder: " + currentDir);
}

void TorielWindow::on_actionOpen_File_Ctrl_O_triggered()
{
    QString file = QFileDialog::getOpenFileName(nullptr, "Open GPC File", QDir::homePath(), "GPC Files (*.gpc);;All Files(*)");

    if(file.isEmpty()) {
        return;
    }

    currentFile = file;

    QFile f(currentFile);
    QFileInfo fi(currentFile);

    if(!f.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(nullptr, "File Error", "Couldn't Open file: " + currentFile);
        return;
    }

    QByteArray data;
    char buffer[8192];
    while (!f.atEnd()) {
        qint64 bytesRead = f.read(buffer, sizeof(buffer));
        if (bytesRead > 0) {
            data.append(buffer, bytesRead);
        }
    }

    currentFileName = fi.fileName();
    f.close();

    ui->code_field->setPlainText(QString::fromUtf8(data));
    tprint(QTime::currentTime().toString("hh:mm:ss") + " | Opened file: " + currentFileName);
}

void TorielWindow::on_actionSave_File_Ctrl_S_triggered()
{
    QString filePath = currentFile;

    if (filePath.isEmpty()) {
        filePath = QFileDialog::getSaveFileName(this, "Save File", QDir::homePath(), "GPC Files (*.gpc);;All Files (*)");

        if (filePath.isEmpty()) {
            return;
        }
        currentFile = filePath;
        QFileInfo fi(currentFile);
        currentFileName = fi.fileName();
    }

    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Error", "Could not save file: " + filePath);
        return;
    }

    QString saveWhat = ui->code_field->toPlainText();
    QTextStream out(&f);
    out << saveWhat;
    f.close();
    tprint(QTime::currentTime().toString("hh:mm:ss") + " | Saved file: " + currentFileName);
}

void TorielWindow::on_actionClose_File_triggered()
{
    tprint(QTime::currentTime().toString("hh:mm:ss") + " | Closed file: " + currentFile);
    currentFile.clear();
    ui->code_field->clear();
}

void TorielWindow::on_actionClose_Project_triggered()
{
    tprint(QTime::currentTime().toString("hh:mm:ss") + " | Closed project: " + currentDir);
    currentDir.clear();
    currentFile.clear();
    ui->code_field->clear();
    ui->directory_view->setModel(nullptr);
}

void TorielWindow::on_actionGPC_Docs_triggered()
{
    QUrl url("https://guide.cronus.support/gpc");
    QDesktopServices::openUrl(url);
}

void TorielWindow::on_actionRepository_triggered()
{
    QUrl url("https://github.com/zKiwiko/Toriel-IDE");
    QDesktopServices::openUrl(url);
}


void TorielWindow::on_directory_view_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid()) {
        tprint(QTime::currentTime().toString("hh:mm:ss") + " | Tree Err: Invalid Index");
        return;
    }

    QFileSystemModel *model = qobject_cast<QFileSystemModel*>(ui->directory_view->model());
    if(!model) {
        return;
    }
    QString filePath = model->filePath(index);
    QFileInfo fileInfo(filePath);
    if (!fileInfo.isFile()) {
        tprint(QTime::currentTime().toString("hh:mm:ss") + " | Error: Selected item is not a file");
        return;
    }

    currentFile = filePath;
    QFileInfo fi(currentFile);
    currentFileName = fi.fileName();

    QFile inputfile(currentFile);
    if(inputfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inputfile);
        QString fileContent = in.readAll();
        ui->code_field->setPlainText(fileContent);
        inputfile.close();
    } else {
        QMessageBox::warning(nullptr, "Error", "Could not open the selected file");
    }
}

