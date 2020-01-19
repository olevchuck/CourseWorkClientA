#include <QtCore/QString>
#include <QtTest/QtTest>

#include "Planet.h"
#include "Fleet.h"
#include "Space.h"
#include "Game.h"

class UnitTestFinalTest : public QObject
{
    Q_OBJECT

public:
    UnitTestFinalTest();

private Q_SLOTS:
    void initTestCase();
    void testCasePlanet();
    void testCaseFleet();
    void testCaseSpace();
    void testCaseGame();
};

UnitTestFinalTest::UnitTestFinalTest()
{
}

void UnitTestFinalTest::initTestCase()
{

}

void UnitTestFinalTest::testCasePlanet()
{
   CPlanet pl(1, 0, 10, 5, 100500, 2);
   QVERIFY2(pl.GetPlayerId() == 2, "Failure player id");
   QVERIFY2(pl.GetArmy() == 100500, "Failure army");
   unsigned int x(0), y(0);
   pl.GetPosition(x,y);
   QVERIFY2((x==0) && (y==10), "Failure coords");
   QVERIFY2(pl.GetRadius() == 5, "Failure radius");

   pl.SetArmy(400);
   QVERIFY2(pl.GetArmy() == 400, "Failure army");
   pl.SetPlayer(3);
   QVERIFY2(pl.GetPlayerId() == 3, "Failure playerId");
}

void UnitTestFinalTest::testCaseFleet()
{
   const double eps = 0.0001;
   CPlanet* fr = new CPlanet(1, 0, 10, 5, 100500, 1);
   CPlanet* to = new CPlanet(2, 10, 20, 15, 100, 2);
   CFleet fl(0, fr, to, 1, 500, 50);
   QVERIFY2(fl.GetPlayerId() == 1, "Failture player");
   QVERIFY2(fl.GetShipCount() == 500, "Failture shipcount");
   double x(0), y(0);
   fl.GetPosition(x,y);
   QVERIFY2((x - 5 < eps) && (y - 15 < eps), "Failture count real position");

   CPlanet* fr2 = new CPlanet(1, 10, 10, 5, 100500, 1);
   CPlanet* to2 = new CPlanet(2, 10, 20, 15, 100, 2);
   CFleet fl2(0, fr2, to2, 1, 500, 50);
   fl2.IncreaseWay(2);
   fl2.GetPosition(x,y);
   QVERIFY2(x - 10 < eps && y - 17 <eps, "Failture increase procent");

   fl.SetPercent(10);
   fl.GetPosition(x,y);
   QVERIFY2((x - 1 < eps) && (y - 11 < eps), "Failture set procent");

   fl.SetPercent(100);
   QVERIFY2(fl.ReachedDestination(), "Failture reach destination");

   fl.SetPercent(200);
   fl.GetPosition(x,y);
   QVERIFY2((x - 10 < eps) && (y - 20 < eps) && fl.ReachedDestination() ,
            "Failture 100% destination");
   QVERIFY2(to->GetPlayerId() == 1, "Failture capture planet");
   QVERIFY2(to->GetArmy() == 400, "Failture capture planet");

}

void UnitTestFinalTest::testCaseSpace()
{
   const double eps = 0.0001;
   Message::CPlanetStartData data = {100, 1, 5 , 10, 10, 1};
   Message::CPlanetStartData data2 = {200, 2, 15 , 10, 20, 2};
   std::vector<Message::CPlanetStartData> vec;
   vec.push_back(data);
   vec.push_back(data2);
   Message::CPlayerStartData pl1 = {1, "Vasya"};
   Message::CPlayerStartData pl2 = {2, "Petya"};
   std::vector<Message::CPlayerStartData> plVec;
   plVec.push_back(pl1);
   plVec.push_back(pl2);
   CSpace* sp = new CSpace(2, 1, 300, 400, vec);
   std::vector<CPlanet*> planets(sp->GetPlanets());

   QVERIFY2(planets.size() == 2, "Fail add planets to space");

   QVERIFY2(planets[0]->GetArmy() == 100 , "Fail add correct planet to space");

   sp->Update(2);
   QVERIFY2((planets[0]->GetArmy() == 102) && (planets[1]->GetArmy() == 202) , "Fail to update space");


   Message::CStatePlanet stPlanet = { 230, 1, 2 };
   std::vector<Message::CStatePlanet> stVecPl;
   //a[1] = std::pair<unsigned long, unsigned short>(230,2);
   stVecPl.push_back(stPlanet);
   sp->SetPlanets(stVecPl);
   QVERIFY2((planets[0]->GetArmy() == 230 && planets[0]->GetPlayerId() == 2) , "Fail to set planets in space");

   CPlanet* pl = sp->GetPlanetById(1);
   QVERIFY2(pl->GetArmy() == 230 , "Fail get planet by id");

   Message::CStateFleet mes = { 220, 1, 50, 2, 1, 1 };
   std::vector<Message::CStateFleet> mesVec;
   mesVec.push_back(mes);
   sp->SetFleets(mesVec);
   CFleet* fl = sp->GetFleets().front();
   QVERIFY2(fl->GetShipCount() == 220, "Fail to set fleet");

   double x(0), y(0);
   fl->GetPosition(x,y);
   QVERIFY2(y == 15, "Fail update fleet");
   QVERIFY2(planets[1]->GetArmy() == 202, "Fail to attack planet");
   sp->Update(3); // Speed = 2!
   fl->GetPosition(x,y);
   QVERIFY2(y == 20 , "Fail update fleet");

   sp->Update(1);
   QVERIFY2(planets[1]->GetArmy() == 16 && planets[1]->GetPlayerId() == 1, "Fail to attack planet");

   QVERIFY2(sp->GetFleets().empty(), "Fail to delete fleet after attack");

}

void UnitTestFinalTest::testCaseGame()
{
   CGame* game = new CGame();

}

QTEST_APPLESS_MAIN(UnitTestFinalTest);

#include "tst_unittestfinaltest.moc"
