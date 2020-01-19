#pragma once

#include <string>

class CPlayer
{
public:
   CPlayer(unsigned short i_id, std::string i_name);
   ~CPlayer();

   unsigned short GetId() const;

   std::string GetName() const;

   unsigned long GetArmy() const;

   unsigned long GetFleetArmy() const;

   void SetArmy(const unsigned long army);
   void NullArmy();

   void AddFleetArmy(const unsigned long army);
   void RemoveFleetArmy(const unsigned long army);


private:
   unsigned short m_id;
   std::string m_name;
   unsigned long m_army;
   unsigned long m_fleetArmy;
};
