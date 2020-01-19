#include "GalconThread.h"

CGalconThread::CGalconThread(QObject *parent) :
    QThread(parent)
{
}
void CGalconThread::run()
{
    QThread::exec();
}
