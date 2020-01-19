#pragma once

#include <vector>
#include <list>
#include <QTime>

#include "GameMapObject.h"

namespace SingleGame
{
   class CMapGame
   {
   public:
      CMapGame();
      ~CMapGame();

   public:
      void GenerationMap(
         const unsigned int widthMap,
         const unsigned int heigthMap,
         const unsigned int flySpeed,
         const unsigned int growSpeed,
         const unsigned int planetMinSize,
         const unsigned int planetMaxSize,
         const unsigned int dispersionPlanets,
         const unsigned int fleetMinCount,
         const unsigned int fleetMaxCount);

      unsigned int CountPlanets();

      void SetPlayers(std::vector<CPlayer>& vPlayer);

      const std::vector<CPlanet>& GetPlanets() const;
      const std::list<CFleet>& GetFleets() const;
      const unsigned int GetWidthMap() const;
      const unsigned int GetHeigthMap() const;
      const unsigned int GetFlySpeed() const;
      const unsigned int GetGrowSpeed() const;

      void UpdateStateMap(
         const unsigned int finishPlanetID,
         const unsigned int percent,
         const std::vector<unsigned int>& startPlanetID,
         const qint64 currentTime);
      void UpdateStateMap(const qint64 currentTime);

      void SetCurrentTime(const qint64 currentTime);

      /// clear all data
      void Clear();

   private:
      void createPlanet();

      void updateFleet(const qint64 currentTime);
      void updatePlanet(const qint64 time);

      void addFleet(
         const unsigned int finishPlanetID,
         const unsigned int percent,
         const std::vector<unsigned int>& startPlanetID,
         const qint64 currentTime);

      CPlanet* getPlanet(const unsigned int ID);

      /// find position fleet with time finish more than parametres fleet
      std::list<CFleet>::iterator findInsertFleetPosition(const CFleet& fleet);

      unsigned int m_widthMap;
      unsigned int m_heigthMap;
      unsigned int m_flySpeed;
      unsigned int m_growSpeed;

      unsigned int m_cMaxPlanetRadius;
      unsigned int m_cMinPlanetRadius;
      unsigned int m_cCoefDispersionPlanets;
      unsigned int m_cMinFleetCount;
      unsigned int m_cMaxFleetCount;

      std::vector<CPlanet> m_vPlanet;
      std::list<CFleet> m_vFleet; ///< sorted for time finish
      CPlayer m_neutralPlayer;

   }; // class CMapGame

} // namespace SingleGame


