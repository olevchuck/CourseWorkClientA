#include <QtGui/QApplication>
#include "galconclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CGalconClient w;
    w.show();

    return a.exec();
}
