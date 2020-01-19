#include <math.h>

#include "GameMapObject.h"

namespace SingleGame
{
   /// -------------------- CGameObject

   unsigned int CGameObject::sID = 0;

   /// ------------------ CGameObject

   void CGameObject::GenerationID()
   {
      m_ID = ++sID;
   }

   unsigned int CGameObject::GetID() const
   {
      return m_ID;
   }

   void CGameObject::ClearID()
   {
      sID = 0;
   }

   /// ------------------ CPlanet

   void CPlanet::UpdatePlanet(const qint64 time, const unsigned int v)
   {
      if(m_timeLastUpdate < time)
      {
         m_countFleet += ( double(time - m_timeLastUpdate) / 1000)
               * v * m_radius;
         m_timeLastUpdate = time;
      }
   }

   /// ----------------- CFleet

   void CFleet::SetTimeFinish(const unsigned int v)
   {
      double distance = sqrt(
         (m_fromPlanet->m_coordinates.x - m_toPlanet->m_coordinates.x)
         * (m_fromPlanet->m_coordinates.x - m_toPlanet->m_coordinates.x)
         + (m_fromPlanet->m_coordinates.y - m_toPlanet->m_coordinates.y)
         * (m_fromPlanet->m_coordinates.y - m_toPlanet->m_coordinates.y));

      m_timeFinishMove = m_timeStartMove + (distance / v) * 1000;
   }

   /// ----------------- CPlayer

   void CPlayer::SetNeutral()
   {
      CGameObject::m_ID = 0;
   }

   std::string m_name;

   void CPlayer::AddFleet(CFleet* pFleet)
   {
      m_vFleet.push_back(pFleet);
   }

   void CPlayer::DeleteFleet(const unsigned int ID)
   {
      std::list<CFleet*>::iterator itB = m_vFleet.begin();
      std::list<CFleet*>::iterator itE = m_vFleet.end();
      for(; itB != itE; ++itB)
      {
         if((*itB)->GetID() == ID)
         {
            m_vFleet.erase(itB);
            break;
         }
      }
   }

   void CPlayer::AddPlanet(CPlanet* pPlanet)
   {
      m_vPlanet.push_back(pPlanet);
   }

   void CPlayer::DeletePlanet(const unsigned int ID)
   {
      std::list<CPlanet*>::iterator itB = m_vPlanet.begin();
      std::list<CPlanet*>::iterator itE = m_vPlanet.end();
      for(; itB != itE; ++itB)
      {
         if((*itB)->GetID() == ID)
         {
            m_vPlanet.erase(itB);
            break;
         }
      }
   }

   bool CPlayer::Empty() const ///< return true if player no have fleets or planets
   {
      return m_vPlanet.empty() && m_vFleet.empty();
   }


} // namespace SingleGame
