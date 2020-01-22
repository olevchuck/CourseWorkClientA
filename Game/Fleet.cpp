
#include "Fleet.h"
#include "Planet.h"
#include "Player.h"

#include <cmath>

namespace Game
{
   const double PI = 3.141592653;
   CFleet::CFleet(const unsigned int id, CPlanet* from, CPlanet* to,
                  CPlayer* player, const unsigned long number, const double percent):
      m_id(id),
      m_from(from),
      m_to(to),
      m_player(player),
      m_number(number),
      m_percentPassed(percent)
   {
      m_playerId = m_player->GetId();
      m_player->AddFleetArmy(m_number);
      if (m_percentPassed < 100)
      {
         m_reached = false;
      }
      else
      {
         m_reached = true;
         m_percentPassed = 100;
      }

      unsigned int x;
      unsigned int y;
      m_from->GetPosition(x, y);
      m_fromPl.x = x;
      m_fromPl.y = y;

      m_to->GetPosition(x, y);
      m_toPl.x = x;
      m_toPl.y = y;

      m_distance = sqrt((m_toPl.x - m_fromPl.x) * (m_toPl.x - m_fromPl.x) +
                      (m_toPl.y - m_fromPl.y) * (m_toPl.y - m_fromPl.y));
      updatePosition();
   }

   CFleet::~CFleet()
   {
      m_player->RemoveFleetArmy(m_number);
   }

   void CFleet::GetPosition(double& o_nX, double& o_nY) const
   {
      o_nX = m_actualX;
      o_nY = m_actualY;
   }

   unsigned short CFleet::GetPlayerId() const
   {
      return m_playerId;
   }

   CPlayer* CFleet::GetPlayer() const
   {
      return m_player;
   }

   unsigned int CFleet::GetId() const
   {
      return m_id;
   }

   unsigned long CFleet::GetShipCount() const
   {
      return m_number;
   }

   void CFleet::SetPercent(const double i_nPerc)
   {
      m_percentPassed = i_nPerc;
      if (m_percentPassed >= 100)
      {
         m_percentPassed = 100;
         if (!m_reached)
         {
            m_reached = true;
            if (m_to->GetArmy() < m_number)
            {
//               m_to->SetPlayer(m_playerId);
//               m_to->SetArmy(m_number - m_to->GetArmy());
            }
            else
            {
//               m_to->SetArmy(m_to->GetArmy() - m_number);
            }
         }
      }
      updatePosition();
   }

   void CFleet::IncreaseWay(double onWay)
   {
      double onPerc = 100 * onWay/m_distance;
      this->SetPercent(m_percentPassed + onPerc);
   }

   bool CFleet::ReachedDestination() const
   {
      return m_reached;
   }

   void CFleet::updatePosition()
   {
      m_actualX = ((double)m_percentPassed/100) * m_toPl.x +
               (1 - (double)m_percentPassed/100) * m_fromPl.x;
      m_actualY = ((double)m_percentPassed/100) * m_toPl.y +
               (1 - (double)m_percentPassed/100) * m_fromPl.y;
   }

   double CFleet::GetAngle() const
   {
      if (m_toPl.x < m_fromPl.x)
      {
         if (m_toPl.y < m_fromPl.y)
         {
            return -asin((m_fromPl.x - m_toPl.x)/m_distance);
         }
         else
         {
            return (-PI/2) - acos((m_fromPl.x - m_toPl.x)/m_distance);
         }
      }
      else
      {
         if (m_toPl.y < m_fromPl.y)
         {
            return asin((m_toPl.x - m_fromPl.x)/m_distance);
         }
         else
         {
            return PI/2 + acos((m_toPl.x - m_fromPl.x)/m_distance);
         }
      }
      return 0;
   }
} // Namepace Game
