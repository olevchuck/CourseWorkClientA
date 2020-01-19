#pragma once

#include <QObject>

#include "Player.h"
#include "message/MessageConnectToServer.h"
#include "message/MessageConnectToSingleGame.h"
#include "message/MessageTimeToStartGame.h"
#include "message/MessageStartMapGame.h"
#include "message/MessageError.h"
#include "message/MessageAddView.h"
#include "message/MessageFinishGame.h"
#include "message/MessageStepPlayer.h"
#include "message/MessageInformation.h"

namespace GUI
{
   class CPlayWindow;
   class CWaitWindow;
   class CEnterWindow;
   class CSingleWindow;
   class ISceneUpdates;
   class CMainWindow;
   class CPauseWindow;

   class CGUI : public QObject
   {
      Q_OBJECT
   private:
      enum ETypeGame
      {
         eSingleGame,
         eNetworkGame
      };

   public:

      explicit CGUI(std::vector<CPlayer* >* players, QObject *parent = 0);
      virtual ~CGUI();

      void ShowWindow();
      void DestroyWindow();

   public slots:
      void TakeConfirmConnectToServer(const Message::CMessageConfirmationConnectToServerPtr mess);

      void TakeError(const Message::CMessageErrorPtr mess);

      void TakeFinishGame(const Message::CMessageFinishGamePtr mess);

      void TakeTimeStartToGame(const Message::CMessageTimeToStartGamePtr mess);

      void TakeFieldSize(const unsigned int X, const unsigned int Y);

      void TakeInInformation(const Message::CMessageInformationPtr mess);

   signals:
      void SendClientToServer(const Message::CMessageConnectToServerPtr mess);
      void SendClientToSingleGame(const Message::CMessageConnectToSingleGamePtr mess);
      void SendView(Message::CMessageAddViewPtr mess);
      void SendStepPlayer(const Message::CMessageStepPlayerPtr mess);

      void signalChoiceSingleGame();
      void signalChoiceNetworkGame();
      void signalDisconnect();

      void signalPause();
      void signalExit();

   private slots:
      void slotChoiceSingleGame();
      void slotChoiceNetworkGame();
      void slotPauseGame();
      void slotExitToMain();

   private:
      CWaitWindow* m_waitWindow;
      CEnterWindow* m_enterWindow;
      CSingleWindow* m_singleWindow;
      CPlayWindow* m_playWindow;
      CMainWindow* m_mainWindow;
      CPauseWindow* m_pauseWindow;

      unsigned int m_playerId;
      bool m_paused;
      std::vector<CPlayer* >* m_players;

      ETypeGame m_typeGame;

   }; // class CGUI
} // Namespace GUI

