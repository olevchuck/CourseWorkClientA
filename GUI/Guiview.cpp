#include <QVariant>
#include <QPainter>
#include <QPen>
#include <QWidget>

#include "Guiview.h"
#include "Guispace.h"
#include "Guiplanet.h"
#include "Guifleet.h"

namespace GUI
{
   CGUIView::CGUIView(unsigned int x, unsigned int y, QWidget* parent) :
      m_width(x),
      m_height(y),
      m_percent(50),
      m_parent(parent)
   {
      m_space = new CGUISpace(m_width,m_height);
   }

   CGUIView::~CGUIView()
   {
   }

   void CGUIView::OnHideView()
   {
   }

   void CGUIView::OnShowView()
   {
   }

   void CGUIView::OnUpdate(const std::vector<Game::CPlanet *>& planets, const std::list<Game::CFleet *>& fleets)
   {
      /// Update planets

      if (m_planets.empty())
      {
         for (unsigned int i = 0; i < planets.size(); ++i)
         {
            CGUIPlanet* currPl = new CGUIPlanet(planets[i]);
            m_planets.push_back(currPl);
         }
      }
      /// Update fleets
      m_fleets.erase(m_fleets.begin(), m_fleets.end());
      Game::CFleet* iterfl;
      foreach (iterfl, fleets)
      {
         CGUIFleet* currFl = new CGUIFleet(iterfl);
         m_fleets.push_back(currFl);
      }
      m_parent->update();
   }

   void CGUIView::Draw(QPainter* painter, double scale)
   {
      painter->setBrush(Qt::green);
      m_space->Draw(painter);

      /// Finding active planets

      CGUIPlanet* currPl;
      std::vector<CGUIPlanet* > activePlanets;

      foreach (currPl, m_planets)
      {
         if (currPl->IsActive())
         {
            activePlanets.push_back(currPl);
         }
      }

      /// Connecting active planets with lines

      if (!activePlanets.empty())
      {
         QPen linesPen(CGUIPlanet::GetColor(m_playerId), 1);
         painter->setPen(linesPen);
         unsigned int min_x(0), min_y(0);
         unsigned int max_x(0), max_y(0);
         unsigned int curr_x(0), curr_y(0);
         activePlanets.front()->GetPlanet()->GetPosition(min_x, min_y);
         activePlanets.front()->GetPlanet()->GetPosition(max_x, max_y);

         /// Find min and max koords of active planets to determine middle planet

         foreach (currPl, activePlanets)
         {
            currPl->GetPlanet()->GetPosition(curr_x, curr_y);
            if (curr_x < min_x)
            {
               min_x = curr_x;
            }
            else if (curr_x > max_x)
            {
               max_x = curr_x;
            }
            if (curr_y < min_y)
            {
               min_y = curr_y;
            }
            else if (curr_y > max_y)
            {
               max_y = curr_y;
            }
         }

         /// Determine central planet

         unsigned int middle_x ((max_x + min_x)/2);
         unsigned int middle_y ((max_y + min_y)/2);
         int diff(middle_x + middle_y);
         CGUIPlanet* centralPl;
         int diffX = curr_x - middle_x;
         int diffY = curr_y - middle_y;
         foreach (currPl, activePlanets)
         {
            currPl->GetPlanet()->GetPosition(curr_x, curr_y);
            if (diff > abs(diffX) + abs(diffY))
            {
               centralPl = currPl;
               diff = abs(diffX) + abs(diffY);
            }
         }

         /// Draw lines to central planet
         unsigned int central_x(0), central_y(0);
         centralPl->GetPlanet()->GetPosition(central_x, central_y);
         foreach (currPl, activePlanets)
         {
            if (currPl != centralPl)
            {
               currPl->GetPlanet()->GetPosition(curr_x, curr_y);
               painter->drawLine(central_x, central_y, curr_x, curr_y);
            }
         }

      }

      /// Draw planets

      foreach (currPl, m_planets)
      {
         currPl->Draw(painter);
      }

      /// Draw fleets
      CGUIFleet* currFl;
      foreach (currFl, m_fleets)
      {
         currFl->Draw(painter, scale);
      }
      /// Draw percents
      painter->setPen(Qt::white);
      QString percentStr("Perc:");
      percentStr.append(QVariant(quint16(m_percent)).toString());
      percentStr.append("%");
      painter->drawText(m_width - 80, 30, percentStr);
   }

   bool CGUIView::Selection(unsigned int beginX, unsigned int beginY,
                            unsigned int endX, unsigned int endY)
   {
      bool returnValue = false;
      CGUIPlanet* currPl;
      unsigned int x(0), y(0);
      foreach (currPl, m_planets)
      {
         Game::CPlanet* pl = currPl->GetPlanet();
         pl->GetPosition(x,y);
         /// If my planet in selected rectangle
         if ((x >= std::min(beginX, endX)) && (x <= std::max(beginX, endX))
               && (y >= std::min(beginY, endY)) && (y <= std::max(beginY,endY))
               && (pl->GetPlayerId() == m_playerId))
         {
            currPl->MakeActive();
            returnValue = true;
         }
         else
         {
            currPl->ReleaseActive();
         }
      }
      return returnValue;
   }

   bool CGUIView::SelectOne(unsigned int clickX, unsigned int clickY)
   {
      CGUIPlanet* currPl;
      unsigned int x(0), y(0), radius(0);
      foreach (currPl, m_planets)
      {
         currPl->GetPlanet()->GetPosition(x,y);
         radius = currPl->GetPlanet()->GetRadius();
         if ((clickX >= x - radius) && (clickX <= x + radius)
               && (clickY >= y - radius) && (clickY <= y + radius)
               && (currPl->GetPlanet()->GetPlayerId() == m_playerId))
         {
            currPl->MakeActive();
            return true;
         }
      }
      return false;
   }

   Message::CMessageStepPlayerPtr CGUIView::Target(unsigned int clickX, unsigned int clickY)
   {
      std::vector<unsigned int> departure;
      unsigned int destination;

      bool flagDest = false;
      CGUIPlanet* currPl;
      foreach(currPl, m_planets)
      {
         if (currPl->IsActive())
         {
            departure.push_back(currPl->GetPlanet()->GetId());
         }

         unsigned int x(0), y(0);
         currPl->GetPlanet()->GetPosition(x,y);
         /// If clicked after selection on some planet - it is destination
         int diffx = clickX - x;
         int diffy = clickY - y;
         if ((abs(diffx) < currPl->GetPlanet()->GetRadius()) &&
               (abs(diffy) < currPl->GetPlanet()->GetRadius()))
         {
               destination = currPl->GetPlanet()->GetId();
               flagDest = true;
         }
      }
      Message::CMessageStepPlayerPtr mess (new Message::CMessageStepPlayer);
      mess->m_finishPlanetID = destination;
      mess->m_startPlanetID = departure;

      if (!flagDest)
      {
         /// If no destination selected or no departure planets
         mess->m_percent = 0;
      }
      else
      {
         mess->m_percent = m_percent;
      }
      return mess;
   }

   void CGUIView::CheckAll(unsigned int clickX, unsigned int clickY)
   {
      bool flagChecked = false;
      CGUIPlanet* currPl;
      foreach (currPl, m_planets)
      {
         unsigned int x(0), y(0);
         currPl->GetPlanet()->GetPosition(x,y);
         int diffX = clickX - x;
         int diffY = clickY - y;
         if ((abs(diffX) < currPl->GetPlanet()->GetRadius()) &&
               (abs(diffY) < currPl->GetPlanet()->GetRadius())
               && (currPl->GetPlanet()->GetPlayerId() == m_playerId))
         {
               flagChecked = true;
         }
      }

      if (flagChecked)
      {
         CGUIPlanet* currPl;
         foreach (currPl, m_planets)
         {
            if (currPl->GetPlanet()->GetPlayerId() == m_playerId)
            {
               currPl->MakeActive();
            }
         }
      }
   }

   unsigned int CGUIView::GetPercent() const
   {
      return m_percent;
   }

   unsigned int CGUIView::GetPlayerId() const
   {
      return m_playerId;
   }



   void CGUIView::SetPercent(int percent)
   {
      if (percent <= 0)
      {
         m_percent = 0;
      }
      else if (percent >= 100)
      {
         m_percent = 100;
      }
      else
      {
         m_percent = percent;
      }
   }

   void CGUIView::SetPlayerId(unsigned int id)
   {
      m_playerId = id;
   }
} // Namespace GUI
