#include <stdlib.h>
#include <algorithm>
#include <time.h>

#include "MapGame.h"

namespace SingleGame
{

   CMapGame::CMapGame()
   {
      m_neutralPlayer.SetNeutral();
   }

   CMapGame::~CMapGame()
   {
   }

/// --------------------- generation start data
   void CMapGame::GenerationMap(
      const unsigned int widthMap,
      const unsigned int heigthMap,
      const unsigned int flySpeed,
      const unsigned int growSpeed,
      const unsigned int planetMinSize,
      const unsigned int planetMaxSize,
      const unsigned int dispersionPlanets,
      const unsigned int fleetMinCount,
      const unsigned int fleetMaxCount)
   {
      m_widthMap = widthMap;
      m_heigthMap = heigthMap;
      m_flySpeed = flySpeed;
      m_growSpeed = growSpeed;

      m_cMaxPlanetRadius = planetMaxSize;
      m_cMinPlanetRadius = planetMinSize;
      m_cCoefDispersionPlanets = dispersionPlanets;
      m_cMinFleetCount = fleetMinCount;
      m_cMaxFleetCount = fleetMaxCount;

      createPlanet();
   }

   void CMapGame::createPlanet()
   {
      unsigned int countPlanetX = m_widthMap
            / (m_cCoefDispersionPlanets * m_cMaxPlanetRadius) - 1;
      unsigned int countPlanetY = m_heigthMap
            / (m_cCoefDispersionPlanets * m_cMaxPlanetRadius) - 1;
      unsigned int countPlanet = countPlanetX * countPlanetY;

      qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

      srand(time(NULL));

      CPlanet tempPlanet;
      for(unsigned int i = 0; i < countPlanet; ++i)
      {
         tempPlanet.GenerationID();
         tempPlanet.m_radius = m_cMinPlanetRadius
               + rand() % (m_cMaxPlanetRadius - m_cMinPlanetRadius);
         tempPlanet.m_coordinates.x = m_cMaxPlanetRadius
               * m_cCoefDispersionPlanets
               * (1 + (i % countPlanetX))
               + (rand() % 2 ? -1 : 1)
               * (rand() % (m_cMaxPlanetRadius * (m_cCoefDispersionPlanets -1) / 2));
         tempPlanet.m_coordinates.y = m_cMaxPlanetRadius
               *  m_cCoefDispersionPlanets
               * (1 + (i / countPlanetX))
               + (rand() % 2 ? -1 : 1)
               * (rand() % (m_cMaxPlanetRadius * (m_cCoefDispersionPlanets -1) / 2));
         tempPlanet.m_countFleet = m_cMinFleetCount
               + rand() % (m_cMaxFleetCount - m_cMinFleetCount);
         tempPlanet.m_pPlayer = &m_neutralPlayer;
         tempPlanet.m_timeLastUpdate = currentTime;

         m_vPlanet.push_back(tempPlanet);
      }
   }

   unsigned int CMapGame::CountPlanets()
   {
      return m_vPlanet.size();
   }

   void CMapGame::SetPlayers(std::vector<CPlayer>& vPlayer)
   {
      std::vector<CPlayer>::iterator itB = vPlayer.begin();
      std::vector<CPlayer>::iterator itE = vPlayer.end();

      for(int index = 0; itB != itE;)
      {
         index = rand() % m_vPlanet.size();
         if(!m_vPlanet[index].m_pPlayer->GetID())
         {
            m_vPlanet[index].m_pPlayer = &(*itB);
            itB->AddPlanet(&m_vPlanet[index]);
            ++itB;
         }
      }
   }

   void CMapGame::Clear()
   {
      m_widthMap = 0;
      m_heigthMap = 0;
      m_flySpeed = 0;
      m_growSpeed = 0;

      m_cMaxPlanetRadius = 0;
      m_cMinPlanetRadius = 0;
      m_cCoefDispersionPlanets = 0;
      m_cMinFleetCount = 0;
      m_cMaxFleetCount = 0;

      m_vPlanet.clear();
      m_vFleet.clear();
   }

/// --------------------------- get data
   const std::vector<CPlanet>& CMapGame::GetPlanets() const
   {
      return m_vPlanet;
   }

   const std::list<CFleet>& CMapGame::GetFleets() const
   {
      return m_vFleet;
   }

   const unsigned int CMapGame::GetWidthMap() const
   {
      return m_widthMap;
   }

   const unsigned int CMapGame::GetHeigthMap() const
   {
      return m_heigthMap;
   }

   const unsigned int CMapGame::GetFlySpeed() const
   {
      return m_flySpeed;
   }

   const unsigned int CMapGame::GetGrowSpeed() const
   {
      return m_growSpeed;
   }

/// --------------------------- update data
   void CMapGame::UpdateStateMap(const qint64 currentTime)
   {
      updateFleet(currentTime);
      updatePlanet(currentTime);
   }

   void CMapGame::UpdateStateMap(
      const unsigned int finishPlanetID,
      const unsigned int percent,
      const std::vector<unsigned int>& startPlanetID,
      const qint64 currentTime)
   {
      UpdateStateMap(currentTime);
      addFleet(finishPlanetID, percent, startPlanetID, currentTime);
   }

   void CMapGame::updateFleet(const qint64 currentTime)
   {
      qint64 timeFinish;

      std::list<CFleet>::iterator itB = m_vFleet.begin();
      std::list<CFleet>::iterator itE = m_vFleet.end();

      for(; itB != itE;)
      {
         timeFinish = itB->m_timeFinishMove;

         if(itB->m_timeFinishMove < currentTime)
         {
            if(!itB->m_toPlanet->GetID())
            {
               itB->m_toPlanet->UpdatePlanet(timeFinish, m_growSpeed);
            }

            if(itB->m_toPlanet->m_pPlayer->GetID()
                  == itB->m_pPlayer->GetID())
            {
               /// go to owner planet
               itB->m_toPlanet->m_countFleet += itB->m_countFleet;
            }
            else if(itB->m_countFleet <= itB->m_toPlanet->m_countFleet)
            {
               /// no take planet
               itB->m_toPlanet->m_countFleet -= itB->m_countFleet;
            }
            else
            {
               /// take planet
               itB->m_toPlanet->m_countFleet = itB->m_countFleet
                  - itB->m_toPlanet->m_countFleet;
               if(itB->m_toPlanet->m_pPlayer->GetID() != 0)
               {
                  itB->m_toPlanet->m_pPlayer->DeletePlanet(itB->m_toPlanet->GetID());
               }
               itB->m_toPlanet->m_pPlayer = itB->m_pPlayer;
               itB->m_toPlanet->m_timeLastUpdate = timeFinish;
               itB->m_pPlayer->AddPlanet(&(*itB->m_toPlanet));
            }

            itB->m_pPlayer->DeleteFleet(itB->GetID());
            itB = m_vFleet.erase(itB);
         }
         else
         {
            break;
         }
      }
   }

   void CMapGame::updatePlanet(const qint64 time)
   {
      std::vector<CPlanet>::iterator itB = m_vPlanet.begin();
      std::vector<CPlanet>::iterator itE = m_vPlanet.end();
      for(; itB != itE; ++itB)
      {
         if(itB->m_pPlayer->GetID())
         {
            itB->UpdatePlanet(time, m_growSpeed);
         }
      }
   }

   void CMapGame::addFleet(
      const unsigned int finishPlanetID,
      const unsigned int percent,
      const std::vector<unsigned int>& startPlanetID,
      const qint64 currentTime)
   {
      CPlanet* finishPlanet = getPlanet(finishPlanetID);      

      std::list<CFleet>::iterator itInsertFleet;
      CFleet tempFleet;
      foreach(unsigned int id, startPlanetID)
      {
         if(finishPlanetID == id)
         {
            continue;
         }

         tempFleet.m_fromPlanet = getPlanet(id);
         tempFleet.m_countFleet = getPlanet(id)->m_countFleet * percent / 100;
         tempFleet.m_fromPlanet->m_countFleet -= tempFleet.m_countFleet;
         tempFleet.m_toPlanet = finishPlanet;
         tempFleet.GenerationID();
         tempFleet.m_timeStartMove = currentTime;
         tempFleet.SetTimeFinish(m_flySpeed);
         tempFleet.m_pPlayer = getPlanet(id)->m_pPlayer;

         itInsertFleet =
               m_vFleet.insert(findInsertFleetPosition(tempFleet), tempFleet);

         tempFleet.m_pPlayer->AddFleet(&(*itInsertFleet));
      }
   }

   CPlanet* CMapGame::getPlanet(const unsigned int ID)
   {
      std::vector<CPlanet>::iterator itB = m_vPlanet.begin();
      std::vector<CPlanet>::iterator itE = m_vPlanet.end();

      for(; itB != itE; ++itB)
      {
         if(itB->GetID() == ID)
         {
            return &(*itB);
         }
      }

      return NULL;
   }

   std::list<CFleet>::iterator CMapGame::findInsertFleetPosition(const CFleet& fleet)
   {
      std::list<CFleet>::iterator itB = m_vFleet.begin();
      std::list<CFleet>::iterator itE = m_vFleet.end();

      for(; itB != itE; ++itB)
      {
         if(itB->m_timeFinishMove > fleet.m_timeFinishMove)
         {
            return itB;
         }
      }

      return itE;
   }

   void CMapGame::SetCurrentTime(const qint64 currentTime)
   {
      std::vector<CPlanet>::iterator itBPlanet = m_vPlanet.begin();
      std::vector<CPlanet>::iterator itEPlanet = m_vPlanet.end();
      qint64 timePause = currentTime - m_vPlanet.begin()->m_timeLastUpdate;
      for(; itBPlanet != itEPlanet; ++itBPlanet)
      {
         itBPlanet->m_timeLastUpdate = currentTime;
      }

      std::list<CFleet>::iterator itBFleet = m_vFleet.begin();
      std::list<CFleet>::iterator itEFleet = m_vFleet.end();
      for(; itBFleet != itEFleet; ++itBFleet)
      {
         itBFleet->m_timeStartMove += timePause;
         itBFleet->m_timeFinishMove += timePause;
      }
   }

} // namespace SingleGame

