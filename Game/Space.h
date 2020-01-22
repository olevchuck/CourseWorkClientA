#pragma once

#include <vector>
#include <list>

#include "message/MessageStartMapGame.h"
#include "message/MessageStateMap.h"
#include "Player.h"

namespace Game
{
   class CPlanet;
   class CFleet;

   class CSpace
   {
   public:

      CSpace(unsigned short flySpeed, unsigned short growSpeed, unsigned short xSize, unsigned short ySize,
             std::vector<Message::CPlanetStartData> planets, std::vector<CPlayer*>* players);
      ~CSpace();

      void SetPlanets(const std::vector<Message::CStatePlanet>&);
      void SetFleets(const std::vector<Message::CStateFleet>&);
      void Update(const double i_time);

      std::vector<CPlanet*> GetPlanets() const;
      std::list<CFleet*> GetFleets() const;

      CPlanet* GetPlanetById(const unsigned short id) const;
      bool CheckPlanetId (const unsigned int id) const;

   private:
      CPlayer* GetPlayerById(const unsigned int id) const;

      std::vector<CPlayer* >* m_players;
      std::vector<CPlanet* > m_planets;
      std::list<CFleet* > m_fleets;
      unsigned short m_speed;
      unsigned short m_growth;
      unsigned short m_width;
      unsigned short m_height;

   };
} // Namespace Game

