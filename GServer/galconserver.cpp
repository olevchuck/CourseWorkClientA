#include "galconserver.h"
#include "ui_galconserver.h"

#include <QtGui>
#include <QtNetwork>
#include <QMessageBox>

static const char socketID[] = "ID";

CGalconServer::CGalconServer(PortNumber portNumber, QWidget *parent) :
   QDialog(parent)
   ,ui(new Ui::CGalconServer)   
   , m_currentClientID(0)
{
   ui->setupUi(this);

   m_tcpServer = new QTcpServer(this);
   if (!m_tcpServer->listen(QHostAddress::Any, portNumber))
   {
      QMessageBox::critical(0,
      "Server Error",
      "Unable to start the server:"
      + m_tcpServer->errorString()
   );
   m_tcpServer->close();
   return;
   }
   connect(m_tcpServer, SIGNAL(newConnection()),
      this,SLOT(slotNewConnection()));
}

CGalconServer::~CGalconServer()
{
    delete ui;
}

void CGalconServer::slotNewConnection()
{
   QTcpSocket* pClientSocket = m_tcpServer->nextPendingConnection();
   pClientSocket->setProperty(socketID, ++m_currentClientID);
   m_connectedSockets.insert(SocketIDPair(m_currentClientID, pClientSocket));

   connect(pClientSocket,SIGNAL(disconnected()), this, SLOT(removeSocket()));
   connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));
   connect(pClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
}

void CGalconServer::slotReadClient()
{
   QTcpSocket* pClientSocket = (QTcpSocket*)sender();
   QDataStream in(pClientSocket);
   in.setVersion(QDataStream::Qt_4_7);
   qint16 nextBlockSize = 0;
   for (;;)
   {
      if (pClientSocket->bytesAvailable() < sizeof(quint16))
      {
         break;
      }
      in >> nextBlockSize;

      if (pClientSocket->bytesAvailable() < nextBlockSize)
      {
         break;
      }

      QByteArray str;
      in >> str;

      ui->tEClient->append(QString(str));

      nextBlockSize = 0;
   }
}

void CGalconServer::sendToClient( QTcpSocket* pSocket, const QString& str )
{
   QByteArray  arrBlock;
   QDataStream out(&arrBlock, QIODevice::WriteOnly);
   out.setVersion(QDataStream::Qt_4_7);
   out << quint16(0) << str.toUtf8();

   out.device()->seek(0);
   out << quint16(arrBlock.size() - sizeof(quint16));

   pSocket->write(arrBlock);
}

void CGalconServer::removeSocket()
{
   QTcpSocket* pClientSocket = (QTcpSocket*)sender();
   SocketID ID = pClientSocket->property(socketID).toUInt();
   m_connectedSockets.erase(ID);
}

void CGalconServer::sendToConnectedClients( const QString& str )
{
   for (SocketsMap::iterator it = m_connectedSockets.begin(); it != m_connectedSockets.end(); ++it)
   {
      sendToClient(it->second, str);
   }
}

void CGalconServer::on_pBSC_CONNID_clicked()
{
   m_mesStr.clear();
   m_mesStr = "SC_CONNID#1##";
   ui->lESend->setText(m_mesStr);
}

void CGalconServer::on_pBSC_TIMETOSTART_clicked()
{
   m_mesStr.clear();
   m_mesStr = "SC_TIMETOSTART#9##";
   ui->lESend->setText(m_mesStr);
}

void CGalconServer::on_pBSC_START_clicked()
{
   m_mesStr.clear();
   m_mesStr.append("SC_START#500#400#2#25#(1,1,120,140,12,20)(2,0,40,200,10,15)(3,1,250,70,45,8)");
   m_mesStr.append("(4,2,280,260,12,20)#(1,Red Fox)(2,Star_123456)##");
   ui->lESend->setText(m_mesStr);
}

void CGalconServer::on_pBSC_STATE_clicked()
{
   m_mesStr.clear();
   m_mesStr = "SC_STATE#(1,1,20)(2,0,15)(3,0,8)(4,4,8)#(1,2,4,2,12,0)(2,2,3,2,24,0)##";
   ui->lESend->setText(m_mesStr);
}

void CGalconServer::on_pBSC_FINISH_clicked()
{
   m_mesStr.clear();
   m_mesStr = "SC_FINISH#2##";
   ui->lESend->setText(m_mesStr);
}

void CGalconServer::on_pBSC_ERR_clicked()
{
   m_mesStr.clear();
   m_mesStr = "SC_ERR#Player1 disconnected!##";
   ui->lESend->setText(m_mesStr);
}

void CGalconServer::on_pBSend_clicked()
{
   sendToConnectedClients(ui->lESend->text());
}
