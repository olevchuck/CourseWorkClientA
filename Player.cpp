#include "Player.h"

CPlayer::CPlayer(unsigned short i_id, std::string i_name): m_id(i_id), m_name(i_name)
{
   m_army = 0;
   m_fleetArmy = 0;
}

unsigned short CPlayer::GetId() const
{
   return m_id;
}

std::string CPlayer::GetName() const
{
   return m_name;
}

unsigned long CPlayer::GetArmy() const
{
   return m_army;
}
unsigned long CPlayer::GetFleetArmy() const
{
   return m_fleetArmy;
}

void CPlayer::SetArmy(const unsigned long army)
{
   m_army += army;
}

void CPlayer::NullArmy()
{
   m_army = 0;
}

void CPlayer::AddFleetArmy(const unsigned long army)
{
   m_fleetArmy += army;
}

void CPlayer::RemoveFleetArmy(const unsigned long army)
{
   m_fleetArmy -= army;
}
