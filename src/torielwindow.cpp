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

TorielWindow::TorielWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TorielWindow)
{
    ui->setupUi(this);
    highlighter = new CodeHighlighter(ui->code_field->document());
    connect(ui->code_field, &QPlainTextEdit::cursorPositionChanged, this, &TorielWindow::highlightCurrentLine);
    setWidgetThemes();
}

TorielWindow::~TorielWindow()
{
    delete ui;
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
    ui->explorer->setVisible(!ui->actionHide_Explorer->isChecked());
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
    if (currentDir.isEmpty()) {
        studio->SendToStudio(ui->code_field->toPlainText());
    } else {
        QString package_location = currentDir + "/project.json";
        pack->parse_File(package_location);

        if (pack->project_main.isEmpty()) {
            QMessageBox::critical(this, "Error", "Main file not specified in project.json.");
            return;
        }

        QString main_location = currentDir + "/" + pack->project_main + ".gpc";

        QStringList processedFiles;
        try {
            QString processedCode = parse->processMain(main_location, processedFiles);
            studio->SendToStudio(processedCode);
        } catch (const std::exception& ex) {
            QMessageBox::critical(this, "Error", ex.what());
        }
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
}
