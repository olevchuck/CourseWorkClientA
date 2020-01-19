/// @file Game.cpp
/// @author Dmitriy Kozhevnikov
/// @date 06-06-2011

#include <QDebug>

#include "Game.h"
#include "Space.h"

namespace Game
{
   CGame::CGame(std::vector<CPlayer *> *players) :
      m_players(players),
      m_paused(false)
   {
      m_timer = new QTimer(this);
      connect(m_timer, SIGNAL(timeout()), this, SIGNAL(signalTimer()));
   }

   void CGame::AddView(Message::CMessageAddViewPtr mes)
   {
      m_view = mes->m_view;
      m_view->OnUpdate(m_space->GetPlanets(), m_space->GetFleets());
      m_timer->start(m_timeTick);
   }

   void CGame::DeleteGame()
   {
      m_timer->stop();

      delete m_space;
   }

   void CGame::slotTimer()
   {
      m_space->Update(1.0*(double)m_timeTick/1000);
      m_view->OnUpdate(m_space->GetPlanets(), m_space->GetFleets());
   }

   void CGame::SlotFinishGame(Message::CMessageFinishGamePtr mess)
   {
      if (AvailiblePlayer(mess->m_playerID))
      {
         DeleteGame();
         emit SendFinishGame(mess);
      }
   }

   void CGame::SlotStartData(Message::CMessageStartMapGamePtr data)
   {
      bool flagRightData = true;
      m_paused = false;
      QString errorMessage;
      /// Check start data

      if ((data->m_mapX > 1000) ||
            (data->m_mapY > 800) ||
            (data->m_flyV > 1000) ||
            (data->m_growV > 50))
      {
         flagRightData = false;
         errorMessage = "Fail map init";

      }
      /// Add players
      Message::CPlayerStartData currPlayer;
      foreach (currPlayer, data->m_playerData)
      {
         if (currPlayer.m_playerID >= 1 && currPlayer.m_playerID <= 8)
         {
            CPlayer* player = new CPlayer(currPlayer.m_playerID,
                                          currPlayer.m_playerName);
            m_players->push_back(player);
         }
         else
         {
            flagRightData = false;
            errorMessage = "Failed player " + QString().setNum(currPlayer.m_playerID).toUtf8();
         }
      }
      /// Add neutral player
      CPlayer* player = new CPlayer(0, "Neutral");
      m_players->push_back(player);

      /// Check planets
      Message::CPlanetStartData currPlanet;
      foreach (currPlanet, data->m_planetData)
      {
         if (!AvailiblePlayer(currPlanet.m_playerID) && (currPlanet.m_playerID != 0))
         {
            flagRightData = false;
            errorMessage = "Failed planet owner in planet " +
                  QString().setNum(currPlanet.m_planetID).toUtf8() + " has player " +
                  QString().setNum(currPlayer.m_playerID).toUtf8();
         }
      }

      /// Add planets

      if (flagRightData)
      {
         m_space = new CSpace(data->m_flyV,
                              data->m_growV,
                              data->m_mapX,
                              data->m_mapY,
                              data->m_planetData,
                              m_players);
         emit SendStartGame(data->m_mapX, data->m_mapY);
      }
      else
      {
         Message::CMessageInformationPtr mess(new Message::CMessageInformation);
         mess->m_typeInformation = Message::CMessageInformation::eGameError;
         mess->m_strInformation = errorMessage.toStdString();
         emit SendError(mess);
      }
   }

   void CGame::SlotStateMap(Message::CMessageStateMapPtr data)
   {
      if (CheckStatePlanet(data))
      {
         m_space->SetPlanets(data->m_planetState);
         if (CheckStateFleet(data))
         {
            m_space->SetFleets(data->m_fleetState);
            m_view->OnUpdate(m_space->GetPlanets(), m_space->GetFleets());
         }
         else
         {
            Message::CMessageInformationPtr mess(new Message::CMessageInformation);
            mess->m_typeInformation = Message::CMessageInformation::eGameError;
            mess->m_strInformation = "Wrong state data";
            emit SendError(mess);
         }
      }
      else
      {
         Message::CMessageInformationPtr mess(new Message::CMessageInformation);
         mess->m_typeInformation = Message::CMessageInformation::eGameError;
         mess->m_strInformation = "Wrong state data";
         emit SendError(mess);
      }

   }

   void CGame::SlotPause()
   {
      if (!m_paused)
      {
         m_timer->stop();
      }
      else
      {
         m_timer->start();
      }
      m_paused = !m_paused;
   }

   void CGame::SlotExitGame()
   {
      DeleteGame();
   }

   inline bool CGame::AvailiblePlayer(unsigned int id) const
   {
      CPlayer* currPl;
      foreach (currPl, *m_players)
      {
         if (currPl->GetId() == id)
         {
            return true;
         }
      }
      return false;
   }

   bool CGame::CheckStatePlanet(Message::CMessageStateMapPtr mess) const
   {
      Message::CStatePlanet currPlanet;
      foreach (currPlanet, mess->m_planetState)
      {
         if (!AvailiblePlayer(currPlanet.m_playerID) && (currPlanet.m_playerID != 0))
         {
            return false;
         }
         if (!m_space->CheckPlanetId(currPlanet.m_planetID))
         {
            return false;
         }
      }
      return true;
   }
   bool CGame::CheckStateFleet(Message::CMessageStateMapPtr mess) const
   {
      Message::CStateFleet currFleet;
      foreach (currFleet, mess->m_fleetState)
      {
         /// Check existence of Start planet, End planet
         /// and for equivalence of ids of fleet owner and start planet owner

         if (!m_space->CheckPlanetId(currFleet.m_planetStartID)
               || !m_space->CheckPlanetId(currFleet.m_planetFinishID)
               || (currFleet.m_playerID == 0))
         {
            return false;
         }
      }
      return true;
   }
} // Namespace Game
