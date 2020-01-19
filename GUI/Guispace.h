#pragma once

#include <QPainter>

namespace GUI
{
   class CGUISpace
   {
   public:
       CGUISpace(){}
       CGUISpace(unsigned int x, unsigned int y);

       void Draw(QPainter*);

   private:
       unsigned int m_width;
       unsigned int m_height;
   };
} // Namespace GUI
