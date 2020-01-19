#ifndef GALCONSERVER_H
#define GALCONSERVER_H

#include <QDialog>
#include <map>
#include <QString>

class QNetworkSession;
class QTcpServer;
class QTcpSocket;

typedef unsigned int PortNumber;
typedef unsigned int SocketID;

namespace Ui {
    class CGalconServer;
}

class CGalconServer : public QDialog
{
    Q_OBJECT

public:
    CGalconServer(PortNumber portNumber, QWidget *parent = 0);
    ~CGalconServer();

private:
    Ui::CGalconServer *ui;

private:
   void sendToClient(QTcpSocket* pSocket, const QString& str);
   void sendToConnectedClients(const QString& str);

private slots:
   void slotNewConnection();
   void slotReadClient();
   void removeSocket();   

   void on_pBSC_CONNID_clicked();
   void on_pBSC_TIMETOSTART_clicked();
   void on_pBSC_START_clicked();
   void on_pBSC_STATE_clicked();
   void on_pBSC_FINISH_clicked();
   void on_pBSC_ERR_clicked();
   void on_pBSend_clicked();

private:
   typedef std::pair<unsigned int, QTcpSocket*> SocketIDPair;
   typedef std::map<unsigned int, QTcpSocket*> SocketsMap;
   QTcpServer *m_tcpServer;   
   unsigned int m_currentClientID;
   SocketsMap m_connectedSockets;

   QString m_mesStr;
};

#endif // GALCONSERVER_H
