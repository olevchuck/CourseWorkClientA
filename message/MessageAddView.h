#pragma once

#include <QSharedPointer>

namespace GUI
{
   class ISceneUpdates;
} // Namespace GUI

namespace Message
{   
   struct CMessageAddView;

   typedef QSharedPointer<CMessageAddView> CMessageAddViewPtr;

   struct CMessageAddView
   {
      GUI::ISceneUpdates* m_view;

   }; // struct CMessageAddView   

} // namespace Message
