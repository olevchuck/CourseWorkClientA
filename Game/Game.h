#pragma once

/// @file Game.h
/// @brief CGame declaration header
/// @author Dmitriy Kozhevnikov
/// @date 06-June-2011

#include <QObject>
#include <QTimer>

#include "message/MessageAddView.h"
#include "message/MessageFinishGame.h"
#include "message/MessageInformation.h"

#include "GUI/SceneUpdates.h"
#include "Space.h"
#include "Player.h"

namespace Game
{
   class CGame : public QObject
   {
      Q_OBJECT

   public:
      CGame(std::vector<CPlayer* >* players);
      static const unsigned long m_timeTick = 50;

   signals:
      void SendStartGame(unsigned int mapX, unsigned int mapY);
      void SendError(Message::CMessageInformationPtr);
      void signalTimer();
      void SendFinishGame(Message::CMessageFinishGamePtr mes);

   public slots:
      void SlotStartData(Message::CMessageStartMapGamePtr data);
      void SlotStateMap(Message::CMessageStateMapPtr data);
      void SlotFinishGame(Message::CMessageFinishGamePtr mes);
      void SlotPause();
      void SlotExitGame();

      void AddView(Message::CMessageAddViewPtr mes);
      void DeleteGame();
      void slotTimer();

   private:
      inline bool AvailiblePlayer(unsigned int id) const;
      bool CheckStatePlanet(Message::CMessageStateMapPtr mess) const;
      bool CheckStateFleet(Message::CMessageStateMapPtr mess) const;

      std::vector<CPlayer* >* m_players;
      GUI::ISceneUpdates* m_view;
      CSpace* m_space;
      QTimer* m_timer;
      bool m_paused;
   };
} // Namespace Game

