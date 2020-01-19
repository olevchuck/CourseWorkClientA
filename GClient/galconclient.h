#pragma once

#include <QDialog>

#include "serverManager/ServerManager.h"
#include "message/MessageInformation.h"

namespace Ui {
    class CGalconClient;
}

class CGalconClient : public QDialog
{
    Q_OBJECT

public:
    explicit CGalconClient(QWidget *parent = 0);
    ~CGalconClient();

private slots:
   void on_pBServerManager_clicked();

   void on_pBSend_clicked();

   void on_pBStepPlayer_clicked();

   void showInfMessage(Message::CMessageInformationPtr sMes);

private:
    Ui::CGalconClient *ui;
    ServerManagerDecl::CServerManager m_serverManager;
};

