#include <QMessageBox>

#include "Errorwindow.h"

namespace GUI
{
   CErrorWindow::CErrorWindow(QObject *parent) :
       QObject(parent)
   {

   }

   void CErrorWindow::Show(const QString & title, const QString & text, bool sync)
   {
       QMessageBox box;
       box.setText(text);
       box.setWindowTitle(title);

       if (sync)
           box.exec();
       else
           box.show();
   }
} // Namespace GUI
