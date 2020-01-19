/// @file Space.cpp
/// @author Dmitriy Kozhevnikov
/// @date 06-06-2011

#include "Planet.h"
#include "Fleet.h"
#include "Space.h"


namespace Game
{
   CSpace::CSpace(unsigned short flySpeed, unsigned short growSpeed, unsigned short xSize, unsigned short ySize,
                  std::vector<Message::CPlanetStartData> planets, std::vector<CPlayer*>* players):
      m_players(players),
      m_speed(flySpeed),
      m_growth(growSpeed),
      m_width(xSize),
      m_height(ySize)
   {
      CPlayer* currPlayer;
      Message::CPlanetStartData currPlData;
      foreach (currPlData, planets)
      {
         currPlayer = GetPlayerById(currPlData.m_playerID);
         CPlanet* tempPlanet = new CPlanet(currPlData.m_planetID,
                                           currPlData.m_planetX,
                                           currPlData.m_planetY,
                                           currPlData.m_planetR,
                                           currPlData.m_countFleet,
                                           currPlayer);
         m_planets.push_back(tempPlanet);
      }
   }

   CSpace::~CSpace()
   {
      while (!m_fleets.empty())
      {
         CFleet* fl = m_fleets.back();
         m_fleets.pop_back();
         delete fl;
      }

      while (!m_planets.empty())
      {
         CPlanet* pl = m_planets.back();
         m_planets.pop_back();
         delete pl;
      }
   }

   void CSpace::Update(const double i_time)
   {
      double plGrowth = i_time * (double)m_growth;
      double flMove = i_time * (double)m_speed;
      /// Null army on every player

      CPlayer* currPlayer;
      foreach (currPlayer, *m_players)
      {
         currPlayer->NullArmy();
      }

      ///  Make growth on planets
      CPlanet* currPlanet;
      foreach (currPlanet, m_planets)
      {
         if (currPlanet->GetPlayerId() != 0) ///< Exclude growth on neutral planets
         {
            currPlanet->UpdateArmy(plGrowth * currPlanet->GetGrowth());
         }
         currPlanet->GetPlayer()->SetArmy(currPlanet->GetArmy());
      }

      /// Move fleets
      CFleet* currFleet;
      bool ReachedFleetsPresent(true);
      foreach (currFleet, m_fleets)
      {
         currFleet->IncreaseWay(flMove);
      }

      /// Delete reached fleets
      while (ReachedFleetsPresent)
      {
         std::list<CFleet*>::iterator iter;
         for(iter = m_fleets.begin(); iter != m_fleets.end(); ++iter)
         {
            if (iter.operator *()->ReachedDestination())
            {
               break;
            }
         }
         if (iter != m_fleets.end())
         {
            CFleet* toDelete = iter.operator *();
            m_fleets.erase(iter);
            delete toDelete;
         }
         else
         {
            ReachedFleetsPresent = false;
         }
      }

   }

   void CSpace::SetPlanets(const std::vector<Message::CStatePlanet>& i_planets)
   {
      /// For every existing planet update its army & owner from given data
      Message::CStatePlanet inPl;
      bool NewFlag = false;
      foreach (inPl, i_planets)
      {
         CPlanet* currPl = 0;
         foreach (currPl, m_planets)
         {
            if (inPl.m_planetID == currPl->GetId())
            {
               NewFlag = true;
               break;
            }
         }
         if (NewFlag)
         {
            currPl->SetArmy(inPl.m_countFleet);
            CPlayer* tempPlayer = GetPlayerById(inPl.m_playerID);
            currPl->SetPlayer(tempPlayer);
         }
      }
   }

   void CSpace::SetFleets(const std::vector<Message::CStateFleet>& message)
   {
      /// For every given fleet
      Message::CStateFleet currMessFleet;
      foreach (currMessFleet, message)
      {
         CFleet* currFleet;
         bool flag = true;

         /// Search for it in existing list
         foreach (currFleet, m_fleets)
         {
            if (currFleet->GetId() == currMessFleet.m_fleetID)
            {
               //currFleet->SetPercent(currMessFleet.m_percentRoute);
               flag = false;
            }
         }

         /// If didn't found - add it to list
         if (flag)
         {
            CPlayer* currPlayer = GetPlayerById(currMessFleet.m_playerID);
            CPlanet* planetFr = GetPlanetById(currMessFleet.m_planetStartID);
            CPlanet* planetTo = GetPlanetById(currMessFleet.m_planetFinishID);
            CFleet* newFleet = new CFleet(currMessFleet.m_fleetID,
                                    planetFr,
                                    planetTo,
                                    currPlayer,
                                    currMessFleet.m_countFleet,
                                    currMessFleet.m_percentRoute);
            m_fleets.push_front(newFleet);
         }
      }
   }

   std::list<CFleet*> CSpace::GetFleets() const
   {
      return m_fleets;
   }

   std::vector<CPlanet*> CSpace::GetPlanets() const
   {
      return m_planets;
   }

   CPlanet* CSpace::GetPlanetById(const unsigned short id) const
   {
      CPlanet* planet;
      foreach (planet, m_planets)
      {
         if (planet->GetId() == id)
            return planet;
      }
      return 0;
   }

   bool CSpace::CheckPlanetId(const unsigned int id) const
   {
      CPlanet* currPl;
      foreach (currPl, m_planets)
      {
         if (currPl->GetId() == id)
         {
            return true;
         }
      }
      return false;
   }

   CPlayer* CSpace::GetPlayerById(const unsigned int id) const
   {
      CPlayer* currPlayer;
      foreach (currPlayer, *m_players)
      {
         if (currPlayer->GetId() == id)
         {
            return currPlayer;
         }
      }
      return 0;
   }
} //Namespace Game
