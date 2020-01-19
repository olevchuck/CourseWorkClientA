#pragma once

#include <QObject>
#include <QTimer>
#include <string>
#include <vector>

#include "message/MessageItf.h"
#include "message/MessageConnectToSingleGame.h"
#include "message/MessageConnectToServer.h"
#include "message/MessageStepPlayer.h"
#include "message/MessageError.h"
#include "message/MessageFinishGame.h"
#include "message/MessageStateMap.h"
#include "message/MessageStartMapGame.h"
#include "message/MessageTimeToStartGame.h"
#include "message/MessageInformation.h"
#include "MapGame.h"
#include "Bot.h"


namespace SingleGame
{
   class CSingleGameManager : public QObject
   {
      Q_OBJECT

   public:
       CSingleGameManager();
       ~CSingleGameManager();

   public slots:
      void TakeServerConnect(const Message::CMessageConnectToSingleGamePtr pMessage);
      void TakeStepPlayer(const Message::CMessageStepPlayerPtr pMessage);
      void TakePause();
      void TakeExit();

   signals:
      void SendConfirmConnect(const Message::CMessageConfirmationConnectToServerPtr pMessage);
      void SendError(const Message::CMessageErrorPtr pMessage);
      void SendFinishGame(const Message::CMessageFinishGamePtr pMessage);
      void SendStateMap(const Message::CMessageStateMapPtr pMessage);
      void SendStartGame(const Message::CMessageStartMapGamePtr pMessage);
      void SendTimeToStart(const Message::CMessageTimeToStartGamePtr pMessage);

      void SendInInformation(const Message::CMessageInformationPtr pMessage);

   private slots:
      void slotWaitTime();
      void slotRunTime();
      void slotStepBot();      

   private:
      /// wait timer + generation map
      bool startGame(
         const unsigned int timeToStart,
         const unsigned int widthMap,
         const unsigned int heigthMap,
         const unsigned int flySpeed,
         const unsigned int growSpeed,
         const std::string& namePlayer,
         const unsigned int planetMinSize,
         const unsigned int planetMaxSize,
         const unsigned int dispersionPlanets,
         const unsigned int fleetMinCount,
         const unsigned int fleetMaxCount,
         const unsigned int botNumber,
         const unsigned int botLevel
         );      

      /// run timer + logic
      void runPlay();
      void clear();

      /// pause
      void pauseON();
      void pauseOFF();

      void createBot(
         const unsigned int botNumber,
         const unsigned int botLevel);

      void checkEndGame();

      CMapGame m_mapGame;
      std::vector<CPlayer> m_vPlayer;
      QTimer m_timerWaitStart;
      QTimer m_timerRunTime;
      QTimer m_timerBot;
      unsigned int m_timeToStart;

      std::vector<CBot> m_vBot;
      bool m_stepBot;

      bool m_pause;

      static const unsigned int sMaxCountFleets = 10;

   }; // class CSingleGameManager

} // namespace SingleGame

