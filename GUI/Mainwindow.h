#pragma once

#include <QDialog>

namespace Ui
{
    class CMainWindow;
}

namespace GUI
{
   class CMainWindow : public QDialog
   {
       Q_OBJECT

   public:
       explicit CMainWindow(QWidget *parent = 0);
       ~CMainWindow();

      void ShowWindow();
      void DestroyWindow();

   signals:
      void signalChoiceSingleGame();
      void signalChoiceNetworkGame();

   private slots:
      void on_pushButtonSingle_clicked();

      void on_pushButtonNetwork_clicked();

   private:
       Ui::CMainWindow *ui;

   }; // class CMainWindow

} // namespace GUI

