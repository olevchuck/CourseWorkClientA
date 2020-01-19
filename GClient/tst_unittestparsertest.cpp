#include <QtCore/QString>
#include <QtTest/QtTest>

#include <string>

#include "serverManager/Parser.h"
#include "message/MessageConnectToServer.h"
#include "message/MessageError.h"
#include "message/MessageFinishGame.h"
#include "message/MessageStateMap.h"
#include "message/MessageStartMapGame.h"
#include "message/MessageTimeToStartGame.h"

class UnitTestParserTest : public QObject
{
    Q_OBJECT

public:
    UnitTestParserTest();

private Q_SLOTS:
    void testCParser_CheckTypeMessage();

    void testCParser_ParseMConfirmConnect();

    void testCParser_ParseMError();

    void testCParser_ParseMFinishGame();

    void testCParser_ParseMStateMap();

    void testCParser_ParseMStartMapGame();

    void testCParser_ParseMTimeToStartGame();

private:
    ServerManagerDecl::CParser m_parser;

};

UnitTestParserTest::UnitTestParserTest()
{
}

void UnitTestParserTest::testCParser_CheckTypeMessage()
{  
   std::string sMes;
   sMes = "SC_CONNID#3##";
   QVERIFY2(m_parser.CheckTypeMessage(sMes) == ServerManagerDecl::CParser::eConfirmConnect,
            "Failure eConfirmConnect");

   sMes = "SC_TIMETOSTART#9##";
   QVERIFY2(m_parser.CheckTypeMessage(sMes) == ServerManagerDecl::CParser::eTimeToStart,
            "Failure eTimeToStart");

   sMes = "SC_START#100#80#2#25#(3,0,50,70,45,8)(4,2,80,60,12,20)#(1,Red Fox)(2,Star_123456)##";
   QVERIFY2(m_parser.CheckTypeMessage(sMes) == ServerManagerDecl::CParser::eStartGame,
            "Failure eStartGame");

   sMes = "SC_STATE#(2,0,15)(3,0,8)(4,4,8)#(1,2,4,2,12,0)(2,2,3,2,24,0)##";
   QVERIFY2(m_parser.CheckTypeMessage(sMes) == ServerManagerDecl::CParser::eStateMap,
            "Failure eStateMap");

   sMes = "SC_FINISH#2##";
   QVERIFY2(m_parser.CheckTypeMessage(sMes) == ServerManagerDecl::CParser::eFinishGame,
            "Failure eFinishGame");

   sMes = "SC_ERR#Player1 disconnected!##";
   QVERIFY2(m_parser.CheckTypeMessage(sMes) == ServerManagerDecl::CParser::eError,
            "Failure eError");

   sMes = "ERROR_MESSAGE_TYPE#Player1 disconnected!##";
   QVERIFY2(m_parser.CheckTypeMessage(sMes) == ServerManagerDecl::CParser::eUnknown,
            "Failure eUnknown");
}

void UnitTestParserTest::testCParser_ParseMConfirmConnect()
{
   std::string sMes = "SC_CONNID#3##";
   Message::CMessageConfirmationConnectToServerPtr ptr =
      m_parser.ParseMConfirmConnect(sMes);

   QVERIFY2(ptr->m_playerID == 3, "SC_CONNID");
}

void UnitTestParserTest::testCParser_ParseMError()
{
   std::string sMes = "SC_ERR#Player1 disconnected!##";
   Message::CMessageErrorPtr ptr = m_parser.ParseMError(sMes);

   QVERIFY2(ptr->m_strError == std::string("Player1 disconnected!"), "SC_ERR");
}

void UnitTestParserTest::testCParser_ParseMFinishGame()
{
   std::string sMes = "SC_FINISH#2##";
   Message::CMessageFinishGamePtr ptr =
      m_parser.ParseMFinishGame(sMes);   

   QVERIFY2(ptr->m_playerID == 2, "SC_FINISH");
}

void UnitTestParserTest::testCParser_ParseMStateMap()
{
   std::string sMes = "SC_STATE#(3,0,8)(4,4,8)#(1,2,4,2,12,0)(2,2,3,2,24,0)##";
   Message::CMessageStateMapPtr ptr =
      m_parser.ParseMStateMap(sMes);
   // (3,0,8)
   QVERIFY2(ptr->m_planetState[0].m_planetID == 3, "SC_STATE");
   QVERIFY2(ptr->m_planetState[0].m_playerID == 0, "SC_STATE");
   QVERIFY2(ptr->m_planetState[0].m_countFleet == 8, "SC_STATE");
   // (4,4,8)
   QVERIFY2(ptr->m_planetState[1].m_planetID == 4, "SC_STATE");
   QVERIFY2(ptr->m_planetState[1].m_playerID == 4, "SC_STATE");
   QVERIFY2(ptr->m_planetState[1].m_countFleet == 8, "SC_STATE");
   // (1,2,4,2,12,0)
   QVERIFY2(ptr->m_fleetState[0].m_fleetID == 1, "SC_STATE");
   QVERIFY2(ptr->m_fleetState[0].m_playerID == 2, "SC_STATE");
   QVERIFY2(ptr->m_fleetState[0].m_planetStartID == 4, "SC_STATE");
   QVERIFY2(ptr->m_fleetState[0].m_planetFinishID == 2, "SC_STATE");
   QVERIFY2(ptr->m_fleetState[0].m_countFleet == 12, "SC_STATE");
   QVERIFY2(ptr->m_fleetState[0].m_percentRoute == 0, "SC_STATE");
   // (2,2,3,2,24,0)
   QVERIFY2(ptr->m_fleetState[1].m_fleetID == 2, "SC_STATE");
   QVERIFY2(ptr->m_fleetState[1].m_playerID == 2, "SC_STATE");
   QVERIFY2(ptr->m_fleetState[1].m_planetStartID == 3, "SC_STATE");
   QVERIFY2(ptr->m_fleetState[1].m_planetFinishID == 2, "SC_STATE");
   QVERIFY2(ptr->m_fleetState[1].m_countFleet == 24, "SC_STATE");
   QVERIFY2(ptr->m_fleetState[1].m_percentRoute == 0, "SC_STATE");
}

void UnitTestParserTest::testCParser_ParseMStartMapGame()
{
   std::string sMes = "SC_START#100#80#2#25#(3,0,50,70,45,8)(4,2,80,60,12,20)#(1,Red Fox)(2,Star_123456)##";
   Message::CMessageStartMapGamePtr ptr =
      m_parser.ParseMStartMapGame(sMes);
   // #100#80#2#25#
   QVERIFY2(ptr->m_mapX == 100, "SC_START");
   QVERIFY2(ptr->m_mapY == 80, "SC_START");
   QVERIFY2(ptr->m_growV == 2, "SC_START");
   QVERIFY2(ptr->m_flyV == 25, "SC_START");
   // (3,0,50,70,45,8)
   QVERIFY2(ptr->m_planetData[0].m_planetID == 3, "SC_START");
   QVERIFY2(ptr->m_planetData[0].m_playerID == 0, "SC_START");
   QVERIFY2(ptr->m_planetData[0].m_planetX == 50, "SC_START");
   QVERIFY2(ptr->m_planetData[0].m_planetY == 70, "SC_START");
   QVERIFY2(ptr->m_planetData[0].m_planetR == 45, "SC_START");
   QVERIFY2(ptr->m_planetData[0].m_countFleet == 8, "SC_START");
   // (4,2,80,60,12,20)
   QVERIFY2(ptr->m_planetData[1].m_planetID == 4, "SC_START");
   QVERIFY2(ptr->m_planetData[1].m_playerID == 2, "SC_START");
   QVERIFY2(ptr->m_planetData[1].m_planetX == 80, "SC_START");
   QVERIFY2(ptr->m_planetData[1].m_planetY == 60, "SC_START");
   QVERIFY2(ptr->m_planetData[1].m_planetR == 12, "SC_START");
   QVERIFY2(ptr->m_planetData[1].m_countFleet == 20, "SC_START");
   // (1,Red Fox)
   QVERIFY2(ptr->m_playerData[0].m_playerID == 1, "SC_START");
   QVERIFY2(ptr->m_playerData[0].m_playerName == "Red Fox", "SC_START");
   // (2,Star_123456)
   QVERIFY2(ptr->m_playerData[1].m_playerID == 2, "SC_START");
   QVERIFY2(ptr->m_playerData[1].m_playerName == "Star_123456", "SC_START");
}

void UnitTestParserTest::testCParser_ParseMTimeToStartGame()
{
   std::string sMes = "SC_TIMETOSTART#9##";
   Message::CMessageTimeToStartGamePtr ptr =
      m_parser.ParseMTimeToStartGame(sMes);

   QVERIFY2(ptr->m_second == 9, "SC_TIMETOSTART");
}

QTEST_APPLESS_MAIN(UnitTestParserTest);

#include "tst_unittestparsertest.moc"
