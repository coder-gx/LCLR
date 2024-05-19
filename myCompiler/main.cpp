#include "myCompiler.h"
#include <QtWidgets/QApplication>
#include<thread>
//#include<Windows.h>




int main(int argc, char* argv[])
{
    QApplication a(argc, argv);



    loading p;
    p.show();
    QCoreApplication::processEvents();

    myCompiler w;


    p.hide();
    //p.showLoading();
    w.show();

    return a.exec();
}
