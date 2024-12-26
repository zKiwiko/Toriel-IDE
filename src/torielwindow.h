#ifndef TORIELWINDOW_H
#define TORIELWINDOW_H

#include <QMainWindow>
#include "theme.h"
#include "parser.h"

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

protected:

private:
    Ui::TorielWindow *ui;
    CodeHighlighter *highlighter;

    QString StatusBarMsg();
    QString currentFile;

    void setWidgetThemes();
    void highlightCurrentLine();
};
#endif // TORIELWINDOW_H
