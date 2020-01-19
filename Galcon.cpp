#include "Galcon.h"

CGalcon::CGalcon()
{
   m_manager = new ServerManagerDecl::CServerManager();
   m_singleGame = new SingleGame::CSingleGameManager();
   m_game = new Game::CGame(&m_players);
   m_gui = new GUI::CGUI(&m_players);

   m_manager->moveToThread(&m_thread);
   m_singleGame->moveToThread(&m_thread);

   m_thread.start();

   qRegisterMetaType<Message::CMessageConnectToSingleGamePtr>("Message::CMessageConnectToSingleGamePtr");
   qRegisterMetaType<Message::CMessageAddViewPtr>("Message::CMessageAddViewPtr");
   qRegisterMetaType<Message::CMessageInformationPtr>("Message::CMessageInformationPtr");
   qRegisterMetaType<Message::CMessageConnectToServerPtr>("Message::CMessageConnectToServerPtr");
   qRegisterMetaType<Message::CMessageStateMapPtr>("Message::CMessageStateMapPtr");
   qRegisterMetaType<Message::CMessageConfirmationConnectToServerPtr>("Message::CMessageConfirmationConnectToServerPtr");

   qRegisterMetaType<Message::CMessageErrorPtr>("Message::CMessageErrorPtr");
   qRegisterMetaType<Message::CMessageFinishGamePtr>("Message::CMessageFinishGamePtr");
   qRegisterMetaType<Message::CMessageStartMapGamePtr>("Message::CMessageStartMapGamePtr");
   qRegisterMetaType<Message::CMessageTimeToStartGamePtr>("Message::CMessageTimeToStartGamePtr");
   qRegisterMetaType<Message::CMessageErrorPtr>("Message::CMessageFinishGamePtr");
   qRegisterMetaType<Message::CMessageStepPlayerPtr>("Message::CMessageStepPlayerPtr");

   connectSendersToTakers();
   m_gui->ShowWindow();
}
CGalcon::~CGalcon()
{
   m_thread.exit();
}

void CGalcon::connectSendersToTakers()
{
   connect(m_gui, SIGNAL(SendView(Message::CMessageAddViewPtr)),
           m_game, SLOT(AddView(Message::CMessageAddViewPtr)));
   connect(m_game, SIGNAL(SendStartGame(uint,uint)),
           m_gui, SLOT(TakeFieldSize(uint,uint)));
   connect(m_game, SIGNAL(SendError(Message::CMessageInformationPtr)),
           m_gui, SLOT(TakeInInformation(Message::CMessageInformationPtr)));
   connect(m_game, SIGNAL(SendFinishGame(Message::CMessageFinishGamePtr)),
           m_gui, SLOT(TakeFinishGame(Message::CMessageFinishGamePtr)));
   connect(m_game, SIGNAL(signalTimer()), m_game, SLOT(slotTimer()));
   connect(m_gui, SIGNAL(signalDisconnect()), this, SLOT(slotDisconnect()));
   connect(m_gui, SIGNAL(SendClientToServer(Message::CMessageConnectToServerPtr)),
           m_manager, SLOT(TakeServerConnect(Message::CMessageConnectToServerPtr)));
   connect(m_gui, SIGNAL(SendClientToSingleGame(Message::CMessageConnectToSingleGamePtr)),
           m_singleGame, SLOT(TakeServerConnect(Message::CMessageConnectToSingleGamePtr)), Qt::QueuedConnection);

   connect(m_gui, SIGNAL(signalPause()), m_game, SLOT(SlotPause()));
   connect(m_gui, SIGNAL(signalPause()), m_singleGame, SLOT(TakePause()));
   connect(m_gui, SIGNAL(signalExit()), m_game, SLOT(SlotExitGame()));

   connect(m_gui, SIGNAL(signalChoiceNetworkGame()), this, SLOT(slotChoiceNetworkGame()));
   connect(m_gui, SIGNAL(signalChoiceSingleGame()), this, SLOT(slotChoiceSingleGame()));

   connect(m_manager, SIGNAL(SendConfirmConnect(Message::CMessageConfirmationConnectToServerPtr)),
           m_gui, SLOT(TakeConfirmConnectToServer(Message::CMessageConfirmationConnectToServerPtr)));
   connect(m_manager, SIGNAL(SendError(Message::CMessageErrorPtr)),
           m_gui, SLOT(TakeError(Message::CMessageErrorPtr)));
   connect(m_manager, SIGNAL(SendFinishGame(Message::CMessageFinishGamePtr)),
           m_game, SLOT(SlotFinishGame(Message::CMessageFinishGamePtr)));
   connect(m_manager, SIGNAL(SendStateMap(Message::CMessageStateMapPtr)),
           m_game, SLOT(SlotStateMap(Message::CMessageStateMapPtr)));
   connect(m_manager, SIGNAL(SendStartGame(Message::CMessageStartMapGamePtr)),
           m_game, SLOT(SlotStartData(Message::CMessageStartMapGamePtr)));
   connect(m_manager, SIGNAL(SendTimeToStart(Message::CMessageTimeToStartGamePtr)),
           m_gui, SLOT(TakeTimeStartToGame(Message::CMessageTimeToStartGamePtr)));
   connect(m_manager, SIGNAL(SendInInformation(Message::CMessageInformationPtr)),
           m_gui, SLOT(TakeInInformation(Message::CMessageInformationPtr)));

   connect(m_singleGame, SIGNAL(SendConfirmConnect(Message::CMessageConfirmationConnectToServerPtr)),
           m_gui, SLOT(TakeConfirmConnectToServer(Message::CMessageConfirmationConnectToServerPtr)));
   connect(m_singleGame, SIGNAL(SendError(Message::CMessageErrorPtr)),
           m_gui, SLOT(TakeError(Message::CMessageErrorPtr)));
   connect(m_singleGame, SIGNAL(SendFinishGame(Message::CMessageFinishGamePtr)),
           m_game, SLOT(SlotFinishGame(Message::CMessageFinishGamePtr)));
   connect(m_singleGame, SIGNAL(SendStateMap(Message::CMessageStateMapPtr)),
           m_game, SLOT(SlotStateMap(Message::CMessageStateMapPtr)));
   connect(m_singleGame, SIGNAL(SendStartGame(Message::CMessageStartMapGamePtr)),
           m_game, SLOT(SlotStartData(Message::CMessageStartMapGamePtr)));
   connect(m_singleGame, SIGNAL(SendTimeToStart(Message::CMessageTimeToStartGamePtr)),
           m_gui, SLOT(TakeTimeStartToGame(Message::CMessageTimeToStartGamePtr)));
   connect(m_singleGame, SIGNAL(SendInInformation(Message::CMessageInformationPtr)),
           m_gui, SLOT(TakeInInformation(Message::CMessageInformationPtr)));
}

void CGalcon::connectNetworkSendersToTakers()
{   
   connect(m_gui, SIGNAL(SendStepPlayer(Message::CMessageStepPlayerPtr)),
           m_manager, SLOT(TakeStepPlayer(Message::CMessageStepPlayerPtr)));
   connect(m_gui, SIGNAL(signalDisconnect()), m_manager, SLOT(TakeExit()));
}

void CGalcon::disconnectNetworkSendersToTakers()
{
   disconnect(m_gui, SIGNAL(SendStepPlayer(Message::CMessageStepPlayerPtr)),
           m_manager, SLOT(TakeStepPlayer(Message::CMessageStepPlayerPtr)));
   disconnect(m_gui, SIGNAL(signalDisconnect()), m_manager, SLOT(TakeExit()));
}

void CGalcon::connectSingleSendersToTakers()
{
   connect(m_gui, SIGNAL(SendStepPlayer(Message::CMessageStepPlayerPtr)),
           m_singleGame, SLOT(TakeStepPlayer(Message::CMessageStepPlayerPtr)));
   connect(m_gui, SIGNAL(signalExit()), m_singleGame, SLOT(TakeExit()));
}

void CGalcon::disconnectSingleSendersToTakers()
{
   disconnect(m_gui, SIGNAL(SendStepPlayer(Message::CMessageStepPlayerPtr)),
           m_singleGame, SLOT(TakeStepPlayer(Message::CMessageStepPlayerPtr)));
   disconnect(m_gui, SIGNAL(signalDisconnect()), m_singleGame, SLOT(TakeExit()));
}

/// private slots

void CGalcon::slotChoiceNetworkGame()
{
   connectNetworkSendersToTakers();
}

void CGalcon::slotChoiceSingleGame()
{
   connectSingleSendersToTakers();
}

void CGalcon::slotDisconnect()
{
   disconnectSingleSendersToTakers();
   disconnectNetworkSendersToTakers();
}
