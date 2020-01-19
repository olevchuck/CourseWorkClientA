#pragma once

#include <string>
#include <QTime>

namespace SingleGame
{
   struct CPlayer;

   struct Point
   {
      unsigned int x;
      unsigned int y;
   }; // struct Point

   struct CGameObject
   {
      void GenerationID();

      unsigned int GetID() const;

      static void ClearID();

   protected:

      static unsigned int sID;

      unsigned int m_ID;

   }; // struct CGameObject   

   struct CPlanet : public CGameObject
   {
      CPlayer* m_pPlayer;
      Point m_coordinates;
      unsigned int m_radius;
      double m_countFleet;

      qint64 m_timeLastUpdate;

      void UpdatePlanet(const qint64 time, const unsigned int v);

   }; // struct CPlanet

   struct CFleet : public CGameObject
   {
      CPlayer* m_pPlayer;
      CPlanet* m_fromPlanet;
      CPlanet* m_toPlanet;
      unsigned int m_countFleet;

      qint64 m_timeStartMove;
      qint64 m_timeFinishMove;

      void SetTimeFinish(const unsigned int v);

   }; // struct CFleet

   struct CPlayer : public CGameObject
   {
      void SetNeutral();

      void AddFleet(CFleet* pFleet);

      void DeleteFleet(const unsigned int ID);

      void AddPlanet(CPlanet* pPlanet);

      void DeletePlanet(const unsigned int ID);

      bool Empty() const; ///< return true if player no have fleets or planets

      std::string m_name;

      std::list<CPlanet*> m_vPlanet;
      std::list<CFleet*> m_vFleet;
   }; // struct CPlayer

} // namespace SingleGame
