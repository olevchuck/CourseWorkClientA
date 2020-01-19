#ifndef GALCONTHREAD_H
#define GALCONTHREAD_H

#include <QThread>

class CGalconThread : public QThread
{
    Q_OBJECT
public:
    explicit CGalconThread(QObject *parent = 0);

protected:
    virtual void run();

signals:

public slots:

};

#endif // GALCONTHREAD_H
