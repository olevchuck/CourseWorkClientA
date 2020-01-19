#include <QVariant>

#include "SingleGameManager.h"

namespace SingleGame
{
/// ---------------- constructor
   CSingleGameManager::CSingleGameManager() :
      m_timerWaitStart(this),
      m_timerRunTime(this),
      m_timerBot(this),
      m_pause(false)
   {
      m_timerWaitStart.setInterval(1000);
      m_timerRunTime.setInterval(2000);
      connect(&m_timerWaitStart, SIGNAL(timeout()), this, SLOT(slotWaitTime()));
      connect(&m_timerRunTime, SIGNAL(timeout()), this, SLOT(slotRunTime()));
      connect(&m_timerBot, SIGNAL(timeout()), this, SLOT(slotStepBot()));
   }

   CSingleGameManager::~CSingleGameManager()
   {
   }

/// -------------------------- publoc slots
   void CSingleGameManager::TakeServerConnect(const Message::CMessageConnectToSingleGamePtr pMessage)
   {      
      bool start = startGame(
               0, //< time to start
               pMessage->m_mapWidth,
               pMessage->m_mapHeigth,
               pMessage->m_flySpeed,
               pMessage->m_growSpeed,
               pMessage->m_namePlayer,
               pMessage->m_planetMinSize,
               pMessage->m_planetMaxSize,
               pMessage->m_dispersionPlanets,
               pMessage->m_fleetMinCount,
               pMessage->m_fleetMaxCount,
               pMessage->m_botNumber,
               pMessage->m_botLevel
               );
      if(!start)
      {
         clear();

         Message::CMessageInformationPtr ptr(
                  new Message::CMessageInformation);
         ptr->m_typeInformation = Message::CMessageInformation::eGameError;
         ptr->m_strInformation = "Error map setting";

         SendInInformation(ptr);
      }
   }

   void CSingleGameManager::TakeStepPlayer(const Message::CMessageStepPlayerPtr pMessage)
   {
      m_mapGame.UpdateStateMap(
               pMessage->m_finishPlanetID,
               pMessage->m_percent,
               pMessage->m_startPlanetID,
               QDateTime::currentMSecsSinceEpoch());
      if(!m_stepBot)
      {
         slotRunTime();
      }
   }

   void CSingleGameManager::slotStepBot()
   {
      if(m_mapGame.GetFleets().size() < sMaxCountFleets)
      {
         m_stepBot = true;
         foreach(CBot bot, m_vBot)
         {
            if(bot.HasPlanets())
            {
               TakeStepPlayer(bot.StepBot());
            }
         }

         m_stepBot = false;
         slotRunTime();
      }
   }

   void CSingleGameManager::TakePause()
   {
      qDebug("Single - pause");

      if(m_pause) //< game is stop
      {         
         pauseOFF();
      }
      else //< game is run
      {
         pauseON();
      }
   }

   void CSingleGameManager::TakeExit()
   {
      qDebug("Single - exit");

      clear();
   }

/// ----------------- generation start data
   bool CSingleGameManager::startGame(
      const unsigned int timeToStart,
      const unsigned int widthMap,
      const unsigned int heigthMap,
      const unsigned int flySpeed,
      const unsigned int growSpeed,
      const std::string& namePlayer,
      const unsigned int planetMinSize,
      const unsigned int planetMaxSize,
      const unsigned int dispersionPlanets,
      const unsigned int fleetMinCount,
      const unsigned int fleetMaxCount,
      const unsigned int botNumber,
      const unsigned int botLevel
      )
   {
      /// register player
      CPlayer tempPlayer;
      tempPlayer.m_name = namePlayer;
      tempPlayer.GenerationID();
      m_vPlayer.push_back(tempPlayer);      

      /// bots      
      createBot(botNumber, botLevel);

      /// generation play map
      m_mapGame.GenerationMap(
         widthMap,
         heigthMap,
         flySpeed,
         growSpeed,
         planetMinSize,
         planetMaxSize,
         dispersionPlanets,
         fleetMinCount,
         fleetMaxCount);

      if(m_mapGame.CountPlanets() < m_vPlayer.size())
      {
         return false;
      }

      /// send message with register player
      Message::CMessageConfirmationConnectToServerPtr ptr(
               new Message::CMessageConfirmationConnectToServer);

      ptr->m_playerID = tempPlayer.GetID();
      SendConfirmConnect(ptr);

      /// set planet's owner
      m_mapGame.SetPlayers(m_vPlayer);

      /// start timer Wait
      m_timeToStart = timeToStart;
      if(m_timeToStart)
      {
         slotWaitTime();
         m_timerWaitStart.start();
      }

      if(!m_timeToStart)
      {
         m_timerWaitStart.stop();
         runPlay();
      }

      return true;
   }

   void CSingleGameManager::clear()
   {
      qDebug("Clear single game");

      m_vPlayer.clear();
      m_timerWaitStart.stop();
      m_timerRunTime.stop();
      m_timerBot.stop();
      m_timeToStart = 0;
      m_vBot.clear();
      m_pause = false;

      m_mapGame.Clear();

      CGameObject::ClearID();
   }

/// --------------------------- timers
   void CSingleGameManager::slotWaitTime()
   {
      Message::CMessageTimeToStartGamePtr ptr(
               new Message::CMessageTimeToStartGame);

      ptr->m_second = m_timeToStart;
      SendTimeToStart(ptr);

      --m_timeToStart;

      if(!m_timeToStart)
      {
         m_timerWaitStart.stop();
         runPlay();
      }
   }

   void CSingleGameManager::slotRunTime()
   {
      qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
      m_mapGame.UpdateStateMap(currentTime);

      Message::CMessageStateMapPtr ptr(
               new Message::CMessageStateMap);

      Message::CStateFleet tempFleet;      
      foreach(CFleet fleet, m_mapGame.GetFleets())
      {
         tempFleet.m_countFleet = fleet.m_countFleet;
         tempFleet.m_fleetID = fleet.GetID();
         tempFleet.m_percentRoute = 100 *
               (currentTime
               - fleet.m_timeStartMove)
               / (fleet.m_timeFinishMove
               - fleet.m_timeStartMove);
         tempFleet.m_planetFinishID = fleet.m_toPlanet->GetID();
         tempFleet.m_planetStartID = fleet.m_fromPlanet->GetID();
         tempFleet.m_playerID = fleet.m_pPlayer->GetID();

         ptr->m_fleetState.push_back(tempFleet);
      }

      Message::CStatePlanet tempPlanet;
      foreach(CPlanet planet, m_mapGame.GetPlanets())
      {
         tempPlanet.m_countFleet = planet.m_countFleet;
         tempPlanet.m_planetID = planet.GetID();
         tempPlanet.m_playerID = planet.m_pPlayer->GetID();

         ptr->m_planetState.push_back(tempPlanet);
      }

      SendStateMap(ptr);
      checkEndGame();
   }

/// ------------------------- run play
   void CSingleGameManager::runPlay()
   {
      Message::CMessageStartMapGamePtr ptr(
               new Message::CMessageStartMapGame);

      ptr->m_flyV = m_mapGame.GetFlySpeed();
      ptr->m_growV = m_mapGame.GetGrowSpeed();
      ptr->m_mapX = m_mapGame.GetWidthMap();
      ptr->m_mapY = m_mapGame.GetHeigthMap();

      Message::CPlanetStartData tempPlanet;      
      foreach(CPlanet planet, m_mapGame.GetPlanets())
      {
         tempPlanet.m_countFleet = planet.m_countFleet;
         tempPlanet.m_planetID = planet.GetID();
         tempPlanet.m_planetR = planet.m_radius;
         tempPlanet.m_planetX = planet.m_coordinates.x;
         tempPlanet.m_planetY = planet.m_coordinates.y;
         tempPlanet.m_playerID = planet.m_pPlayer->GetID();

         ptr->m_planetData.push_back(tempPlanet);
      }

      Message::CPlayerStartData tempPlayer;      
      foreach(CPlayer player, m_vPlayer)
      {
         tempPlayer.m_playerID = player.GetID();
         tempPlayer.m_playerName = player.m_name;

         ptr->m_playerData.push_back(tempPlayer);
      }

      m_timerRunTime.start();

      SendStartGame(ptr);

      m_timerBot.start();
   }

   void CSingleGameManager::checkEndGame()
   {
      unsigned int countPlayers = 0;
      unsigned int winnId = 0;

      foreach(CPlayer player, m_vPlayer)
      {
         if(!player.Empty())
         {
            winnId = player.GetID();
            ++countPlayers;
         }
      }

      if(countPlayers <= 1)
      {
         m_timerBot.stop();
         m_timerRunTime.stop();

         Message::CMessageFinishGamePtr ptr(
                  new Message::CMessageFinishGame);

         ptr->m_playerID = winnId;

         clear();

         SendFinishGame(ptr);
      }
   }
/// -------------------------------- pause
   void CSingleGameManager::pauseON()
   {
      m_pause = true;
      m_timerRunTime.stop();
      m_timerBot.stop();
      slotRunTime();
   }

   void CSingleGameManager::pauseOFF()
   {
      m_pause = false;
      m_mapGame.SetCurrentTime(QDateTime::currentMSecsSinceEpoch());

      m_timerRunTime.start();
      m_timerBot.start();
   }

/// -------------------------------- bot
   void CSingleGameManager::createBot(
         const unsigned int botNumber,
         const unsigned int botLevel)
   {
      CBot tempBot;
      CPlayer tempPlayer;

      /// register bots
      for(unsigned int i = 0; i < botNumber; ++i)
      {
         tempPlayer.m_name = "Bot" + QVariant(i + 1).toString().toStdString();
         tempPlayer.GenerationID();
         m_vPlayer.push_back(tempPlayer);
      }

      std::vector<CPlayer>::iterator itB = m_vPlayer.begin();
      std::vector<CPlayer>::iterator itE = m_vPlayer.end();
      for(++itB; itB != itE; ++itB)
      {
         tempBot.CreateBot(&(*itB), &m_mapGame, botLevel);
         m_vBot.push_back(tempBot);
      }

      m_timerBot.setInterval((10 - botLevel + 1) * 1000);
   }

} // namespace SingleGame
