#pragma once

#include <QObject>

namespace GUI
{
   class CErrorWindow : public QObject
   {
       Q_OBJECT
   public:
       explicit CErrorWindow(QObject *parent = 0);
       static void Show(const QString & title, const QString & text, bool sync = true);

   };
} // Namespace GUI
