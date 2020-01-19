#pragma once

#include <string>

#include <QObject>
#include <QTcpSocket>

#include "message/MessageItf.h"
#include "message/MessageConnectToServer.h"
#include "message/MessageStepPlayer.h"
#include "message/MessageError.h"
#include "message/MessageFinishGame.h"
#include "message/MessageStateMap.h"
#include "message/MessageStartMapGame.h"
#include "message/MessageTimeToStartGame.h"
#include "message/MessageInformation.h"
#include "Parser.h"

namespace ServerManagerDecl
{
   class CServerManager : public QObject
   {
      Q_OBJECT

   public:
      CServerManager();
      ~CServerManager();      

   public slots:
      void TakeServerConnect(const Message::CMessageConnectToServerPtr pMessage);
      void TakeStepPlayer(const Message::CMessageStepPlayerPtr pMessage);

   signals:
      void SendConfirmConnect(const Message::CMessageConfirmationConnectToServerPtr pMessage);
      void SendError(const Message::CMessageErrorPtr pMessage);
      void SendFinishGame(const Message::CMessageFinishGamePtr pMessage);
      void SendStateMap(const Message::CMessageStateMapPtr pMessage);
      void SendStartGame(const Message::CMessageStartMapGamePtr pMessage);
      void SendTimeToStart(const Message::CMessageTimeToStartGamePtr pMessage);

      void SendInInformation(const Message::CMessageInformationPtr pMessage);

   private slots:
      void slotConnected();
      void slotReadyRead();
      void slotError(QAbstractSocket::SocketError);

   private:
      void connectToServer(const std::string& serverIP, const unsigned int m_serverPort);
      void sendToServer(const Message::IMessagePtr pMessage);
      void parseStrFromServer(const std::string& sMes);

      QTcpSocket* m_tcpSocket;

      CParser m_parser;

      /// flags
      bool m_connectToServer;

   }; // class CServerManager
} // namespace ServerManagerDecl
