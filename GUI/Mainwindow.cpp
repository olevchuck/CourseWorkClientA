#include "Mainwindow.h"
#include "ui_mainwindow.h"


namespace GUI
{
   CMainWindow::CMainWindow(QWidget *parent) :
       QDialog(parent),
       ui(new Ui::CMainWindow)
   {
       ui->setupUi(this);
       QPixmap bkgnd(":/GUI/fon.jpg");
       bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
       QPalette palette;
       palette.setBrush(QPalette::Background, bkgnd);
       this->setPalette(palette);
   }

   CMainWindow::~CMainWindow()
   {
       delete ui;
   }

   void CMainWindow::ShowWindow()
   {
      show();
   }

   void CMainWindow::DestroyWindow()
   {
      hide();
   }

   void CMainWindow::on_pushButtonSingle_clicked()
   {
      emit signalChoiceSingleGame();
   }

   void CMainWindow::on_pushButtonNetwork_clicked()
   {
      emit signalChoiceNetworkGame();
   }
}




