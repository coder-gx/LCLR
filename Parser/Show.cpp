#include "Show.h"
#include <qdebug.h>
#include"myQGraphicsView.h"
#include <qmessagebox.h>
#include<fstream>
#include<sstream>
#include<qfile.h>
#include<qtextbrowser.h>
#include<qtablewidget.h>
#include<qpixmap.h>




Show::Show(QWidget* parent) : QMainWindow(parent)
{
    this->layout =NULL;
    this->ui = new Ui::ShowWindow;
    this->ui->setupUi(this);
}


Show::~Show()
{
}

/**
* @func:   չʾ������token���б�
* @para:   
* @return: 
*/
void Show::showToken()
{

    this->ui->centralwidget->setWindowTitle("Token��ʾ");

    QTextBrowser* textBrowser = new QTextBrowser(this->ui->centralwidget);
    // ���� TextBrowser ������
    QFile file(TOKEN_LIST_FILE);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        // ��ȡ�ļ����ݲ�д�� TextBrowser
        textBrowser->setPlainText(in.readAll());
        file.close();
    }
    else {
        // �����ļ���ʧ�ܵ����
        textBrowser->setPlainText("Error: Unable to open the file.");
    }
    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // �� TextBrowser ��ӵ�������
    this->layout->addWidget(textBrowser);
    // ���������ڵĲ���
    this->ui->centralwidget->setLayout(this->layout);
}
  
/**
* @func:   չʾ�ʷ�DFA
* @para:
* @return:
*/
void Show::showDFA()
{
    
    // ����һ�� QGraphicsView
    myQGraphicsView* view = new myQGraphicsView(this->ui->centralwidget);
    // ����һ�� QGraphicsScene
    QGraphicsScene* scene = new QGraphicsScene(this->ui->centralwidget);

    QPixmap pixmap(DFA_PNG_FILE);

    scene->addPixmap(pixmap);
    view->setDragMode(QGraphicsView::ScrollHandDrag);// ���������ק��ʽ

    // ���������ø� QGraphicsView
    view->setScene(scene);

    if (this->layout != NULL) {
        delete this->layout;
    }
    // ����һ����ֱ���ֹ�����
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // �� QGraphicsView ��ӵ�������
    this->layout->addWidget(view);

    // ���������ڵĲ���
    this->ui->centralwidget->setLayout(this->layout);
}

/**
* @func:   //չʾAction goto��
* @para:
* @return:
*/
void Show::showTable()
{
    this->ui->centralwidget->setWindowTitle("Action Goto����ʾ");
    // ���� QTableWidget ����
    QTableWidget* tableWidget = new QTableWidget();

    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);
    


    // ��CSV�ļ�
    QFile file(Table_FILE);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        // ��ȡ�ļ����ݲ�����Ϊ��
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(',');
            //qDebug() << line<<endl;

            // ��QTableWidget�в���һ��
            int row = tableWidget->rowCount();
            tableWidget->insertRow(row);
           

            // �ڵ�ǰ�е�ÿһ���в�������
            tableWidget->setColumnCount(fields.size());
            for (int column = 0; column < fields.size(); column++) {
               // qDebug() << fields.at(column);
                QTableWidgetItem* item = new QTableWidgetItem(fields.at(column));
                tableWidget->setItem(row, column, item);
            }
           // tableWidget->resizeColumnsToContents();
        }
        tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

        file.close();
    }
    else {
        // �����ļ���ʧ�ܵ����
        QTableWidgetItem* errorItem = new QTableWidgetItem("Error: Unable to open the file.");
        tableWidget->setItem(0, 0, errorItem);
    }

    // �� QTableWidget ��ӵ�������
    this->layout->addWidget(tableWidget);

    // ���������ڵĲ���
    this->ui->centralwidget->setLayout(this->layout);
}

/**
* @func:   //չʾ�﷨������
* @para:
* @return:
*/
void Show::showTree()
{
    this->ui->centralwidget->setWindowTitle("Action Goto����ʾ");
    // ����һ�� QGraphicsView
    myQGraphicsView* view = new myQGraphicsView(this->ui->centralwidget);

    // ����һ�� QGraphicsScene
    QGraphicsScene* scene = new QGraphicsScene(this->ui->centralwidget);
   
    QPixmap pixmap(TREE_PNG_FILE);

    scene->addPixmap(pixmap);
    view->setDragMode(QGraphicsView::ScrollHandDrag);// ���������ק��ʽ

    // ���������ø� QGraphicsView
    view->setScene(scene);

    if (this->layout != NULL) {
        delete this->layout;
    }
    // ����һ����ֱ���ֹ�����
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // �� QGraphicsView ��ӵ�������
    this->layout->addWidget(view);

    // ���������ڵĲ���
    this->ui->centralwidget->setLayout(this->layout);

}

/**
* @func:   //չʾ�﷨DFA
* @para:
* @return:
*/
void Show::showParseDFA()
{
   
    
    this->ui->centralwidget->setWindowTitle("LR(1) DFA��ʾ");
    // ����һ�� QGraphicsView
    myQGraphicsView* view = new myQGraphicsView(this->ui->centralwidget);

    // ����һ�� QGraphicsScene
    QGraphicsScene* scene = new QGraphicsScene(this->ui->centralwidget);

    QPixmap pixmap(PARSE_DFA_PNG_FILE);

    scene->addPixmap(pixmap);
    view->setDragMode(QGraphicsView::ScrollHandDrag);// ���������ק��ʽ

    // ���������ø� QGraphicsView
    view->setScene(scene);

    if (this->layout != NULL) {
        delete this->layout;
    }
    // ����һ����ֱ���ֹ�����
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // �� QGraphicsView ��ӵ�������
    this->layout->addWidget(view);

    // ���������ڵĲ���
    this->ui->centralwidget->setLayout(this->layout);

}

/**
* @func:   //չʾ��Լ����
* @para:
* @return:
*/
void Show::showProcess()
{
    this->ui->centralwidget->setWindowTitle("��Լ������ʾ");
    QTextBrowser* textBrowser = new QTextBrowser(this->ui->centralwidget);
    // ���� TextBrowser ������
    QFile file(REDUCTION_PROCESS_FILE);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        // ��ȡ�ļ����ݲ�д�� TextBrowser
        textBrowser->setPlainText(in.readAll());
        file.close();
    }
    else {
        // �����ļ���ʧ�ܵ����
        textBrowser->setPlainText("Error: Unable to open the file.");
    }
    if (this->layout != NULL) {
        delete this->layout;
    }
    this->layout = new QVBoxLayout(this->ui->centralwidget);

    // �� TextBrowser ��ӵ�������
    this->layout->addWidget(textBrowser);

    // ���������ڵĲ���
    this->ui->centralwidget->setLayout(this->layout);

}