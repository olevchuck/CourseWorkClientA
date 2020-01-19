#include "Guispace.h"

namespace GUI
{
   CGUISpace::CGUISpace(unsigned int x, unsigned int y):m_width(x), m_height(y)
   {
   }

   void CGUISpace::Draw(QPainter* painter)
   {
      painter->setBrush(Qt::black);
      painter->drawRect(0, 0, m_width, m_height);

      QPixmap pm(":/GUI/backspace.png");
      QSize areaSize(m_width, m_height);
      QPixmap space(pm.scaled(areaSize, Qt::IgnoreAspectRatio));
      painter->drawPixmap(0,0, space);
   }
} // Namespace GUI
