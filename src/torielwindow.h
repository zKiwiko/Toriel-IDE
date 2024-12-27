#pragma once

#include <QMainWindow>
#include <QColor>
#include <QFileDialog>

#include "theme.h"
#include "parser.h"
#include "package_parser.h"
#include "zenstudio.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class TorielWindow;
}
QT_END_NAMESPACE

class TorielWindow : public QMainWindow
{
    Q_OBJECT

public:
    TorielWindow(QWidget *parent = nullptr);
    ~TorielWindow();

private slots:
    void on_code_field_textChanged();

    void on_actionHide_Buttons_changed();

    void on_actionHide_Explorer_changed();

    void on_actionHide_Terminal_changed();

    void on_BuildAndRun_clicked();

    void on_actionOpen_Folder_triggered();

    void on_actionOpen_File_Ctrl_O_triggered();

private:
    QString currentDir;
    QString currentFile;
    QString currentFileName;

    Ui::TorielWindow *ui;
    CodeHighlighter *highlighter;
    Package *pack;
    Parser *parse;
    ZenStudio *studio;

    QString StatusBarMsg();

    void create_backup();

    QColor adjustGlow(const QColor& color, int adjustment);

    void setWidgetThemes();
    void highlightCurrentLine();
    void setTreeView();
};
