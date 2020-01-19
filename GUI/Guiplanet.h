#pragma once

#include <QPainter>
#include <QColor>

#include "Game/Planet.h"

namespace GUI
{
   class CGUIPlanet
   {
   public:
      CGUIPlanet(){}
      CGUIPlanet(Game::CPlanet* planet);

      void Draw(QPainter*);

      Game::CPlanet* GetPlanet() const;

      void MakeActive();
      void ReleaseActive();
      bool IsActive() const;

      static QColor GetColor(unsigned int id)
      {
         switch (id)
         {
            case 0:
               return Qt::gray;
               break;
            case 1:
               return Qt::red;
               break;
            case 2:
               return Qt::green;
               break;
            case 3:
               return Qt::blue;
               break;
            case 4:
               return QColor::fromRgb(0, 255,255);
               break;
            case 5:
               return QColor::fromRgb(255,0,255);
               break;
            case 6:
               return Qt::yellow;
               break;
            case 7:
               return QColor::fromRgb(96,0,0);
               break;
            case 8:
               return QColor::fromRgb(0, 96, 0);
               break;
            default:
               return Qt::lightGray;
               break;
         }
      }

   private:
      Game::CPlanet* m_planet;
      bool m_active;
   };
} // Namespace GUI
