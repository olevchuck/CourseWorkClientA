#pragma once

#include "SingleGame/GameMapObject.h"
#include "message/MessageStepPlayer.h"
#include "MapGame.h"

namespace SingleGame
{

   class CBot
   {

   public:
      CBot();
      ~CBot();

      void CreateBot(
         CPlayer* botPlayer,
         CMapGame* mapGame,
         const unsigned int level);

      const Message::CMessageStepPlayerPtr StepBot();
      bool HasPlanets();

   private:      

      CPlayer* m_player;
      CMapGame* m_mapGame;

      unsigned int m_level;

   }; // class CBot

} // namespace SingleGame
