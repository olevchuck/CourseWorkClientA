#include <QVariant>
#include <QRadialGradient>

#include "Guiplanet.h"

namespace GUI
{
   CGUIPlanet::CGUIPlanet(Game::CPlanet* planet)
      : m_planet(planet),
        m_active(false)
   {

   }

   void CGUIPlanet::Draw(QPainter* painter)
   {
      QBrush br(Qt::lightGray);
      unsigned x(0), y(0);
      m_planet->GetPosition(x,y);
      /// If planet is selected - draw a bit larger circle first
      painter->setBrush(Qt::NoBrush);
      if(m_active)
      {
         painter->setPen(QPen(GetColor(m_planet->GetPlayerId()), 1));
         //painter->setBrush(Qt::blue);
         painter->drawEllipse(x - m_planet->GetRadius()/2 - 5, y - m_planet->GetRadius()/2 - 5,
                             m_planet->GetRadius() + 10, m_planet->GetRadius() + 10);
      }
      QRadialGradient gr(x, y, m_planet->GetRadius(), x, y);
      gr.setColorAt(0.0, Qt::white);
      gr.setColorAt(0.5, GetColor(m_planet->GetPlayerId()));
      gr.setColorAt(1.0, Qt::black);
      painter->setPen(Qt::NoPen);
      painter->setBrush(gr);
      painter->drawEllipse(x - m_planet->GetRadius()/2, y - m_planet->GetRadius()/2,
                          m_planet->GetRadius(), m_planet->GetRadius());
      /// Draw amount of army on planet
      painter->setPen(Qt::white);
      painter->drawText(x - m_planet->GetRadius()/2, y - m_planet->GetRadius()/2,
                        QVariant(quint32(m_planet->GetArmy())).toString());
   }

   void CGUIPlanet::MakeActive()
   {
      m_active = true;
   }

   void CGUIPlanet::ReleaseActive()
   {
      m_active = false;
   }

   bool CGUIPlanet::IsActive() const
   {
      return m_active;
   }

   Game::CPlanet* CGUIPlanet::GetPlanet() const
   {
      return m_planet;
   }
} // Namespace GUI
