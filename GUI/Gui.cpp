#include <QDialog>
#include <QVariant>

#include "Gui.h"
#include "Mainwindow.h"
#include "Playwindow.h"
#include "Waitwindow.h"
#include "Enterwindow.h"
#include "Singlewindow.h"
#include "Errorwindow.h"
#include "Pausewindow.h"

namespace GUI
{
   CGUI::CGUI(std::vector<CPlayer *> *players, QObject *parent) :
      QObject(parent),
      m_playerId(0),
      m_paused(false),
      m_players(players)
   {
      m_mainWindow = new CMainWindow();
      m_enterWindow = new CEnterWindow();
      m_waitWindow = new CWaitWindow();
      m_playWindow = new CPlayWindow();
      m_singleWindow = new CSingleWindow();
      m_pauseWindow = new CPauseWindow();
      m_pauseWindow->setWindowTitle("Paused");

      connect(m_enterWindow, SIGNAL(SendClientToServer(Message::CMessageConnectToServerPtr)),
              this, SIGNAL(SendClientToServer(Message::CMessageConnectToServerPtr)));
      connect(m_singleWindow, SIGNAL(SendClientToSingleGame(Message::CMessageConnectToSingleGamePtr)),
              this, SIGNAL(SendClientToSingleGame(Message::CMessageConnectToSingleGamePtr)));

      connect(m_playWindow, SIGNAL(SendStepPlayer(Message::CMessageStepPlayerPtr)),
              this, SIGNAL(SendStepPlayer(Message::CMessageStepPlayerPtr)));

      connect(m_mainWindow, SIGNAL(signalChoiceSingleGame()),
              this, SLOT(slotChoiceSingleGame()));
      connect(m_mainWindow, SIGNAL(signalChoiceNetworkGame()),
              this, SLOT(slotChoiceNetworkGame()));
      connect(m_pauseWindow, SIGNAL(ReturnToGame()), this, SLOT(slotPauseGame()));
      connect(m_playWindow, SIGNAL(PauseGame()), this, SLOT(slotPauseGame()));
      connect(m_pauseWindow, SIGNAL(ExitGame()), this, SLOT(slotExitToMain()));
   }

   CGUI::~CGUI()
   {
      delete m_mainWindow;
      delete m_enterWindow;
      delete m_waitWindow;
      delete m_playWindow;
      delete m_singleWindow;
   }

   void CGUI::ShowWindow()
   {
      m_mainWindow->ShowWindow();
   }

   void CGUI::DestroyWindow()
   {
      m_mainWindow->DestroyWindow();
   }

   void CGUI::TakeConfirmConnectToServer(const Message::CMessageConfirmationConnectToServerPtr mess)
   {
       m_playerId = mess->m_playerID;
       m_playWindow->SetPlayerId(m_playerId);
       switch(m_typeGame)
       {
       case eSingleGame:
          m_singleWindow->DestroyWindow();
          break;
       case eNetworkGame:
          m_enterWindow->DestroyWindow();
          break;
       }

       m_waitWindow->ShowWindow();
   }

   void CGUI::TakeError(const Message::CMessageErrorPtr mess)
   {
      CErrorWindow::Show("Server error", mess->m_strError.c_str());
      m_playWindow->DestroyWindow();
      m_waitWindow->DestroyWindow();
      m_mainWindow->ShowWindow();
      emit signalExit();
   }

   void CGUI::TakeTimeStartToGame(const Message::CMessageTimeToStartGamePtr mess)
   {
      m_waitWindow->SetSecToStart(mess->m_second);
   }

   void CGUI::TakeFinishGame(const Message::CMessageFinishGamePtr mess)
   {
      m_playWindow->DestroyWindow();
      CPlayer* currPlayer = NULL;
      foreach (currPlayer, *m_players)
      {
         if (mess->m_playerID == currPlayer->GetId())
         {
            break;
         }
      }
      if (currPlayer->GetId() == m_playerId)
      {
         CErrorWindow::Show("Winner!", "Congratulation "+
            QString::fromStdString(currPlayer->GetName()) + " you win!");
      }
      else
      {
         CErrorWindow::Show("Loser!", "Unfortunately you lose. The winner is "+
            QString::fromStdString(currPlayer->GetName()) + "!");
      }
      m_players->clear();
      m_mainWindow->ShowWindow();
      emit signalDisconnect();
   }

   void CGUI::TakeInInformation(const Message::CMessageInformationPtr mess)
   {
      if(mess->m_typeInformation == Message::CMessageInformation::eGameError)
      {
         CErrorWindow::Show("Game error", QString::fromStdString(mess->m_strInformation));
      }

      if(mess->m_typeInformation == Message::CMessageInformation::eConnectionToServer)
      {
         CErrorWindow::Show("Server error", QString::fromStdString(mess->m_strInformation));
      }
   }

   void CGUI::TakeFieldSize(const unsigned int X, const unsigned int Y)
   {
      m_playWindow->CreateWindow(X, Y);
      m_playWindow->SetPlayers(m_players);
      m_waitWindow->DestroyWindow();
      m_playWindow->ShowWindow();
      m_paused = false;

      Message::CMessageAddViewPtr ptr(new Message::CMessageAddView);
      ptr->m_view = (GUI::ISceneUpdates*)m_playWindow->GetView();
      SendView(ptr);
   }

   void CGUI::slotChoiceSingleGame()
   {
      m_typeGame = eSingleGame;

      emit signalChoiceSingleGame();

      m_mainWindow->DestroyWindow();
      m_singleWindow->ShowWindow();
   }

   void CGUI::slotChoiceNetworkGame()
   {
      m_typeGame = eNetworkGame;

      emit signalChoiceNetworkGame();

      m_mainWindow->DestroyWindow();
      m_enterWindow->ShowWindow();
   }

   void CGUI::slotPauseGame()
   {
      if (!m_paused)
      {
         m_pauseWindow->show();
         if (m_typeGame == eSingleGame)
         {
            emit signalPause();
         }
      }
      else
      {
         m_pauseWindow->hide();
         if (m_typeGame == eSingleGame)
         {
            emit signalPause();
         }
      }
      m_paused = !m_paused;
   }

   void CGUI::slotExitToMain()
   {
      qDebug("Gui - exit to main");
      m_paused = false;
      m_playWindow->DestroyWindow();
      m_pauseWindow->hide();
      m_players->clear();
      m_mainWindow->ShowWindow();
      emit signalExit();
   }

} // Namespace GUI
