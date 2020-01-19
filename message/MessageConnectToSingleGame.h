#pragma once

#include <QSharedPointer>
#include <string>

namespace Message
{
   struct CMessageConnectToSingleGame;

   typedef QSharedPointer<CMessageConnectToSingleGame> CMessageConnectToSingleGamePtr;

   struct CMessageConnectToSingleGame
   {
      std::string m_namePlayer;

      unsigned int m_mapWidth;
      unsigned int m_mapHeigth;

      unsigned int m_flySpeed;
      unsigned int m_growSpeed;

      unsigned int m_planetMinSize;
      unsigned int m_planetMaxSize;

      unsigned int m_dispersionPlanets;

      unsigned int m_fleetMinCount;
      unsigned int m_fleetMaxCount;

      unsigned int m_botNumber;
      unsigned int m_botLevel;

   }; // struct CMessageConnectToSingleGame

} // namespace Message
