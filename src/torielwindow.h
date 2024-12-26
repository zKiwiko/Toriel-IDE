#pragma once

#include <QMainWindow>
#include "theme.h"
#include "parser.h"
#include "package_parser.h"

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

    void on_actionHide_Suggestions_changed();

    void on_actionHide_Terminal_changed();

    void on_BuildAndRun_clicked();

protected:

private:
    Ui::TorielWindow *ui;
    CodeHighlighter *highlighter;
    Package *pack;
    Parser *parse;

    QString StatusBarMsg();
    QString currentDir;
    QString currentFile;

    void create_backup();
    void sendToStudio(const QString &what, bool project);

    void setWidgetThemes();
    void highlightCurrentLine();
};
