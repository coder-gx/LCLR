#include "Parser.h"
#include <QtWidgets/QApplication>
#include<thread>
//#include<Windows.h>

  
//class loa : public loading{
//    Q_OBJECT
//signals:
//    void loadingShown();
//public slots:
//    void showLoading() {
//        show();
//        emit loadingShown();
//    }
//};
//
//class par : public Parser {
//    Q_OBJECT
//public slots:
//    void onLoadingShown() {
//        // ��loading������ʾ��ִ�еĲ���
//        show();
//    }
//};
//
//int main(int argc, char* argv[]) {
//    QApplication a(argc, argv);
//
//    loa p;
//    par w;
//
//    QObject::connect(&p, &loa::loadingShown, &w, &par::onLoadingShown);
//
//    p.showLoading();  // ��ʾloading����
//
//    return a.exec();
//}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    
   
    loading p;
    p.show();
     QCoreApplication::processEvents();

    Parser w;

   
    p.hide();
    //p.showLoading();
    w.show();
  
    return a.exec();
}
