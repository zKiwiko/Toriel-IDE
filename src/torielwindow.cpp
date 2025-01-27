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
#include <QImage>
#include <cmath>
#include <vector>
#include <QClipboard>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QInputDialog>

TorielWindow::TorielWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TorielWindow)
{
    ui->setupUi(this);
    studio = new ZenStudio();
    highlighter = new CodeHighlighter(ui->code_field->document());
    processor = new Processor();

    AutoClosingPairs::Setup(ui->code_field);

    explorer.Init(ui->code_field, ui->explorer, highlighter->GPC_Datatypes);

    autocomplete.SetupWords(highlighter->GPC_Keywords, highlighter->GPC_Functions, highlighter->GPC_Datatypes, highlighter->GPC_Constants);
    autocomplete.Init(ui->code_field);
    autocomplete.Style(highlighter->editorColor, highlighter->lineColor);

    setWidgetThemes();
    checkForUpdate();

    tprint("Toriel IDE version: " + toriel_ver);
    connect(ui->code_field, &QPlainTextEdit::cursorPositionChanged, this, &TorielWindow::highlightCurrentLine);

}

TorielWindow::~TorielWindow()
{
    delete ui;
    delete processor;
    delete studio;
    delete highlighter;
}

void TorielWindow::tprint(const QString &what) {
    QString time = QTime::currentTime().toString("hh:mm:ss") + " | ";
    ui->terminal->appendPlainText(time + what);
}

void TorielWindow::checkForUpdate() {
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QUrl url("https://api.github.com/repos/zKiwiko/Toriel-IDE/releases/latest");
    QNetworkRequest request(url);

    QObject::connect(manager, &QNetworkAccessManager::finished, [this, manager](QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        tprint("Failed to check for updates: " + reply->errorString());
        reply->deleteLater();
        manager->deleteLater();
        return;
    }

        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject release = jsonDoc.object();

        QString latestTag = release.value("tag_name").toString();
        if (latestTag.isEmpty()) {
            tprint("No release information available.");
            reply->deleteLater();
            manager->deleteLater();
            return;
        }

        QVersionNumber currentVer = QVersionNumber::fromString(toriel_ver);
        QVersionNumber latestVer = QVersionNumber::fromString(latestTag);

        if (latestVer > currentVer) {
            tprint("A new version of Toriel is available at https://github.com/zKiwiko/Toriel-IDE");
        } else {
            tprint("You're Up-To-Date.");
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

    QStringList processedFiles;
    QString package_location = currentDir + "/project.json";

    if (currentDir.isEmpty()) {
        if (!currentFile.isEmpty()) {
            QFile file(currentFile);
            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::critical(this, "Error", "Selected File could not be read");
                return;
            }

            QString data = file.readAll();
            if (data.isEmpty()) {
                QMessageBox::critical(this, "Error", "File is empty or could not be read.");
                return;
            }

            file.close();
            studio->SendToStudio(data);
            tprint("Sent to Zen Studio");
            return;
        } else {
            QMessageBox::critical(this, "Error", "No file or directory selected.");
            return;
        }
    }

    if (!QFile::exists(package_location)) {
        QMessageBox::critical(this, "Error", "Missing project.json file.");
        return;
    }

    try {
        tprint("Parsing Main source file...");
        processor->parse_File(package_location);
    } catch (const std::exception& ex) {
        QMessageBox::critical(this, "Error", QString("Exception while parsing package file: %1").arg(ex.what()));
        return;
    } catch (...) {
        QMessageBox::critical(this, "Error", "Unknown exception while parsing package file.");
        return;
    }

    QString main_location = currentDir + "/" + processor->pr_src;
    if (!QFile::exists(main_location)) {
        QMessageBox::critical(this, "Error", "Main file not found.");
        return;
    }

    try {
        tprint("Processing Main source file and inclusions...");
        QString processedCode = processor->processMain(main_location, processedFiles);
        if (processedCode.isEmpty()) {
            QMessageBox::critical(this, "Error", "Processed code is empty.");
            return;
        }
        studio->SendToStudio(processedCode);
        build_processed(processedCode);

    } catch (const std::exception& ex) {
        QMessageBox::critical(this, "Error", QString("Exception: %1").arg(ex.what()));
        return;
    } catch (...) {
        QMessageBox::critical(this, "Error", "Unknown exception occurred.");
        return;
    }
}

bool TorielWindow::backup_project(const QString& sPath, const QString& dPath) {
    tprint("Backing up project...");

    QDir source(sPath);
    QDir destination(dPath);

    if(!source.exists()) {
        tprint("Source directory does not exist: " + sPath);
        return false;
    }

    if(!destination.exists()) {
        if(!destination.mkpath(".")) {
            tprint("Failed to create backup directory: " + dPath);
            return false;
        }
    }

    for(const QFileInfo &fileInfo : source.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries)) {
        QString sourceFilePath = fileInfo.absoluteFilePath();
        QString destinationFilePath = destination.filePath(fileInfo.fileName());

        if (fileInfo.isDir()) {
            if (!backup_project(sourceFilePath, destinationFilePath)) {
                return false;
            }
        } else if (fileInfo.isFile()) {
            if (!QFile::copy(sourceFilePath, destinationFilePath)) {
                tprint("Failed to copy file: " + sourceFilePath + " to " + destinationFilePath);
                return false;
            }
        }
    }
    return true;
}

void TorielWindow::build_processed(const QString &content) {
    QString buildDir = currentDir + "/build";
    QDir dir(buildDir);

    if (!dir.exists()) {
        if(!dir.mkdir(buildDir)) {
            return;
        }
    }

    QString fileName = (processor->pr_name + "-" + processor->pr_ver + ".gpc");
    QString filePath = dir.filePath(fileName);
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to create or open file:" << filePath;
        return;
    }

    QTextStream out(&file);
    out << content;
    file.close();
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
    QFileSystemModel *model = new QFileSystemModel();
    model->setRootPath(QDir::rootPath());
    QModelIndex root = model->index(currentDir);

    QFileIconProvider *iconProvider = new QFileIconProvider();
    model->setIconProvider(iconProvider);

    class CustomIconProvider : public QFileIconProvider {
    public:
        QIcon icon(const QFileInfo &info) const override {
            if (info.suffix() == "gpc" || info.suffix() == "gpch" || info.suffix() == "gph") {
                return QIcon("bin/assets/icon/files/cronus.ico");
            } else if(info.suffix() == "json") {
                return QIcon("bin/assets/icon/files/json.ico");
            } else {
                return QFileIconProvider::icon(info);
            }
        }
    };

    model->setIconProvider(new CustomIconProvider());

    ui->directory_view->setModel(model);
    ui->directory_view->setRootIndex(root);

    for (int column = 1; column < model->columnCount(); ++column) {
        ui->directory_view->setColumnHidden(column, true);
    }

    ui->directory_view->setHeaderHidden(true);
    ui->directory_view->setAnimated(false);
    ui->directory_view->setColumnWidth(0, 250);
}

void TorielWindow::on_actionOpen_Folder_triggered()
{
    QString directory = QFileDialog::getExistingDirectory(
        nullptr,
        "Select Folder",
        currentDir.isEmpty() ? QDir::homePath() : currentDir,
        QFileDialog::ShowDirsOnly
        );

    if (directory.isEmpty()) {
        ui->directory_view->setModel(nullptr);
        tprint("Folder selection canceled.");
    } else {
        currentDir = directory;
        setTreeView();
        tprint("Opened folder: " + currentDir);
        updateIntelliSense();
    }
}

void TorielWindow::on_actionOpen_File_Ctrl_O_triggered()
{
    QString file = QFileDialog::getOpenFileName(
        nullptr,
        "Open GPC File",
        currentDir.isEmpty() ? QDir::homePath() : currentDir,
        "GPC Files (*.gpc);;All Files (*)"
        );

    if(file.isEmpty()) {
        return;
    }

    currentFile = file;
    QFileInfo fi(currentFile);
    currentFileName = fi.fileName();

    QFuture<QByteArray> future = QtConcurrent::run([this] {
        QFile f(currentFile);
        if(!f.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(nullptr, "File Error", "Couldn't Open file: " + currentFile);
            return QByteArray();
        }

        QByteArray data;
        char buffer[8192];
        while (!f.atEnd()) {
            qint64 bytesRead = f.read(buffer, sizeof(buffer));
            if (bytesRead > 0) {
                data.append(buffer, bytesRead);
            }
        }
        f.close();
        return data;
    });

    QFutureWatcher<QByteArray> *watcher = new QFutureWatcher<QByteArray>(this);
    connect(watcher, &QFutureWatcher<QByteArray>::finished, [this, watcher] {
        QByteArray data = watcher->result();
        if (!data.isEmpty()) {
            ui->code_field->setPlainText(QString::fromUtf8(data));
            tprint("Opened file: " + currentFileName);
        }
        watcher->deleteLater();
    });
    watcher->setFuture(future);
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
    tprint("Saved file: " + currentFileName);
}

void TorielWindow::on_actionClose_File_triggered()
{
    tprint("Closed file: " + currentFile);
    currentFile.clear();
    ui->code_field->clear();
}

void TorielWindow::on_actionClose_Project_triggered()
{
    tprint("Closed project: " + currentDir);
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
        tprint("Tree Err: Invalid Index");
        return;
    }

    QFileSystemModel *model = qobject_cast<QFileSystemModel*>(ui->directory_view->model());
    if(!model) {
        return;
    }

    if(!currentFile.isEmpty()) {
        QFile f(currentFile);
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(nullptr, "Error", "Could not save file: " + currentFile);
            return;
        }

        QString saveWhat = ui->code_field->toPlainText();
        QTextStream out(&f);
        out << saveWhat;
        f.close();
        tprint("Saved file: " + currentFileName);
    }

    QString filePath = model->filePath(index);
    QFileInfo fileInfo(filePath);
    if (!fileInfo.isFile()) {
        tprint("Error: Selected item is not a file");
        return;
    }

    currentFile = filePath;
    QFileInfo fi(currentFile);
    currentFileName = fi.fileName();

    QFuture<QByteArray> future = QtConcurrent::run([this] {
        QFile f(currentFile);
        if(!f.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(nullptr, "File Error", "Couldn't Open file: " + currentFile);
            return QByteArray();
        }

        QByteArray data;
        char buffer[8192];
        while (!f.atEnd()) {
            qint64 bytesRead = f.read(buffer, sizeof(buffer));
            if (bytesRead > 0) {
                data.append(buffer, bytesRead);
            }
        }
        f.close();
        return data;
    });

    QFutureWatcher<QByteArray> *watcher = new QFutureWatcher<QByteArray>(this);
    connect(watcher, &QFutureWatcher<QByteArray>::finished, [this, watcher] {
        QByteArray data = watcher->result();
        if (!data.isEmpty()) {
            ui->code_field->setPlainText(QString::fromUtf8(data));
            tprint("Opened file: " + currentFileName);
        }
        watcher->deleteLater();
    });
    watcher->setFuture(future);
}


void TorielWindow::on_actionImage_Generator_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Select Image", QDir::rootPath(), "Images (*.png *.jpg);;All Files (*)");
    if(filePath.isEmpty()) {
        return;
    }

    QImage img(filePath);
    if (img.isNull()) {
        QMessageBox::warning(nullptr, "Error", "Failed to load the image!");
        return;
    }

    int width = img.width();
    int height = img.height();

    int arraySize = std::ceil(double(width * height) / 8);
    std::vector<int> ints(arraySize, 0);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor color = img.pixelColor(x, y);
            if(color.red() > 127) {
                int i = y * width + x;
                ints[i >> 3] |= 1 << (7 - (i & 7));
            }
        }
    }

    QStringList list;
    list.append(QString("%1,%2").arg(width).arg(height));
    for(int i : ints) {
        list.append(QString::number(i));
    }

    QString clipboardText = list.join(",");
    QClipboard *clipboard = QGuiApplication::clipboard();

    clipboard->setText(clipboardText);
    tprint("Generated Image copied to clipboard.");
    tprint("Generated Image backed up to `bin/data/generated/images`");

    QDir dir;
    if (!dir.exists("bin/data/generated/images")) {
        dir.mkpath("bin/data/generated/images");
    }

    QFile file("bin/data/generated/images/array_backup.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << QString("%1,%2,").arg(width).arg(height);
        for (int i : ints) {
            out << i << ",";
        }
        file.close();
    } else {
        QMessageBox::warning(nullptr, "Error", "Failed to save the backup file!");
    }
}

void TorielWindow::on_actionChoose_Theme_triggered()
{
    highlighter->ReloadThemeData();
}

void TorielWindow::on_actionReload_Theme_triggered()
{
    highlighter->RetrieveThemeData();
    highlighter->SetSyntaxFormat();
    setWidgetThemes();
}

void TorielWindow::updateIntelliSense() {

    if(currentDir.isEmpty()) {
        return;
    }
    if(!QFile(currentDir + "/project.json").exists()) {
        return;
    }

    QStringList headers = processor->getProjectHeaders((currentDir + "/project.json"));
    QStringList std = processor->getStandardLibrary((currentDir + "/project.json"));

    autocomplete.headerContent = "";

    autocomplete.resyncHeaders(currentDir, headers);
    autocomplete.resyncStd("bin/data/gpc/libs", std);
}

void TorielWindow::on_actionNew_File_Ctrl_N_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Create New File", "", "GPC Files (*.gpc);;All Files (*)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.close();
        }
    }
}

void TorielWindow::on_actionNew_Project_Directory_triggered()
{
    if(currentDir.isEmpty()) {
        return;
    }
    bool i;
    QString dirName = QInputDialog::getText(this, "New Directory", "Enter directory name:", QLineEdit::Normal, "", &i);
    if(!dirName.isEmpty()) {
        QDir dir(currentDir);
        if(i && dir.mkdir(dirName)) {
            setTreeView();
        } else {
            return;
        }
    }
}

void TorielWindow::on_actionNew_Project_File_triggered()
{
    if(currentDir.isEmpty()) {
        return;
    }
    QString path;
    QString fileName = QInputDialog::getText(this, "New File", "Enter file name:", QLineEdit::Normal, "");
    if(!fileName.isEmpty()) {
        if(!fileName.contains(".gpc")) {
            fileName += ".gpc";
        }
        path = currentDir + "/" + fileName;

        QFile file(path);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&fileName);
            out << "// Created " << QDateTime::currentDateTime().toString("mm:dd:yyyy");
            file.close();
        }
    }
}


void TorielWindow::on_actionDelete_Project_FIle_triggered()
{
    if(currentDir.isEmpty()) {
        return;
    }
    QString path;
    QString fileName = QInputDialog::getText(this, "Delete File", "Enter file name:", QLineEdit::Normal, "");
    if(!fileName.isEmpty()) {
        if(!fileName.contains(".gpc")) {
            fileName += ".gpc";
        }
        path = currentDir + "/" + fileName;

        QFile file(path);

        if(file.exists()) {
            file.remove();
        }
    }
}


void TorielWindow::on_actionDelete_Project_Directory_triggered()
{
    if(currentDir.isEmpty()) {
        return;
    }
    QString dirName = QInputDialog::getText(this, "Delete Directory", "Enter directory name:", QLineEdit::Normal, "");
    if(!dirName.isEmpty()) {
        QDir dir(currentDir + "/" + dirName);
        if(dir.exists()) {
            dir.removeRecursively();
        }
    }
}

void TorielWindow::on_resync_Button_pressed()
{
    updateIntelliSense();
}

