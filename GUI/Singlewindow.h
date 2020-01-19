#pragma once

#include <QDialog>
#include <QValidator>
#include <QLineEdit>

#include "message/MessageConnectToSingleGame.h"

namespace Ui
{
    class CSingleWindow;
}

namespace GUI
{

   class CSingleWindow : public QDialog
   {
       Q_OBJECT

   public:
      explicit CSingleWindow(QWidget *parent = 0);
      ~CSingleWindow();

      void ShowWindow();
      void DestroyWindow();

   signals:
      void SendClientToSingleGame(const Message::CMessageConnectToSingleGamePtr mess);

   private slots:
      void on_pushButtonStartPlay_clicked();

      void on_lineEditMapWidth_editingFinished();
      void on_lineEditMapHeigt_editingFinished();
      void on_lineEditPlanetMinSize_editingFinished();
      void on_lineEditPlanetMaxSize_editingFinished();
      void on_lineEditFleetMinCount_editingFinished();
      void on_lineEditFleetMaxCount_editingFinished();
      void on_lineEditFlySpeed_editingFinished();
      void on_lineEditGrowSpeed_editingFinished();
      void on_lineEditDispersion_editingFinished();

   private:
      bool checkInputData();
      void checkInputDataToEdit(QLineEdit* edit);

      Ui::CSingleWindow *ui;

      QIntValidator* m_validator;

   }; // class CSingleWindow

} // namespace GUI


