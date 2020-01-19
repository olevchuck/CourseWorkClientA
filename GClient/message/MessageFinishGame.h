#pragma once

#include <QSharedPointer>

namespace Message
{
   struct CMessageFinishGame;

   typedef QSharedPointer<CMessageFinishGame> CMessageFinishGamePtr;

   struct CMessageFinishGame
   {
      unsigned int m_playerID;   

   }; // struct CMessageFinishGame
} // namespace Message
