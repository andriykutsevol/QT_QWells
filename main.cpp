#include "mainwindow.h"
#include <QApplication>
#include <iostream>

#include "SomeTests/some.h"

#include <cuda_runtime.h>
// This is the 'elder trick of the...' - Tell the compiler this function is defined in other place
extern "C"
void cuda_main();

//void foo ( void );
//extern void bar ( void );
//static void baz ( void );


int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();


//    foo();
//    bar();
//    baz();
    //std::cout<<"hello" << std::endl;

}
