/********************************************************************************
** Form generated from reading UI file 'Parser.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARSER_H
#define UI_PARSER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ParserClass
{
public:
    QAction *action_Code;
    QAction *action_Grammar;
    QAction *actionDFA;
    QAction *action_Token;
    QAction *action_DFA;
    QAction *action_Table;
    QAction *action_Process;
    QAction *action_Tree;
    QAction *action_Run;
    QAction *action_Restart;
    QAction *actionLR_1_DFA;
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QTextEdit *textEdit;
    QGroupBox *groupBox_2;
    QTextBrowser *textBrowser;
    QMenuBar *menuBar;
    QMenu *menu_F;
    QMenu *menu_W;
    QMenu *menu_G;
    QMenu *menu_C;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ParserClass)
    {
        if (ParserClass->objectName().isEmpty())
            ParserClass->setObjectName(QString::fromUtf8("ParserClass"));
        ParserClass->resize(829, 614);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Parser/picture/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        ParserClass->setWindowIcon(icon);
        action_Code = new QAction(ParserClass);
        action_Code->setObjectName(QString::fromUtf8("action_Code"));
        action_Grammar = new QAction(ParserClass);
        action_Grammar->setObjectName(QString::fromUtf8("action_Grammar"));
        actionDFA = new QAction(ParserClass);
        actionDFA->setObjectName(QString::fromUtf8("actionDFA"));
        action_Token = new QAction(ParserClass);
        action_Token->setObjectName(QString::fromUtf8("action_Token"));
        action_DFA = new QAction(ParserClass);
        action_DFA->setObjectName(QString::fromUtf8("action_DFA"));
        action_Table = new QAction(ParserClass);
        action_Table->setObjectName(QString::fromUtf8("action_Table"));
        action_Process = new QAction(ParserClass);
        action_Process->setObjectName(QString::fromUtf8("action_Process"));
        action_Tree = new QAction(ParserClass);
        action_Tree->setObjectName(QString::fromUtf8("action_Tree"));
        action_Run = new QAction(ParserClass);
        action_Run->setObjectName(QString::fromUtf8("action_Run"));
        action_Restart = new QAction(ParserClass);
        action_Restart->setObjectName(QString::fromUtf8("action_Restart"));
        actionLR_1_DFA = new QAction(ParserClass);
        actionLR_1_DFA->setObjectName(QString::fromUtf8("actionLR_1_DFA"));
        centralWidget = new QWidget(ParserClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setStyleSheet(QString::fromUtf8("QWidget {\n"
"  background-color: #19232D;\n"
"  border: 0px solid #455364;\n"
"  padding: 0px;\n"
"  color: #E0E1E3;\n"
"  selection-background-color: #346792;\n"
"  selection-color: #E0E1E3;\n"
"}\n"
"\n"
"QWidget:disabled {\n"
"  background-color: #19232D;\n"
"  color: #9DA9B5;\n"
"  selection-background-color: #26486B;\n"
"  selection-color: #9DA9B5;\n"
"}\n"
"\n"
"QWidget::item:selected {\n"
"  background-color: #346792;\n"
"}\n"
"\n"
"QWidget::item:hover:!selected {\n"
"  background-color: #1A72BB;\n"
"}"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(-1, -1, 831, 351));
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"	color: rgb(170, 170, 255);\n"
"  font-weight: bold;\n"
"  border: 1px solid #455364;\n"
"  border-radius: 4px;\n"
"  padding: 2px;\n"
"  margin-top: 6px;\n"
"  margin-bottom: 4px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"  rgb(255, 255, 255)\n"
"  subcontrol-origin: margin;\n"
"  subcontrol-position: top left;\n"
"  left: 4px;\n"
"  padding-left: 2px;\n"
"  padding-right: 4px;\n"
"  padding-top: -4px;\n"
"}\n"
"\n"
"QGroupBox::indicator {\n"
"  margin-left: 2px;\n"
"  margin-top: 2px;\n"
"  padding: 0;\n"
"  height: 14px;\n"
"  width: 14px;\n"
"}\n"
""));
        textEdit = new QTextEdit(groupBox);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(10, 30, 811, 311));
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        textEdit->setFont(font);
        textEdit->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"  background-color: #FFFFFF;\n"
"  color: #19232D;\n"
"  border-radius: 4px;\n"
"  border: 1px solid #C9CDD0;\n"
"}\n"
"\n"
"QTextEdit:focus {\n"
"  border: 1px solid #73C7FF;\n"
"}\n"
"\n"
"QTextEdit:selected {\n"
"  background: #9FCBFF;\n"
"  color: #C9CDD0;\n"
"}"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(0, 350, 821, 201));
        groupBox_2->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"	color: rgb(170, 170, 255);\n"
"  font-weight: bold;\n"
"  border: 1px solid #455364;\n"
"  border-radius: 4px;\n"
"  padding: 2px;\n"
"  margin-top: 6px;\n"
"  margin-bottom: 4px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"  rgb(255, 255, 255)\n"
"  subcontrol-origin: margin;\n"
"  subcontrol-position: top left;\n"
"  left: 4px;\n"
"  padding-left: 2px;\n"
"  padding-right: 4px;\n"
"  padding-top: -4px;\n"
"}\n"
"\n"
"QGroupBox::indicator {\n"
"  margin-left: 2px;\n"
"  margin-top: 2px;\n"
"  padding: 0;\n"
"  height: 14px;\n"
"  width: 14px;\n"
"}\n"
""));
        textBrowser = new QTextBrowser(groupBox_2);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(10, 30, 811, 171));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        textBrowser->setFont(font1);
        textBrowser->setStyleSheet(QString::fromUtf8("QTextBrowser {\n"
"	background-color: rgb(255, 255, 255);\n"
"  \n"
"  color: #19232D;\n"
"  border-radius: 4px;\n"
"  border: 1px solid #C9CDD0;\n"
"}"));
        ParserClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ParserClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 829, 35));
        menuBar->setStyleSheet(QString::fromUtf8("QMenuBar {\n"
"  background-color: #455364;\n"
"  padding: 2px;\n"
"  border: 1px solid #19232D;\n"
"  color: #E0E1E3;\n"
"  selection-background-color: #1A72BB;\n"
"}\n"
"\n"
"QMenuBar:focus {\n"
"  border: 1px solid #346792;\n"
"}\n"
"\n"
"QMenuBar::item {\n"
"  background: transparent;\n"
"  padding: 4px;\n"
"}\n"
"\n"
"QMenuBar::item:selected {\n"
"  padding: 4px;\n"
"  background: transparent;\n"
"  border: 0px solid #455364;\n"
"  background-color: #1A72BB;\n"
"}"));
        menu_F = new QMenu(menuBar);
        menu_F->setObjectName(QString::fromUtf8("menu_F"));
        menu_F->setToolTipsVisible(false);
        menu_W = new QMenu(menuBar);
        menu_W->setObjectName(QString::fromUtf8("menu_W"));
        menu_G = new QMenu(menuBar);
        menu_G->setObjectName(QString::fromUtf8("menu_G"));
        menu_C = new QMenu(menuBar);
        menu_C->setObjectName(QString::fromUtf8("menu_C"));
        ParserClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(ParserClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setStyleSheet(QString::fromUtf8("QStatusBar {\n"
"	color: rgb(255, 255, 255);\n"
"  border: 1px solid #455364;\n"
"  /* Fixes Spyder #9120, #9121 */\n"
"  background: #455364;\n"
"  /* Fixes #205, white vertical borders separating items */\n"
"}\n"
"\n"
"QStatusBar::item {\n"
"  border: none;\n"
"}\n"
"\n"
"QStatusBar QToolTip {\n"
"  background-color: #1A72BB;\n"
"  border: 1px solid #19232D;\n"
"  color: #19232D;\n"
"  /* Remove padding, for fix combo box tooltip */\n"
"  padding: 0px;\n"
"  /* Reducing transparency to read better */\n"
"  opacity: 230;\n"
"}\n"
"\n"
"QStatusBar QLabel {\n"
"  /* Fixes Spyder #9120, #9121 */\n"
"  background: transparent;\n"
"}\n"
""));
        ParserClass->setStatusBar(statusBar);

        menuBar->addAction(menu_F->menuAction());
        menuBar->addAction(menu_W->menuAction());
        menuBar->addAction(menu_G->menuAction());
        menuBar->addAction(menu_C->menuAction());
        menu_F->addAction(action_Code);
        menu_F->addAction(action_Grammar);
        menu_W->addAction(action_Token);
        menu_W->addAction(action_DFA);
        menu_G->addAction(action_Table);
        menu_G->addAction(action_Process);
        menu_G->addAction(action_Tree);
        menu_G->addAction(actionLR_1_DFA);
        menu_C->addAction(action_Run);
        menu_C->addAction(action_Restart);

        retranslateUi(ParserClass);

        QMetaObject::connectSlotsByName(ParserClass);
    } // setupUi

    void retranslateUi(QMainWindow *ParserClass)
    {
        ParserClass->setWindowTitle(QApplication::translate("ParserClass", "Light Compiler Based on LR1 Metod(to be continued)", nullptr));
        action_Code->setText(QApplication::translate("ParserClass", "\345\257\274\345\205\245\346\272\220\344\273\243\347\240\201", nullptr));
        action_Grammar->setText(QApplication::translate("ParserClass", "\345\257\274\345\205\245\346\226\207\346\263\225", nullptr));
        actionDFA->setText(QApplication::translate("ParserClass", "DFA", nullptr));
        action_Token->setText(QApplication::translate("ParserClass", "Token", nullptr));
        action_DFA->setText(QApplication::translate("ParserClass", "DFA", nullptr));
        action_Table->setText(QApplication::translate("ParserClass", "Action Goto Table", nullptr));
        action_Process->setText(QApplication::translate("ParserClass", "\345\275\222\347\272\246\350\277\207\347\250\213", nullptr));
        action_Tree->setText(QApplication::translate("ParserClass", "\350\257\255\346\263\225\345\210\206\346\236\220\346\240\221", nullptr));
        action_Run->setText(QApplication::translate("ParserClass", "\345\274\200\345\247\213\347\274\226\350\257\221", nullptr));
        action_Restart->setText(QApplication::translate("ParserClass", "\351\207\215\347\275\256", nullptr));
        actionLR_1_DFA->setText(QApplication::translate("ParserClass", "LR(1)  DFA", nullptr));
        groupBox->setTitle(QApplication::translate("ParserClass", "\344\273\243\347\240\201", nullptr));
        groupBox_2->setTitle(QApplication::translate("ParserClass", "\350\276\223\345\207\272", nullptr));
        menu_F->setTitle(QApplication::translate("ParserClass", "\346\226\207\344\273\266(F)", nullptr));
        menu_W->setTitle(QApplication::translate("ParserClass", "\350\257\215\346\263\225(W)", nullptr));
        menu_G->setTitle(QApplication::translate("ParserClass", "\350\257\255\346\263\225(G)", nullptr));
        menu_C->setTitle(QApplication::translate("ParserClass", "\347\274\226\350\257\221(C)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ParserClass: public Ui_ParserClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARSER_H
