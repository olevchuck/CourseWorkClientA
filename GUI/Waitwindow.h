#pragma once

#include <QDialog>
#include <QTimer>

#include "message/MessageTimeToStartGame.h"

namespace Ui
{
    class CWaitWindow;
}

namespace GUI
{

   class CWaitWindow : public QDialog
   {
       Q_OBJECT

   public:
       explicit CWaitWindow(QWidget *parent = 0);
       ~CWaitWindow();

      void ShowWindow();
      void DestroyWindow();

      void SetSecToStart(const unsigned int seconds);

   private slots:
       void slotIncremetTimer();

   private:
       unsigned int m_secondToStart;
       QTimer m_timer;
       Ui::CWaitWindow *ui;
   };
} // Namespace GUI
