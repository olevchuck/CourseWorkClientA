#pragma once

#include <vector>
#include <list>

#include "Game/Planet.h"
#include "Game/Fleet.h"

namespace GUI
{
   class ISceneUpdates
   {
   public:
      virtual ~ISceneUpdates() {}

      virtual void OnShowView() = 0;
      virtual void OnHideView() = 0;

      virtual void OnUpdate(const std::vector<Game::CPlanet* >&, const std::list<Game::CFleet* >&) = 0;
   };
} // Namespace GUI
