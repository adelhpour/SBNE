#include "ne_mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if TELLURIUM_INCLUDED
    CPyInstance pyInstance;
#endif
    
    QApplication a(argc, argv);
    

    MainWindow w;
    w.show();
    
    return a.exec();
}

