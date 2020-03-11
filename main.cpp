#include "mainwindow.h"
#include "node.h"
#include "linkedList.h"
#include "myBlock.h"
#include "FileSystem.h"
#include "myFile.h"

#include <QApplication>
//#include node

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
