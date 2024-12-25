/********************************************************************************
** Form generated from reading UI file 'torielwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TORIELWINDOW_H
#define UI_TORIELWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TorielWindow
{
public:
    QAction *actionNew_File_Ctrl_N;
    QAction *actionOpen_File_Ctrl_O;
    QAction *actionSave_File_Ctrl_S;
    QAction *actionChoose_Theme;
    QAction *actionHide_Buttons;
    QAction *actionHide_Explorer;
    QAction *actionHide_Suggestions;
    QAction *actionHide_Terminal;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QFrame *Button_Container;
    QVBoxLayout *verticalLayout_3;
    QPushButton *BuildAndRun;
    QPushButton *create_backup;
    QSpacerItem *verticalSpacer;
    QPushButton *GPC_Docs;
    QPushButton *Toriel_Docs;
    QPushButton *github_repository;
    QWidget *Editor_Containers;
    QVBoxLayout *verticalLayout_5;
    QPlainTextEdit *code_field;
    QPlainTextEdit *terminal;
    QWidget *Extras_Container;
    QVBoxLayout *verticalLayout_4;
    QLabel *label;
    QPlainTextEdit *suggestions;
    QLabel *label_2;
    QPlainTextEdit *explorer;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuSettings;
    QMenu *menuView;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TorielWindow)
    {
        if (TorielWindow->objectName().isEmpty())
            TorielWindow->setObjectName("TorielWindow");
        TorielWindow->resize(1280, 680);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TorielWindow->sizePolicy().hasHeightForWidth());
        TorielWindow->setSizePolicy(sizePolicy);
        actionNew_File_Ctrl_N = new QAction(TorielWindow);
        actionNew_File_Ctrl_N->setObjectName("actionNew_File_Ctrl_N");
        actionOpen_File_Ctrl_O = new QAction(TorielWindow);
        actionOpen_File_Ctrl_O->setObjectName("actionOpen_File_Ctrl_O");
        actionSave_File_Ctrl_S = new QAction(TorielWindow);
        actionSave_File_Ctrl_S->setObjectName("actionSave_File_Ctrl_S");
        actionChoose_Theme = new QAction(TorielWindow);
        actionChoose_Theme->setObjectName("actionChoose_Theme");
        actionHide_Buttons = new QAction(TorielWindow);
        actionHide_Buttons->setObjectName("actionHide_Buttons");
        actionHide_Buttons->setCheckable(true);
        actionHide_Explorer = new QAction(TorielWindow);
        actionHide_Explorer->setObjectName("actionHide_Explorer");
        actionHide_Explorer->setCheckable(true);
        actionHide_Suggestions = new QAction(TorielWindow);
        actionHide_Suggestions->setObjectName("actionHide_Suggestions");
        actionHide_Suggestions->setCheckable(true);
        actionHide_Terminal = new QAction(TorielWindow);
        actionHide_Terminal->setObjectName("actionHide_Terminal");
        actionHide_Terminal->setCheckable(true);
        centralwidget = new QWidget(TorielWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setEnabled(true);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        centralwidget->setMinimumSize(QSize(480, 240));
        centralwidget->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        Button_Container = new QFrame(centralwidget);
        Button_Container->setObjectName("Button_Container");
        Button_Container->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Button_Container->sizePolicy().hasHeightForWidth());
        Button_Container->setSizePolicy(sizePolicy1);
        Button_Container->setMinimumSize(QSize(120, 0));
        Button_Container->setMaximumSize(QSize(160, 16777215));
        verticalLayout_3 = new QVBoxLayout(Button_Container);
        verticalLayout_3->setObjectName("verticalLayout_3");
        BuildAndRun = new QPushButton(Button_Container);
        BuildAndRun->setObjectName("BuildAndRun");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(BuildAndRun->sizePolicy().hasHeightForWidth());
        BuildAndRun->setSizePolicy(sizePolicy2);
        BuildAndRun->setMaximumSize(QSize(127, 27));
        BuildAndRun->setSizeIncrement(QSize(0, 0));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));
        BuildAndRun->setIcon(icon);

        verticalLayout_3->addWidget(BuildAndRun);

        create_backup = new QPushButton(Button_Container);
        create_backup->setObjectName("create_backup");
        sizePolicy2.setHeightForWidth(create_backup->sizePolicy().hasHeightForWidth());
        create_backup->setSizePolicy(sizePolicy2);
        create_backup->setMaximumSize(QSize(125, 27));

        verticalLayout_3->addWidget(create_backup);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        GPC_Docs = new QPushButton(Button_Container);
        GPC_Docs->setObjectName("GPC_Docs");
        sizePolicy2.setHeightForWidth(GPC_Docs->sizePolicy().hasHeightForWidth());
        GPC_Docs->setSizePolicy(sizePolicy2);
        GPC_Docs->setMinimumSize(QSize(0, 0));
        GPC_Docs->setMaximumSize(QSize(125, 27));

        verticalLayout_3->addWidget(GPC_Docs);

        Toriel_Docs = new QPushButton(Button_Container);
        Toriel_Docs->setObjectName("Toriel_Docs");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(Toriel_Docs->sizePolicy().hasHeightForWidth());
        Toriel_Docs->setSizePolicy(sizePolicy3);
        Toriel_Docs->setMaximumSize(QSize(127, 16777215));

        verticalLayout_3->addWidget(Toriel_Docs);

        github_repository = new QPushButton(Button_Container);
        github_repository->setObjectName("github_repository");
        sizePolicy2.setHeightForWidth(github_repository->sizePolicy().hasHeightForWidth());
        github_repository->setSizePolicy(sizePolicy2);
        github_repository->setMaximumSize(QSize(125, 27));

        verticalLayout_3->addWidget(github_repository);


        horizontalLayout_2->addWidget(Button_Container);

        Editor_Containers = new QWidget(centralwidget);
        Editor_Containers->setObjectName("Editor_Containers");
        verticalLayout_5 = new QVBoxLayout(Editor_Containers);
        verticalLayout_5->setObjectName("verticalLayout_5");
        code_field = new QPlainTextEdit(Editor_Containers);
        code_field->setObjectName("code_field");
        sizePolicy.setHeightForWidth(code_field->sizePolicy().hasHeightForWidth());
        code_field->setSizePolicy(sizePolicy);
        code_field->setMinimumSize(QSize(0, 0));
        code_field->setMaximumSize(QSize(16777215, 16777215));
        code_field->setStyleSheet(QString::fromUtf8("QPlainTextEdit {\n"
"	border-radius: 0px;\n"
"}"));
        code_field->setTabStopDistance(40.000000000000000);

        verticalLayout_5->addWidget(code_field);

        terminal = new QPlainTextEdit(Editor_Containers);
        terminal->setObjectName("terminal");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(terminal->sizePolicy().hasHeightForWidth());
        terminal->setSizePolicy(sizePolicy4);
        terminal->setMaximumSize(QSize(16777215, 111));

        verticalLayout_5->addWidget(terminal);


        horizontalLayout_2->addWidget(Editor_Containers);

        Extras_Container = new QWidget(centralwidget);
        Extras_Container->setObjectName("Extras_Container");
        QSizePolicy sizePolicy5(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(Extras_Container->sizePolicy().hasHeightForWidth());
        Extras_Container->setSizePolicy(sizePolicy5);
        Extras_Container->setMaximumSize(QSize(210, 16777215));
        verticalLayout_4 = new QVBoxLayout(Extras_Container);
        verticalLayout_4->setObjectName("verticalLayout_4");
        label = new QLabel(Extras_Container);
        label->setObjectName("label");

        verticalLayout_4->addWidget(label);

        suggestions = new QPlainTextEdit(Extras_Container);
        suggestions->setObjectName("suggestions");
        QSizePolicy sizePolicy6(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(suggestions->sizePolicy().hasHeightForWidth());
        suggestions->setSizePolicy(sizePolicy6);
        suggestions->setMaximumSize(QSize(201, 16777215));
        suggestions->viewport()->setProperty("cursor", QVariant(QCursor(Qt::CursorShape::CrossCursor)));
        suggestions->setReadOnly(true);

        verticalLayout_4->addWidget(suggestions);

        label_2 = new QLabel(Extras_Container);
        label_2->setObjectName("label_2");

        verticalLayout_4->addWidget(label_2);

        explorer = new QPlainTextEdit(Extras_Container);
        explorer->setObjectName("explorer");
        sizePolicy6.setHeightForWidth(explorer->sizePolicy().hasHeightForWidth());
        explorer->setSizePolicy(sizePolicy6);
        explorer->setMaximumSize(QSize(201, 16777215));
        explorer->viewport()->setProperty("cursor", QVariant(QCursor(Qt::CursorShape::CrossCursor)));
        explorer->setReadOnly(true);

        verticalLayout_4->addWidget(explorer);


        horizontalLayout_2->addWidget(Extras_Container);

        TorielWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(TorielWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1280, 25));
        menubar->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen));
        menuFile->setIcon(icon1);
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName("menuSettings");
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::DriveOptical));
        menuSettings->setIcon(icon2);
        menuView = new QMenu(menubar);
        menuView->setObjectName("menuView");
        QIcon icon3(QIcon::fromTheme(QIcon::ThemeIcon::SystemSearch));
        menuView->setIcon(icon3);
        TorielWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(TorielWindow);
        statusbar->setObjectName("statusbar");
        TorielWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuSettings->menuAction());
        menuFile->addAction(actionNew_File_Ctrl_N);
        menuFile->addAction(actionOpen_File_Ctrl_O);
        menuFile->addAction(actionSave_File_Ctrl_S);
        menuSettings->addAction(actionChoose_Theme);
        menuView->addAction(actionHide_Buttons);
        menuView->addAction(actionHide_Explorer);
        menuView->addAction(actionHide_Suggestions);
        menuView->addAction(actionHide_Terminal);

        retranslateUi(TorielWindow);

        QMetaObject::connectSlotsByName(TorielWindow);
    } // setupUi

    void retranslateUi(QMainWindow *TorielWindow)
    {
        TorielWindow->setWindowTitle(QCoreApplication::translate("TorielWindow", "Toriel IDE", nullptr));
        actionNew_File_Ctrl_N->setText(QCoreApplication::translate("TorielWindow", "New File...", nullptr));
#if QT_CONFIG(shortcut)
        actionNew_File_Ctrl_N->setShortcut(QCoreApplication::translate("TorielWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpen_File_Ctrl_O->setText(QCoreApplication::translate("TorielWindow", "Open File...", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen_File_Ctrl_O->setShortcut(QCoreApplication::translate("TorielWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave_File_Ctrl_S->setText(QCoreApplication::translate("TorielWindow", "Save File", nullptr));
#if QT_CONFIG(shortcut)
        actionSave_File_Ctrl_S->setShortcut(QCoreApplication::translate("TorielWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionChoose_Theme->setText(QCoreApplication::translate("TorielWindow", "Choose Theme", nullptr));
        actionHide_Buttons->setText(QCoreApplication::translate("TorielWindow", "Hide Buttons", nullptr));
        actionHide_Explorer->setText(QCoreApplication::translate("TorielWindow", "Hide Explorer", nullptr));
        actionHide_Suggestions->setText(QCoreApplication::translate("TorielWindow", "Hide Suggestions", nullptr));
        actionHide_Terminal->setText(QCoreApplication::translate("TorielWindow", "Hide Terminal", nullptr));
        BuildAndRun->setText(QCoreApplication::translate("TorielWindow", "Build", nullptr));
#if QT_CONFIG(shortcut)
        BuildAndRun->setShortcut(QCoreApplication::translate("TorielWindow", "Ctrl+R", nullptr));
#endif // QT_CONFIG(shortcut)
        create_backup->setText(QCoreApplication::translate("TorielWindow", "Create Backup", nullptr));
        GPC_Docs->setText(QCoreApplication::translate("TorielWindow", "GPC Docs", nullptr));
        Toriel_Docs->setText(QCoreApplication::translate("TorielWindow", "Toriel Docs", nullptr));
        github_repository->setText(QCoreApplication::translate("TorielWindow", "Repository", nullptr));
        label->setText(QCoreApplication::translate("TorielWindow", "Suggestions", nullptr));
        label_2->setText(QCoreApplication::translate("TorielWindow", "Explorer", nullptr));
        menuFile->setTitle(QCoreApplication::translate("TorielWindow", "File", nullptr));
        menuSettings->setTitle(QCoreApplication::translate("TorielWindow", "Settings", nullptr));
        menuView->setTitle(QCoreApplication::translate("TorielWindow", "View", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TorielWindow: public Ui_TorielWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TORIELWINDOW_H
