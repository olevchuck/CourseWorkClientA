#include <QDebug>

#include <QString>
#include <QDataStream>

#include "ServerManager.h"

namespace ServerManagerDecl
{
/// constructor
   CServerManager::CServerManager():
      m_timerConfirm(this)
   {
      m_connectToServer = false;
      m_ePhaseMessage = CParser::eUnknown;
      m_timerConfirm.setInterval(100000);
      connect(&m_timerConfirm, SIGNAL(timeout()), this, SLOT(slotTimeStartOut()));
   }

   CServerManager::~CServerManager()
   {
   }

/// private function
   void CServerManager::connectToServer(const std::string& serverIP,
      const unsigned int m_serverPort)
   {
      m_tcpSocket = new QTcpSocket(this);

      m_tcpSocket->connectToHost(QString(serverIP.c_str()), m_serverPort);
      connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
      connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
      connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
         this,SLOT(slotError(QAbstractSocket::SocketError)));      
   }

   void CServerManager::disconnectFromServer()
   {
      m_connectToServer = false;
      m_tcpSocket->waitForDisconnected(0);
      m_tcpSocket->disconnectFromHost();

      disconnect(m_tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
      disconnect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
      disconnect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
         this,SLOT(slotError(QAbstractSocket::SocketError)));

      delete m_tcpSocket;

      qDebug("Disconnect from server - end");
   }

   void CServerManager::sendToServer(const Message::IMessagePtr pMessage)
   {
      QByteArray  arrBlock;
      QDataStream out(&arrBlock, QIODevice::WriteOnly);
      out.setVersion(QDataStream::Qt_4_7);
      out << quint16(0) << QString(pMessage->toString().c_str()).toUtf8();

      out.device()->seek(0);
      out << quint16(arrBlock.size() - sizeof(quint16));

      m_tcpSocket->write(arrBlock);
   }

   void CServerManager::parseStrFromServer(const std::string& sMes)
   {
      try
      {
         switch(m_parser.CheckTypeMessage(sMes))
         {
         case CParser::eConfirmConnect :
            if(m_ePhaseMessage == CParser::eUnknown)
            {
               m_timerConfirm.stop();
               m_ePhaseMessage = CParser::eConfirmConnect;
               emit SendConfirmConnect(m_parser.ParseMConfirmConnect(sMes));
            }
            else
            {
               throw std::string("Error, Server send error message");
            }
            break;
         case CParser::eError :
            emit SendError(m_parser.ParseMError(sMes));
            break;
         case CParser::eFinishGame :
            if(m_ePhaseMessage == CParser::eStartGame)
            {
               disconnectFromServer();
               m_ePhaseMessage = CParser::eUnknown;
               emit SendFinishGame(m_parser.ParseMFinishGame(sMes));
            }
            else
            {
               throw std::string("Error, Server send error message");
            }
            break;
         case CParser::eStartGame :
            if(m_ePhaseMessage == CParser::eTimeToStart)
            {
               m_ePhaseMessage = CParser::eStartGame;
               emit SendStartGame(m_parser.ParseMStartMapGame(sMes));
            }
            else
            {
               throw std::string("Error, Server send error message");
            }
            break;
         case CParser::eStateMap :
            if(m_ePhaseMessage == CParser::eStartGame)
            {
               emit SendStateMap(m_parser.ParseMStateMap(sMes));
            }
            else
            {
               throw std::string("Error, Server send error message");
            }
            break;
         case CParser::eTimeToStart :
            if(m_ePhaseMessage == CParser::eConfirmConnect)
            {
               m_ePhaseMessage = CParser::eTimeToStart;
               emit SendTimeToStart(m_parser.ParseMTimeToStartGame(sMes));
            }
            else
            {
               throw std::string("Error, Server send error message");
            }
            break;
         case CParser::eUnknown :
            throw std::string("Error, Server send unknown message");
            break;
         }
      }
      catch(std::string str)
      {
         Message::CMessageInformationPtr ptr(new Message::CMessageInformation);
         ptr->m_typeInformation = Message::CMessageInformation::eMessageFromServer;
         ptr->m_strInformation = str;
         emit SendInInformation(ptr);
      }
   }

/// slots connect to server
   void CServerManager::slotConnected()
   {
      m_connectToServer = true;

      Message::CMessageInformationPtr ptr(new Message::CMessageInformation);
      ptr->m_typeInformation = Message::CMessageInformation::eConnectionToServer;
      ptr->m_strInformation = "Connected";
      emit SendInInformation(ptr);
   }

   void CServerManager::slotReadyRead()
   {
      QDataStream in(m_tcpSocket);
      in.setVersion(QDataStream::Qt_4_7);
      qint16 nextBlockSize = 0;
      for (;;)
      {
          if (m_tcpSocket->bytesAvailable() < sizeof(quint16))
          {
              break;
          }

          in >> nextBlockSize;

          if (m_tcpSocket->bytesAvailable() < nextBlockSize)
          {
              break;
          }

          QByteArray str;
          in >> str;

          nextBlockSize = 0;

          Message::CMessageInformationPtr ptr(new Message::CMessageInformation);
          ptr->m_typeInformation = Message::CMessageInformation::eMessageFromServer;
          ptr->m_strInformation = QString(str).toStdString();
          emit SendInInformation(ptr);

          qDebug(QString(str).toUtf8());

          parseStrFromServer(QString(str).toStdString());
      }
   }

   void CServerManager::slotError(QAbstractSocket::SocketError)
   {
      m_connectToServer = false;

      Message::CMessageErrorPtr ptr(
               new Message::CMessageError);
      ptr->m_strError = "Error connection";
      emit SendError(ptr);
   }

   void CServerManager::slotTimeStartOut()
   {
      qDebug("Network - stop timer");

      m_timerConfirm.stop();

      Message::CMessageInformationPtr ptr(new Message::CMessageInformation);
      ptr->m_typeInformation = Message::CMessageInformation::eConnectionToServer;
      ptr->m_strInformation = "Error, time wait confirmation from server - out";

      emit SendInInformation(ptr);

      disconnectFromServer();
   }

/// public slots
   void CServerManager::TakeServerConnect(const Message::CMessageConnectToServerPtr pMessage)
   {
      if(!m_connectToServer)
      {
         connectToServer(pMessage->m_serverIP, pMessage->m_serverPort);
         sendToServer(pMessage);

         qDebug("Network - start wait confirm timer");

         m_timerConfirm.start();
      }
      else
      {
         Message::CMessageInformationPtr ptr(new Message::CMessageInformation);
         ptr->m_typeInformation = Message::CMessageInformation::eConnectionToServer;
         ptr->m_strInformation = "Error, You again trying connect to server";
         emit SendInInformation(ptr);
      }
   }

   void CServerManager::TakeStepPlayer(const Message::CMessageStepPlayerPtr pMessage)
   {
      if(m_connectToServer)
      {
         sendToServer(pMessage);
      }
      else
      {
         Message::CMessageInformationPtr ptr(new Message::CMessageInformation);
         ptr->m_typeInformation = Message::CMessageInformation::eConnectionToServer;
         ptr->m_strInformation = "Error, No connection to server";
         emit SendInInformation(ptr);
      }
   }

   void CServerManager::TakeExit()
   {
      disconnectFromServer();
   }

} // namespace ServerManagerDecl
