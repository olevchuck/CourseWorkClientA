
#ifndef GALCON_H
#define GALCON_H

#include <QObject>

#include "GUI/Gui.h"
#include "serverManager/ServerManager.h"
#include "Game/Game.h"
#include "Player.h"
#include "SingleGame/SingleGameManager.h"
#include "GalconThread.h"

class CGalcon : public QObject
{

   Q_OBJECT

public:
   CGalcon();
   ~CGalcon();

private slots:
   void slotChoiceNetworkGame();
   void slotChoiceSingleGame();
   void slotDisconnect();

private:
   void connectSendersToTakers();
   void connectNetworkSendersToTakers();
   void connectSingleSendersToTakers();
   void disconnectNetworkSendersToTakers();
   void disconnectSingleSendersToTakers();

   GUI::CGUI* m_gui;
   Game::CGame* m_game;
   CGalconThread m_thread;
   ServerManagerDecl::CServerManager* m_manager;
   SingleGame::CSingleGameManager* m_singleGame;
   std::vector<CPlayer*> m_players;
};

#endif // GALCON_H

