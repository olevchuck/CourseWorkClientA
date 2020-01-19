#include "Pausewindow.h"

#include<QPushButton>
namespace GUI
{
   CPauseWindow::CPauseWindow(QWidget *parent) : QDialog(parent)
   {
      m_returnButton = new QPushButton("Return to game", this);
      m_returnButton->setGeometry(30, 30, 200, 50);
      connect(m_returnButton, SIGNAL(clicked()), this, SIGNAL(ReturnToGame()));

      m_exitButton = new QPushButton("Exit to main menu", this);
      m_exitButton->setGeometry(30, 110, 200, 50);
      connect(m_exitButton, SIGNAL(clicked()), this, SIGNAL(ExitGame()));
   }

} //Namespace GUI

