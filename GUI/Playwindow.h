#pragma once

#include <QWidget>
#include "message/MessageStepPlayer.h"

class CPlayer;

namespace GUI
{
   class CGUIView;

   class CPlayWindow : public QWidget
   {
       Q_OBJECT
   public:
       explicit CPlayWindow(QWidget* parent  = 0);

       void CreateWindow(const unsigned int x, const unsigned int y);
       void SetPlayers(std::vector<CPlayer* >* players);
       void DestroyWindow();
       void ShowWindow();

       void paintEvent(QPaintEvent *);

       void mousePressEvent(QMouseEvent *);
       void mouseMoveEvent(QMouseEvent *);
       void mouseReleaseEvent(QMouseEvent *);
       void mouseDoubleClickEvent(QMouseEvent *);
       void wheelEvent(QWheelEvent *);
       void resizeEvent(QResizeEvent *);
       void keyPressEvent(QKeyEvent *);

       void mouseClick();

       CGUIView* GetView();

       void SetPlayerId(unsigned int id);
       void GetSize (unsigned int& width, unsigned int& height);
       void GetLogicalFromActual(int& x, int& y) const;

   signals:
       void SendStepPlayer(Message::CMessageStepPlayerPtr mess);
       void PauseGame();
   private:
       enum EMouseClick {
          ESingle,
          EDouble,
          ENone
       };

       enum EMouseState {
          EPlanetsSelected,
          ENotSelected
       };
       EMouseState m_mouseState;
       EMouseClick m_mouseClick;
       bool m_mouseRelease;

       int m_mousePressedX;
       int m_mousePressedY;
       int m_mouseCurrentX;
       int m_mouseCurrentY;

       CGUIView* m_view;
       std::vector<CPlayer*>* m_players;
       unsigned int m_width;
       unsigned int m_height;
       unsigned int m_logicalWidth;
       unsigned int m_logicalHeight;
       unsigned int m_playerId;
   };
} // Namespace GUI
