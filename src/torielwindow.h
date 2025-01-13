#pragma once

#include <QMainWindow>
#include <QColor>
#include <QFileDialog>

#include "theme.h"
#include "processor.h"
#include "zenstudio.h"
#include "closingpairs.h"
#include "autocomplete.h"
#include "explorer.h"

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

    void on_actionSave_File_Ctrl_S_triggered();

    void on_actionClose_File_triggered();

    void on_actionClose_Project_triggered();

    void on_actionGPC_Docs_triggered();

    void on_actionRepository_triggered();

    void on_directory_view_doubleClicked(const QModelIndex &index);

    void on_actionImage_Generator_triggered();

    void on_actionChoose_Theme_triggered();

    void on_actionReload_Theme_triggered();

private:
    QString currentDir;
    QString currentFile;
    QString currentFileName;
    QString toriel_ver = "0.0.3";

    Ui::TorielWindow *ui;
    CodeHighlighter *highlighter;
    Processor *processor;
    ZenStudio *studio;
    AutoComplete autocomplete;
    Explorer explorer;

    QString StatusBarMsg();

    bool backup_project(const QString& sPath, const QString& dPath);
    void backup_processed(const QString& path, const QString& pC);
    void tprint(const QString &what);
    void checkForUpdate();

    QColor adjustGlow(const QColor& color, int adjustment);

    void setWidgetThemes();
    void highlightCurrentLine();
    void setTreeView();
};
