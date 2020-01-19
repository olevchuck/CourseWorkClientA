#include <QApplication>
#include "galconserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CGalconServer w(1234);
    w.show();

    return a.exec();
}
