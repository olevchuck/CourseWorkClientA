#include <stdlib.h>
#include <map>

#include "Bot.h"

namespace SingleGame
{   

   CBot::CBot()
   {
   }

   CBot::~CBot()
   {
   }

   void CBot::CreateBot(
      CPlayer* botPlayer,
      CMapGame* mapGame,
      const unsigned int level)
   {
      m_player = botPlayer;
      m_mapGame = mapGame;
      m_level = level;
   }

   bool CBot::HasPlanets()
   {
      return !m_player->m_vPlanet.empty();
   }

   const Message::CMessageStepPlayerPtr CBot::StepBot()
   {
      Message::CMessageStepPlayerPtr ptr(
               new Message::CMessageStepPlayer);      

      /// start planets id
      size_t countStartPlanet = m_player->m_vPlanet.size();
      countStartPlanet = (countStartPlanet * (rand() % (m_level * 10))) / 100;

      if(!countStartPlanet)
      {
         countStartPlanet = 1;
      }

      foreach(CPlanet* planet, m_player->m_vPlanet)
      {
         if((rand() % 2) && countStartPlanet)
         {
            ptr->m_startPlanetID.push_back(planet->GetID());
            --countStartPlanet;
         }
      }

      /// finish planet id
      if(ptr->m_startPlanetID.empty())
      {
         ptr->m_startPlanetID.push_back((*m_player->m_vPlanet.begin())->GetID());
      }

      /// percent
      ptr->m_finishPlanetID =
         m_mapGame->GetPlanets()[rand() % m_mapGame->GetPlanets().size()].GetID();

      unsigned int minPercent = (m_level - 1) ? (m_level - 1) * 10 : 1;
      if(minPercent > 50)
      {
         minPercent = 50;
      }
      ptr->m_percent = minPercent + rand() % (m_level * 10 - minPercent);

      return ptr;
   }

} // namespace SingleGame
