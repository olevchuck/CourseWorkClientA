#pragma once

#include <vector>
#include <QSharedPointer>

namespace Message
{
   struct CMessageStateMap;

   typedef QSharedPointer<CMessageStateMap> CMessageStateMapPtr;

   struct CStatePlanet
   {
      unsigned int m_countFleet;
      unsigned int m_planetID;
      unsigned int m_playerID;
   }; // struct CStatePlanet

   struct CStateFleet
   {
      unsigned int m_countFleet;
      unsigned int m_fleetID;
      unsigned int m_percentRoute;
      unsigned int m_planetFinishID;
      unsigned int m_planetStartID;
      unsigned int m_playerID;

   }; // struct CStateFleet

   struct CMessageStateMap
   {
      std::vector<CStateFleet> m_fleetState;
      std::vector<CStatePlanet> m_planetState;

   }; // struct CMessageStateMap

}; // namespace Message
