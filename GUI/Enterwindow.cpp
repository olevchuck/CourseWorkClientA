#include <QHostAddress>
#include <QVariant>

#include "Enterwindow.h"
#include "ui_enterwindow.h"
#include "Errorwindow.h"


namespace GUI
{
   CEnterWindow::CEnterWindow(QWidget *parent) :
       QDialog(parent),
       ui(new Ui::CEnterWindow)
   {
       ui->setupUi(this);
       QPixmap bkgnd(":/GUI/sky.png");
       bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
       QPalette palette;
       palette.setBrush(QPalette::Background, bkgnd);
       this->setPalette(palette);
   }

   CEnterWindow::~CEnterWindow()
   {
       delete ui;
   }

   void CEnterWindow::ShowWindow()
   {
      show();
   }

   void CEnterWindow::DestroyWindow()
   {
      hide();
   }

   void CEnterWindow::on_connectButton_clicked()
   {
       CheckAndSend();
   }

   void CEnterWindow::CheckAndSend()
   {
      if (ui->m_ip->text() == "")
      {
          CErrorWindow::Show("Error", "IP not entered");
          return;
      }
      else if (ui->m_port->text() == "")
      {
         CErrorWindow::Show("Error", "Port not entered");
         return;
      }
      else if (ui->m_name->text() == "")
      {
         CErrorWindow::Show("Error", "Name not entered");
         return;
      }
      QHostAddress host;
      if (!host.setAddress(ui->m_ip->text()))
      {
         CErrorWindow::Show("Error", "Bad IP");
         return;
      }
      if (!(QVariant(ui->m_port->text()).canConvert(QVariant::Int)))
      {
         CErrorWindow::Show("Error", "Port is not valid");
         return;
      }
      Message::CMessageConnectToServerPtr mess(new Message::CMessageConnectToServer());
      mess->m_namePlayer = ui->m_name->text().toStdString();
      mess->m_serverIP = ui->m_ip->text().toStdString();
      mess->m_serverPort = ui->m_port->text().toInt();
      emit SendClientToServer(mess);
   }
} // Namespace GUI
