#include "Galcon.h"
#include <QApplication>
#include <QFontDatabase>

int main (int argc, char* argv[])
{
   QApplication a(argc, argv);
   CGalcon game;
   QFontDatabase fontDB;
   fontDB.addApplicationFont(":/GUI/font.ttf");

   return a.exec();
}
