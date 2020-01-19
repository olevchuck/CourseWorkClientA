#pragma once

#include "SceneUpdates.h"
#include "message/MessageStepPlayer.h"

namespace GUI
{
   class CGUIPlanet;

   class CGUIFleet;

   class CGUISpace;

   class CGUIView : public ISceneUpdates
   {
   public:
       explicit CGUIView(unsigned int x, unsigned int y, QWidget* parent);
       ~CGUIView();

       void OnShowView();
       void OnHideView();
       void OnUpdate(const std::vector<Game::CPlanet *> &, const std::list<Game::CFleet *> &);

       void Draw(QPainter* painter, double scale);

       bool Selection(unsigned int beginX, unsigned int beginY,
                      unsigned int endX, unsigned int endY);
       bool SelectOne(unsigned int clickX, unsigned int clickY);

       Message::CMessageStepPlayerPtr Target(unsigned int clickX, unsigned int clickY);
       void CheckAll(unsigned int clickX, unsigned int clickY);

       unsigned int GetPlayerId() const;
       unsigned int GetPercent() const;

       void SetPlayerId(unsigned int id);
       void SetPercent(int percent);

   private:
      unsigned int m_width;
      unsigned int m_height;
      unsigned int m_percent;
      unsigned int m_playerId;

      std::vector<CGUIPlanet*> m_planets;
      std::list<CGUIFleet*> m_fleets;
      CGUISpace* m_space;

      QWidget* m_parent;
   };
} // Namespace GUI
