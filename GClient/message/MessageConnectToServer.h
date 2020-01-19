#pragma once

#include <string>
#include <QSharedPointer>

#include "MessageItf.h"

namespace Message
{
   struct CMessageConnectToServer;
   struct CMessageConfirmationConnectToServer;

   typedef QSharedPointer<CMessageConnectToServer> CMessageConnectToServerPtr;
   typedef QSharedPointer<CMessageConfirmationConnectToServer>
      CMessageConfirmationConnectToServerPtr;

   struct CMessageConnectToServer : public IMessage
   {
      std::string m_namePlayer;
      std::string m_serverIP;
      unsigned int m_serverPort;

   protected:

      virtual std::string toString() const
      {
         if(m_namePlayer.empty())
         {
            return "CS_CONN##";
         }

         return "CS_CONN#" + m_namePlayer + "##";
      }

   }; // struct CMessageConnectToServer


   struct CMessageConfirmationConnectToServer
   {
      unsigned int m_playerID;   

   }; // struct CMessageConfirmationConnectToServer
} // namespace Message
