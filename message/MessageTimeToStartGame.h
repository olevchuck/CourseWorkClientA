#pragma once

#include <QSharedPointer>

namespace Message
{
   struct CMessageTimeToStartGame;

   typedef QSharedPointer<CMessageTimeToStartGame> CMessageTimeToStartGamePtr;

   struct CMessageTimeToStartGame
   {
      unsigned int m_second;

   }; // struct CMessaheTimeToStartGame

}; // namespace Message
